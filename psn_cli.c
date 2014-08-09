#include "psn_cli.h"

int main(int argc, const char* argv[])
{
	char line[2048];
	char *partition = NULL;
	char *delims = " \n";
	struct psn_s myPsn;
	int end = 0;
	
	while(!end) {
		printf("> ");

		gets(line);
		partition = strtok(line, delims);

		if(partition == NULL) {
			continue;
		}

		if(!strcmp(partition, "exit")) {
			printf("Exiting ...\n");
			end = 1;
		} else if(!strcmp(partition, "set")) {
			partition = strtok(NULL, delims);
			if(!strcmp(partition, "name")) {
				//set name here
			} else if(!strcmp(partition, "shown")) {
				//set shown name here
			} else if(!strcmp(partition, "server")) {
				//set server/pass here
			} else if(!strcmp(partition, "user")) {
				//char * username = strtok(NULL, delims);
				partition = strtok(NULL, delims);
				
				//----->
				//set settings for other users here (shown_name etc)
			}
		} else if(!strcmp(partition, "add")) {
			//add user here
			char *target = strtok(NULL, delims);
		    char *message = strtok(NULL, delims);
			psn_make_friend_req(&myPsn, target, message);
				
		} else if(!strcmp(partition, "accept")) {
			//accept friend request here
			char *target = strtok(NULL, delims);
			psn_accept_friend_req(&myPsn, target);
		} else if(!strcmp(partition, "refuse")) {
			//refuse friend request here
		} else if(!strcmp(partition, "delete")) {
			//delete friend here
		} else if(!strcmp(partition, "msg")) {
		    //write message to user here
		    char *target = strtok(NULL, delims);
		    char *message = strtok(NULL, "");
		    psn_send_message(&myPsn, target, message);	
		} else if(!strcmp(partition, "connect")) {
		    //connect to server here
		} else if(!strcmp(partition, "disconnect")) {
		    //disconnect from server here
		} else if(!strcmp(partition, "testenv")) {
			
			psn_init(&myPsn);
			psn_set_username(&myPsn,strtok(NULL, delims), "");
			psn_set_server(&myPsn, "localhost", 1335);

			psn_connect(&myPsn);
		}
	}
	return 0;
}