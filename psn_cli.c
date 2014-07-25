#include "psn_cli.h"

int main()
{
	init_tomcrypt_lib();
	init_mosquitto_lib();

	//// RSA TEST ////
	int err;
	rsa_key key_;
	if ((err = rsa_make_key(0, 
							find_prng("sprng"), 
							1024/8, 65537, &key_))
	    != CRYPT_OK) {
		printf("rsa_make_key failed: %s\n", error_to_string(err));
	}
	unsigned char buffer  [1024];
	unsigned char buffer2 [1024];
	
	unsigned long outlen = sizeof(buffer);
	rsa_export(buffer, &outlen, PK_PUBLIC, &key_);

	rsa_key key2_;
	rsa_import(buffer, outlen, &key2_);

	unsigned long outlen2 = sizeof(buffer2);
	rsa_export(buffer2,&outlen2, PK_PUBLIC, &key2_);


	if(strncmp((char *) buffer, (char *) buffer2, 1024) == 0) {
		printf("%s\n", "Keys Are Equal");
	}
	
	//printf("Printing Buffer: \n");
	// for (int i = 0; i < outlen; i++)
	// {
	// 	printf("%x" ,buffer[i]);
	// }

	/// SERDES TEST ///
	struct psn_s *psn = (struct psn_s *) malloc(sizeof(struct psn_s));

	psn_init(psn, NULL);

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


	//PRINT STR
	char *json_str = malloc(2048 * sizeof(char));
	psn_serialize_config(psn, json_str);
	printf("%s\n", json_str);

	printf("Adding one more.\n");
	new_user = malloc(sizeof(struct user_s));
	strcpy(new_user->name, "Uffke2");
	strcpy(new_user->shown_name, "schnuffi2");
	HASH_ADD_STR(psn->friend_requests_incoming, name, new_user);

	//PRINT STR
	json_str = malloc(2048 * sizeof(char));
	psn_serialize_config(psn, json_str);
	printf("%s\n", json_str);

	return 0;

}
