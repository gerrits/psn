#ifndef _PSN_H_
#define _PSN_H_

#include <stdlib.h>
#include <stdio.h>
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
	
	//network settings
	char hostname[256];
	int port;

	//lists
	struct user_s *friends;
	struct user_s *friend_requests_outgoing;
	struct user_s *friend_requests_incoming;
	
	//keys
	rsa_key priv_key;
	rsa_key pub_key;
};

//friend list entry
struct user_s
{
	char name[32]; //key for hashmap
	char shown_name[32];

	rsa_key pub_key;

	UT_hash_handle hh;
};

//friend list entry
/*struct message_s
{
	int type;

	struct user_s *dest;
	struct user_s *src;
	
	void *content;
	
	UT_hash_handle hh;
};*/

//public
int psn_init(struct psn_s *psn, char *config_file_path);
int psn_create_new_identity(struct psn_s *psn, char *username);

int psn_make_friend_req(struct psn_s *psn , char *target, char *message);
int psn_accept_friend_req(struct psn_s *psn, char *target);
int psn_refuse_friend_req(struct psn_s *psn, char *target);
int psn_del_friend(struct psn_s *psn, char *target);
int psn_send_message(struct psn_s *psn, char *target, char *message);
int psn_get_friend_list(struct psn_s *psn);

//private:
int psn_serialize_config(struct psn_s *psn, char *dest_str);
int psn_deserialize_config(struct psn_s *psn, char *src_str);

//send pixx etc
#endif /* _PSN_H_ */