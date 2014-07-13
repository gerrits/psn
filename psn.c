#include "psn.h"

static struct mosquitto *client = NULL;

int psn_init(struct psn_s *psn, char *config_file_path)
{
	char *username = "test1";

	mosquitto_lib_init();
	
	client = mosquitto_new(username, true, NULL);
	if(client == NULL) {
		fprintf(stderr, "Could not create mosquitto instance.\n");
		return 1;
	}

	if(mosquitto_connect(client, "localhost", 1337, 60)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}

	mosquitto_subscribe(psn->mosq, NULL, "/lol/c", 0);

	return 0;
}


void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", (char *) message->topic, (char *) message->payload);
	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}