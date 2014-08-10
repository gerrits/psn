#ifndef _PSN_H_
#define _PSN_H_

#define DEBUG_ 0

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

#include "mosquitto.h"
#include "jansson.h"
#include "tomcrypt.h"
#include "uthash.h"

#include "init.h"

struct psn_s {
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
};

struct user_s {
    char name[32]; //key for hashmap
    char shown_name[32];

    rsa_key pub_key;

    UT_hash_handle hh;
};

typedef enum psn_err_e {
    PSN_ERR_SUCCESS,
    PSN_ERR_NOT_CONNECTED,
    PSN_ERR_COULD_NOT_CONNECT,
    PSN_ERR_ALREADY_CONNECTED,
    PSN_ERR_USER_NOT_IN_FRIEND_LIST,
    PSN_ERR_USER_NOT_IN_INCOMING_FRIEND_LIST,
    PSN_ERR_USER_NOT_IN_OUTGOING_FRIEND_LIST,
    PSN_ERR_FREQUEST_PENDING,
    PSN_ERR_FAIL
} psn_err;

psn_err psn_init(struct psn_s *psn);
psn_err psn_free(struct psn_s *psn);

psn_err psn_generate_new_key(struct psn_s *psn);

psn_err psn_connect(struct psn_s *psn);
psn_err psn_disconnect(struct psn_s *psn);

psn_err psn_set_username(struct psn_s *psn, char *username, char *password);
psn_err psn_set_shown_name(struct psn_s *psn, char *shown_name);
psn_err psn_set_server(struct psn_s *psn, char *hostname, int port);
int psn_get_friend_list(struct psn_s *psn, char ***friends);

psn_err psn_make_friend_req(struct psn_s *psn , char *target, char *message);
psn_err psn_accept_friend_req(struct psn_s *psn, char *target);
psn_err psn_refuse_friend_req(struct psn_s *psn, char *target);
psn_err psn_delete_friend(struct psn_s *psn, char *target);
psn_err psn_send_message(struct psn_s *psn, char *target, char *message);

//private :
psn_err psn_serialize_config(struct psn_s *psn, char *dest_str);
psn_err psn_deserialize_config(struct psn_s *psn, char *src_str);

//callbacks :
void psn_message_callback(struct mosquitto *mosq, void *userdata,
                          const struct mosquitto_message *message);

#define DEBUG(fmt, ...) \
    do { if (DEBUG_) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                     __LINE__, __func__, __VA_ARGS__); } while (0)

#endif /* _PSN_H_ */
