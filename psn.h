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

//client config struct
struct psn_s
{
	struct mosquitto* mosq;

	//user settings
	char username[32];
	char shown_name[32];

	char password[32];
	
	//network settings
	char hostname[256];
	int port;

	//lists
	struct user_s *friends;
	struct user_s *friend_requests_outgoing;
	struct user_s *friend_requests_incoming;
	
	//keys
	rsa_key pk_key;
};

//friend list entry
struct user_s
{
	char name[32]; //key for hashmap
	char shown_name[32];

	rsa_key pub_key;

	UT_hash_handle hh;
};

typedef enum psn_err_e { 	 	
							PSN_ERR_USER_NOT_IN_FRIEND_LIST,
				 			PSN_ERR_NO_CONN,
				 			PSN_ERR_FREQUEST_PENDING,
				 	   } psn_err;

//public :

/* 
 * function psn_init
 * description:
 *  must be called before every other action with that instance
 * parameters:
 * 	struct psn_s *psn - pointer to the psn instance
 * returns:
 * 	int - 0 on success		
 */
int psn_init(struct psn_s *psn);

/* 
 * function psn_connect
 * description:
 *  connect to the psn broker
 * parameters:
 * 	struct psn_s *psn - pointer to the psn instance
 * returns:
 * 	int - 0 on success		
 */
int psn_connect(struct psn_s *psn);

/* 
 * function psn_disconnect
 * description:
 *  connect to the psn broker
 * parameters:
 * 	struct psn_s *psn - pointer to the psn instance
 * returns:
 * 	int - 0 on success		
 */
int psn_disconnect(struct psn_s *psn);

/* 
 * function psn_set_username
 * description:
 *  set the username and password for the login
 * parameters:
 * 	struct psn_s *psn - pointer to the psn instance
 *  char *username - string containing the username to be used
 *  char *password - string containing the password to be used
 * returns:
 * 	int - 0 on success			
 */
int psn_set_username(struct psn_s *psn, char *username, char* password);

/* 
 * function psn_set_shown_name
 * description:
 *  set the shown name
 * parameters:
 * 	struct psn_s *psn - pointer to the psn instance
 *  char *shown_name - string containing the new shown_name
 * returns:
 * 	int - 0 on success			
 */
int psn_set_shown_name(struct psn_s *psn, char *shown_name);

/* 
 * function psn_set_server
 * description:
 *  set the hostname and the port of the server to connect to
 * parameters:
 * 	struct psn_s *psn - pointer to the psn instance
 *  char *hostname - string containing the hostname/ip to be used
 *  int port - integer containing the port to be used
 * returns:
 * 	int - 0 on success			
 */
int psn_set_server(struct psn_s *psn, char* hostname, int port);
int psn_generate_new_key(struct psn_s *psn);
int psn_make_friend_req(struct psn_s *psn , char *target, char *message);
int psn_accept_friend_req(struct psn_s *psn, char *target);
int psn_refuse_friend_req(struct psn_s *psn, char *target);
int psn_delete_friend(struct psn_s *psn, char *target);
int psn_send_message(struct psn_s *psn, char *target, char *message);
int psn_get_friend_list(struct psn_s *psn, char ***friends);

//private :
int psn_serialize_config(struct psn_s *psn, char *dest_str);
int psn_deserialize_config(struct psn_s *psn, char *src_str);

//callbacks :
void psn_message_callback(struct mosquitto *mosq, void *userdata, 
						 const struct mosquitto_message *message);

#define DEBUG(fmt, ...) \
        do { if (DEBUG_) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#endif /* _PSN_H_ */