#include "psn.h"
#include "init.h"

int psn_init(struct psn_s *psn, char *config_file_path)
{
	int ret = 0;
	init_tomcrypt_lib();
	init_mosquitto_lib();

	char *username = "test1";
	
	psn->mosq = mosquitto_new(username, true, NULL);
	if(psn->mosq == NULL) {
		fprintf(stderr, "Could not create mosquitto instance.\n");
		ret = 1;
	}

	mosquitto_username_pw_set(psn->mosq, username, "lol");
	if(mosquitto_connect(psn->mosq, "localhost", 1337, 60)){
		fprintf(stderr, "Unable to connect.\n");
		ret = 2;
	}

	//init uthash
	psn->friends = NULL;
	psn->friend_requests_incoming = NULL;
	psn->friend_requests_outgoing = NULL;

	return ret;
}

int psn_create_new_identity(struct psn_s *psn, char *username)
{
	return 0;
}

void my_message_callback(struct mosquitto *mosq, void *userdata, 
						 const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", (char *) message->topic, (char *) message->payload);
	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

int psn_make_friend_req(struct psn_s *psn , char *target, char *message)
{
	return 0;
}

int psn_accept_friend_req(struct psn_s *psn, char *target)
{
	return 0;
}

int psn_refuse_friend_req(struct psn_s *psn, char *target)
{
	return 0;
}

int psn_del_friend(struct psn_s *psn, char *target)
{
	return 0;
}

int psn_send_message(struct psn_s *psn, char *target, char *message)
{
	return 0;
}

int psn_get_friend_list(struct psn_s *psn)
{
	return 0;
}

int psn_serialize_config(struct psn_s *psn, char *dest_str)
{
	json_t *root = json_object();
	struct user_s *s;

	json_object_set_new(root, "username", json_string(psn->username));
	json_object_set_new(root, "hostname", json_string(psn->hostname));
	json_object_set_new(root, "port", json_integer(psn->port));

	json_t *friend_list = json_array();
	for(s=psn->friends; s != NULL; s=s->hh.next) {
		json_t *new_obj = json_object();
		json_object_set_new(new_obj, "name",json_string(s->name));
		json_object_set_new(new_obj, "shown_name", json_string(s->shown_name));
    	json_array_append(friend_list, new_obj);
    }

    json_t *friend_req_i_list = json_array();
	for(s=psn->friend_requests_incoming; s != NULL; s=s->hh.next) {
		json_t *new_obj = json_object();
		json_object_set_new(new_obj, "name",json_string(s->name));
		json_object_set_new(new_obj, "shown_name", json_string(s->shown_name));
    	json_array_append(friend_req_i_list, new_obj);
    }

    json_t *friend_req_o_list = json_array();
	for(s=psn->friend_requests_outgoing; s != NULL; s=s->hh.next) {
		json_t *new_obj = json_object();
		json_object_set_new(new_obj, "name",json_string(s->name));
		json_object_set_new(new_obj, "shown_name", json_string(s->shown_name));
    	json_array_append(friend_req_o_list, new_obj);
	}

	json_object_set_new(root, "friends", friend_list);
	json_object_set_new(root, "friendrqsinc", friend_req_i_list);
	json_object_set_new(root, "friendrqsout", friend_req_o_list);

	char * json_result = json_dumps(root, 0);

	strcpy(dest_str, json_result);

	free(json_result);

	return 0;
}

int psn_deserialize_config(struct psn_s *psn, char *src_str)
{
	json_error_t error;
	json_t *friend_arr;
	int i;

	json_t *root = json_loads(src_str, 0, &error);

	strcpy(psn->username,json_string_value(json_object_get(root, "username")));
	strcpy(psn->hostname,json_string_value(json_object_get(root, "hostname")));
	psn->port = json_integer_value(json_object_get(root, "port"));

	friend_arr = json_object_get(root, "friends");
	for(i = 0; json_array_size(friend_arr); i++) {
		struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));
		json_t *new_obj = json_array_get(friend_arr, i);
		strcpy(new->name, json_string_value(json_object_get(new_obj, "name")));
		strcpy(new->shown_name, json_string_value(json_object_get(new_obj, "shown_name")));
		HASH_ADD_STR(psn->friends, name, new);
	}

	friend_arr = json_object_get(root, "friendrqsinc");
	for(i = 0; json_array_size(friend_arr); i++) {
		struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));
		json_t *new_obj = json_array_get(friend_arr, i);
		strcpy(new->name, json_string_value(json_object_get(new_obj, "name")));
		strcpy(new->shown_name, json_string_value(json_object_get(new_obj, "shown_name")));
		HASH_ADD_STR(psn->friend_requests_incoming, name, new);
	}

	friend_arr = json_object_get(root, "friendrqsout");
	for(i = 0; json_array_size(friend_arr); i++) {
		struct user_s *new = (struct user_s *) malloc(sizeof(struct user_s));
		json_t *new_obj = json_array_get(friend_arr, i);
		strcpy(new->name, json_string_value(json_object_get(new_obj, "name")));
		strcpy(new->shown_name, json_string_value(json_object_get(new_obj, "shown_name")));
		HASH_ADD_STR(psn->friend_requests_outgoing, name, new);
	}

	return 0;
}