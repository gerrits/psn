#include <stdlib.h>
#include <stdio.h>
#include <mosquitto.h>
#include "uthash/src/uthash.h"

//client struct
struct psn_s
{
	struct mosquitto* mosq;

	char *username;

	struct user_s *friends;
};

//configs
struct psn_config_s
{

};

//friend list entry
struct user_s
{
	int id; //key for hashmap
	char *name;

	UT_hash_handle *hh;
};

int psn_init(struct psn_s *psn, char *config_file_path);
int psn_make_friend_req(struct psn_s *psn , char *target, char *message);
int psn_accept_friend_req(struct psn_s *psn, char *target);
int psn_refuse_friend_req(struct psn_s *psn, char *target);
int psn_del_friend(struct psn_s *psn, char *target);
int psn_send_message(struct psn_s *psn, char *target, char *message);
int psn_get_friend_list(struct psn_s *psn);
//send pixx etc