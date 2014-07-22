#include "psn_cli.h"

int main()
{
	struct psn_s *psn = (struct psn_s *) malloc(sizeof(struct psn_s));

	strcpy(psn->username, "lol");
	strcpy(psn->hostname, "LAL_HOSD");
	psn->port = 1337;

	struct user_s *new_user = malloc(sizeof(struct user_s));
	strcpy(new_user->name, "Uffke");
	strcpy(new_user->shown_name, "schnuffi");

	HASH_ADD_STR(psn->friends, name, new_user);

	new_user = malloc(sizeof(struct user_s));
	strcpy(new_user->name, "Uffke1");
	strcpy(new_user->shown_name, "schnuffi1");

	HASH_ADD_STR(psn->friend_requests_outgoing, name, new_user);

	new_user = malloc(sizeof(struct user_s));
	strcpy(new_user->name, "Uffke2");
	strcpy(new_user->shown_name, "schnuffi2");

	HASH_ADD_STR(psn->friend_requests_incoming, name, new_user);
	char *json_str = malloc(2048 * sizeof(char));
	psn_serialize_config(psn, json_str);
	printf("%s\n", json_str);
	return 0;
}