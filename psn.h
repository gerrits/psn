#ifndef _PSN_H_
#define _PSN_H_

#define DEBUG_ 0

#include <stdlib.h>
#include <stdio.h>

#include "mosquitto.h"
#include "jansson.h"
#include "tomcrypt.h"
#include "uthash.h"

#include "init.h"

typedef enum psn_message_type_e {
    ENCRYPTED
} psn_message_type;

typedef enum psn_err_e {
    PSN_ERR_SUCCESS,
    PSN_ERR_NOT_CONNECTED,
    PSN_ERR_COULD_NOT_CONNECT,
    PSN_ERR_ALREADY_CONNECTED,
    PSN_ERR_USER_NOT_IN_FRIEND_LIST,
    PSN_ERR_USER_NOT_IN_INCOMING_FRIEND_LIST,
    PSN_ERR_USER_NOT_IN_OUTGOING_FRIEND_LIST,
    PSN_ERR_FREQUEST_PENDING,
    PSN_ERR_FAIL,
    PSN_ERR_CRYPT_FAIL
} psn_err;

typedef struct psn_s {
    struct mosquitto *mosq;

    char username[32];
    char shown_name[32];

    char password[32];

    char hostname[256];
    int port;

    struct user_s *friends;
    struct user_s *friend_requests_outgoing;
    struct user_s *friend_requests_incoming;

    rsa_key pk_key;
} psn_t;

typedef struct user_s {
    char name[32]; //key for hashmap
    char shown_name[32];

    rsa_key pub_key;

    UT_hash_handle hh;
} user_t;

typedef struct message_s {
    psn_message_type type;
    char *encrypted_key;
    unsigned char *payload;
} message_t;

psn_err psn_init(psn_t *psn);
psn_err psn_free(psn_t *psn);

psn_err psn_generate_new_key(psn_t *psn);

psn_err psn_connect(psn_t *psn);
psn_err psn_disconnect(psn_t *psn);

psn_err psn_set_username(psn_t *psn, char *username, char *password);
psn_err psn_set_shown_name(psn_t *psn, char *shown_name);
psn_err psn_set_server(psn_t *psn, char *hostname, int port);
int psn_get_friend_list(psn_t *psn, char ***friends);

psn_err psn_make_friend_req(psn_t *psn , char *target, char *message);
psn_err psn_accept_friend_req(psn_t *psn, char *target);
psn_err psn_refuse_friend_req(psn_t *psn, char *target);
psn_err psn_delete_friend(psn_t *psn, char *target);
psn_err psn_send_message(psn_t *psn, char *target, char *message);

//private :
psn_err psn_serialize_config(psn_t *psn, char *dest_str);
psn_err psn_deserialize_config(psn_t *psn, char *src_str);


//callbacks :
void psn_message_callback(struct mosquitto *mosq, void *userdata,
                          const struct mosquitto_message *message);

//testing:
psn_err psn_send_encrypted_message(psn_t *psn, char *target,
                                   char *plain_message);

#define CONVERT_HEX(stream, string) \
    for (size_t i = 0; i < sizeof(string); ++i) \
        stream += sprintf(stream, "%02x", string[i]);

#define DEBUG(fmt, ...) \
    do { if (DEBUG_) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                     __LINE__, __func__, __VA_ARGS__); \
    } while (0)

#endif /* _PSN_H_ */
