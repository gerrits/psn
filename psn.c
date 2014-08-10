#include "psn.h"
#include "init.h"

static bool connected = false;

psn_err psn_init(struct psn_s *psn)
{
    if (init_tomcrypt_lib()) {
        printf("* ERROR: libtomcrypt init failed.\n");
        return PSN_ERR_FAIL;
    }
    if (init_mosquitto_lib()) {
        printf("* ERROR: libmosquitto init failed\n");
        return PSN_ERR_FAIL;
    }

    //init uthash
    psn->friends = NULL;
    psn->friend_requests_incoming = NULL;
    psn->friend_requests_outgoing = NULL;

    return PSN_ERR_SUCCESS;
}

psn_err psn_connect(struct psn_s *psn)
{
    if (psn->mosq == NULL) {
        mosquitto_reinitialise(psn->mosq, psn->username, true, psn);
    } else {
        psn->mosq = mosquitto_new(psn->username, true, psn);
    }

    mosquitto_message_callback_set(psn->mosq, psn_message_callback);

    if (psn->mosq == NULL) {
        DEBUG("Could not create mosquitto instance.\n%s", "");
        return PSN_ERR_FAIL;
    }

    mosquitto_username_pw_set(psn->mosq, psn->username, psn->password);
    if (mosquitto_connect(psn->mosq, psn->hostname, psn->port, 60)) {
        DEBUG("Unable to connect.\n%s", "");
        return PSN_ERR_COULD_NOT_CONNECT;
    }

    //subscribe to users topics
    char topic[128];
    sprintf(topic, "users/%s/#", psn->username);
    mosquitto_subscribe(psn->mosq, NULL, topic, 1);

    sprintf(topic, "chat/+/%s", psn->username);
    mosquitto_subscribe(psn->mosq, NULL, topic, 1);

    if (mosquitto_loop_start(psn->mosq) != MOSQ_ERR_SUCCESS) {
        DEBUG("Loop start failed\n%s", "");
        return PSN_ERR_FAIL;
    }

    connected = true;
    DEBUG("Connection successful\n%s", "");

    return PSN_ERR_SUCCESS;
}

psn_err psn_disconnect(struct psn_s *psn)
{
    if (psn == NULL || psn->mosq == NULL) {
        DEBUG("psn or psn->mosq instance invalid\n%s", "");
        return PSN_ERR_FAIL;
    }
    if (!connected) {
        DEBUG("already connected\n%s", "");
        return PSN_ERR_ALREADY_CONNECTED;
    }
    mosquitto_disconnect(psn->mosq);
    mosquitto_loop_stop(psn->mosq, false);
    connected = false;
    return PSN_ERR_SUCCESS;
}

psn_err psn_generate_new_key(struct psn_s *psn)
{
    int err;

    if ((err = rsa_make_key(0,
                            find_prng("sprng"),
                            1024 / 8, 65537, &psn->pk_key))
        != CRYPT_OK) {
        DEBUG("rsa_make_key failed: %s\n", error_to_string(err));
        return PSN_ERR_FAIL;
    }
    return PSN_ERR_SUCCESS;
}


psn_err psn_set_server(struct psn_s *psn, char *hostname, int port)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }

    strcpy(psn->hostname, hostname);
    psn->port = port;

    return PSN_ERR_SUCCESS;
}

psn_err psn_set_username(struct psn_s *psn, char *username, char *password)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    strcpy(psn->password, password);
    strcpy(psn->username, username);
    strcpy(psn->shown_name, username);

    return PSN_ERR_SUCCESS;
}

psn_err psn_set_shown_name(struct psn_s *psn, char *shown_name)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    strcpy(psn->shown_name, shown_name);

    return PSN_ERR_SUCCESS;
}

void psn_message_callback(struct mosquitto *mosq, void *userdata,
                          const struct mosquitto_message *message)
{
    char **topics = NULL;
    int topic_count;

    struct psn_s *psn = (struct psn_s *) userdata;

    mosquitto_sub_topic_tokenise(message->topic, &topics, &topic_count);
    if (topic_count == 3) {
        if (!strcmp(topics[0], "chat") && !strcmp(topics[2], psn->username) && message->payloadlen) {
            //A message has been received
            DEBUG("check if the sender(%s) of the incoming message is in friend list\n", topics[1]);

            struct user_s *in_list = NULL;
            HASH_FIND_STR(psn->friends, topics[1], in_list);

            if (in_list != NULL) {
                DEBUG("Sender is in list%s\n", "");
                //ensure NULL-Byte at end of the payload
                ((char *) message->payload)[message->payloadlen] = '\0';
                printf("\r%s: %s\n> ", topics[1], message->payload);
            } else {
                DEBUG("Sender was not in list, doing nothing%s\n", "");
            }

        }
    }

    if (topic_count == 4) {
        if (!strcmp(topics[0], "users") && !strcmp(topics[2], "frequest")) {
            //A Friend Request is incoming
            struct user_s *from_list = NULL;

            HASH_FIND_STR(psn->friend_requests_incoming, topics[3], from_list);
            if (from_list != NULL) {
                //Friend is already in incoming friends list
                //do nothing
                mosquitto_sub_topic_tokens_free(&topics, topic_count);
                return;
            }

            HASH_FIND_STR(psn->friends, topics[3], from_list);
            if (from_list != NULL) {
                //Friend is already in friends list
                //do nothing

                mosquitto_sub_topic_tokens_free(&topics, topic_count);
                return;
            }

            HASH_FIND_STR(psn->friend_requests_outgoing, topics[3], from_list);
            if (from_list != NULL) {
                //Friend is already in outgoing friends list
                //the friend has accepted the friend request
                HASH_DEL(psn->friend_requests_outgoing, from_list);

                struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));

                strcpy(new->name, topics[3]);
                strcpy(new->shown_name, topics[3]);

                HASH_ADD_STR(psn->friends, name, new);
                printf("\r* %s has accepted your friend request\n> ", new->name);

                mosquitto_sub_topic_tokens_free(&topics, topic_count);
                fflush(stdout);
                return;
            }
            //source is not in any list, so push it to incoming list
            struct user_s *new = (struct user_s *)malloc(sizeof(struct user_s));

            strcpy(new->name, topics[3]);
            strcpy(new->shown_name, topics[3]);

            HASH_ADD_STR(psn->friend_requests_incoming, name, new);
            printf("\r* %s sent you a friend request\n> ", new->name);
        }
    }

    fflush(stdout);
    mosquitto_sub_topic_tokens_free(&topics, topic_count);
    return;
}

psn_err psn_make_friend_req(struct psn_s *psn , char *target, char *message)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }

    if (psn->mosq == NULL) {
        DEBUG("mosquitto instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }

    if (!connected) {
        printf("* ERROR: not connected\n");
        return PSN_ERR_NOT_CONNECTED;
    }

    //build topic
    char topic[128];
    sprintf(topic, "users/%s/frequest/%s", target, psn->username);

    //get length of the message
    int len = strlen(message);

    //publish the friend request
    mosquitto_publish(psn->mosq, NULL, topic, len, message , 1, false);

    struct user_s *new = malloc(sizeof(struct user_s));
    strcpy(new->name, target);
    strcpy(new->shown_name, target);

    HASH_ADD_STR(psn->friend_requests_outgoing, name, new);
    return PSN_ERR_SUCCESS;
}

psn_err psn_accept_friend_req(struct psn_s *psn, char *target)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }

    if (psn->mosq == NULL) {
        DEBUG("mosquitto instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    if (!connected) {
        printf("* ERROR: not connected\n");
        return PSN_ERR_NOT_CONNECTED;
    }

    struct user_s *in_list = NULL;
    HASH_FIND_STR(psn->friend_requests_incoming, target, in_list);
    if (in_list == NULL) {
        DEBUG("Cannot accept %s, not in incoming list\n", target);
        return PSN_ERR_USER_NOT_IN_INCOMING_FRIEND_LIST;
    }

    HASH_DEL(psn->friend_requests_incoming, in_list);

    //build topic
    char topic[128];
    sprintf(topic, "users/%s/frequest/%s", target, psn->username);

    //publish the friend request
    mosquitto_publish(psn->mosq, NULL, topic, 0, "", 1, false);

    //add to friend list
    HASH_ADD_STR(psn->friends, name, in_list);
    return PSN_ERR_SUCCESS;
}

psn_err psn_refuse_friend_req(struct psn_s *psn, char *target)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }

    //to refuse a friend request, delete the incoming frq
    struct user_s *in_list;
    HASH_FIND_STR(psn->friend_requests_incoming, target, in_list);

    if (in_list == NULL) {
        //user was not in incoming list
        return PSN_ERR_USER_NOT_IN_INCOMING_FRIEND_LIST;
    }

    //delete user from incoming list
    HASH_DEL(psn->friend_requests_incoming, in_list);
    free(in_list);
    return PSN_ERR_SUCCESS;
}

psn_err psn_delete_friend(struct psn_s *psn, char *target)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    struct user_s *in_list = NULL;

    HASH_FIND_STR(psn->friends, target, in_list);
    if (in_list == NULL) {
        //user was not in friend list
        return PSN_ERR_USER_NOT_IN_FRIEND_LIST;
    }

    //delete user from friend list
    HASH_DEL(psn->friends, in_list);
    free(in_list);
    return PSN_ERR_SUCCESS;
}

psn_err psn_send_message(struct psn_s *psn, char *target, char *message)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    if (!connected) {
        printf("*ERROR: not connected\n");
        return PSN_ERR_NOT_CONNECTED;
    }
    struct user_s *in_list = NULL;
    HASH_FIND_STR(psn->friends, target, in_list);

    if (in_list == NULL) {
        printf("*ERROR: target not in friend list\n");
        return PSN_ERR_USER_NOT_IN_FRIEND_LIST;
    }


    //build topic
    char topic[128];
    sprintf(topic, "chat/%s/%s", psn->username, target);

    int len = strlen(message);
    mosquitto_publish(psn->mosq, NULL, topic, len, message, 1, false);
    return PSN_ERR_SUCCESS;
}

int psn_get_friend_list(struct psn_s *psn, char ***friends)
{
    struct user_s *s;
    int i = 0;
    int len = 0;

    //count number of friends
    for (s = psn->friends; s != NULL; s = s->hh.next) {
        len++;
    }

    *friends = malloc(len * sizeof(char **));

    for (s = psn->friends; s != NULL; s = s->hh.next) {
        *friends[i] = malloc(32 * sizeof(char));
        strcpy(*friends[i], s->name);
        i++;
    }

    return len;
}

psn_err psn_serialize_config(struct psn_s *psn, char *dest_str)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    json_t *root = json_object();
    struct user_s *s;

    json_object_set_new(root, "username", json_string(psn->username));
    json_object_set_new(root, "hostname", json_string(psn->hostname));
    json_object_set_new(root, "port", json_integer(psn->port));

    json_t *friend_list = json_array();
    for (s = psn->friends; s != NULL; s = s->hh.next) {
        json_t *new_obj = json_object();
        json_object_set_new(new_obj, "name", json_string(s->name));
        json_object_set_new(new_obj, "shown_name", json_string(s->shown_name));
        json_array_append(friend_list, new_obj);
    }

    json_t *friend_req_i_list = json_array();
    for (s = psn->friend_requests_incoming; s != NULL; s = s->hh.next) {
        json_t *new_obj = json_object();
        json_object_set_new(new_obj, "name", json_string(s->name));
        json_object_set_new(new_obj, "shown_name", json_string(s->shown_name));
        json_array_append(friend_req_i_list, new_obj);
    }

    json_t *friend_req_o_list = json_array();
    for (s = psn->friend_requests_outgoing; s != NULL; s = s->hh.next) {
        json_t *new_obj = json_object();
        json_object_set_new(new_obj, "name", json_string(s->name));
        json_object_set_new(new_obj, "shown_name", json_string(s->shown_name));
        json_array_append(friend_req_o_list, new_obj);
    }

    json_object_set_new(root, "friends", friend_list);
    json_object_set_new(root, "friendrqsinc", friend_req_i_list);
    json_object_set_new(root, "friendrqsout", friend_req_o_list);

    char *json_result = json_dumps(root, 0);

    strcpy(dest_str, json_result);

    free(json_result);

    return PSN_ERR_SUCCESS;
}

psn_err psn_deserialize_config(struct psn_s *psn, char *src_str)
{
    if (psn == NULL) {
        DEBUG("psn instance not valid\n%s", "");
        return PSN_ERR_FAIL;
    }
    json_error_t error;
    json_t *friend_arr;
    int i;

    json_t *root = json_loads(src_str, 0, &error);

    strcpy(psn->username, json_string_value(json_object_get(root, "username")));
    strcpy(psn->shown_name, json_string_value(json_object_get(root, "username")));
    strcpy(psn->hostname, json_string_value(json_object_get(root, "hostname")));
    psn->port = json_integer_value(json_object_get(root, "port"));

    friend_arr = json_object_get(root, "friends");
    for (i = 0; i < json_array_size(friend_arr); i++) {
        struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));
        json_t *new_obj = json_array_get(friend_arr, i);
        strcpy(new->name, json_string_value(json_object_get(new_obj, "name")));
        strcpy(new->shown_name, json_string_value(json_object_get(new_obj, "shown_name")));
        HASH_ADD_STR(psn->friends, name, new);
    }



    friend_arr = json_object_get(root, "friendrqsinc");
    for (i = 0; i < json_array_size(friend_arr); i++) {
        struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));
        json_t *new_obj = json_array_get(friend_arr, i);
        strcpy(new->name, json_string_value(json_object_get(new_obj, "name")));
        strcpy(new->shown_name, json_string_value(json_object_get(new_obj, "shown_name")));
        HASH_ADD_STR(psn->friend_requests_incoming, name, new);
    }

    friend_arr = json_object_get(root, "friendrqsout");
    for (i = 0; i < json_array_size(friend_arr); i++) {
        struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));
        json_t *new_obj = json_array_get(friend_arr, i);
        strcpy(new->name, json_string_value(json_object_get(new_obj, "name")));
        strcpy(new->shown_name, json_string_value(json_object_get(new_obj, "shown_name")));
        HASH_ADD_STR(psn->friend_requests_outgoing, name, new);
    }

    //DEBUG("deserializing finished\n%s","");
    return PSN_ERR_SUCCESS;
}

psn_err psn_free(struct psn_s *psn)
{
    if (psn == NULL) {
        DEBUG("no valid psn instance%s\n", "");
        return PSN_ERR_FAIL;
    }

    if (psn->mosq != NULL) {
        mosquitto_destroy(psn->mosq);
    }

    //clear userlsts
    struct user_s *s;
    for (s = psn->friends; s != NULL; s = s->hh.next) {
        HASH_DEL(psn->friends, s);
        free(s);
    }
    for (s = psn->friend_requests_incoming; s != NULL; s = s->hh.next) {
        HASH_DEL(psn->friend_requests_incoming, s);
        free(s);
    }
    for (s = psn->friend_requests_outgoing; s != NULL; s = s->hh.next) {
        HASH_DEL(psn->friend_requests_outgoing, s);
        free(s);
    }

    return PSN_ERR_SUCCESS;
}
