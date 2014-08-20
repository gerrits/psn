#include "psn_cli.h"

int main(int argc, const char *argv[])
{
    char line[2048];
    char *token = NULL;
    char *delims = " \n";
    struct psn_s myPsn;
    int end = 0;

    //init psn instance
    psn_init(&myPsn);

    if (argc >= 3) {
        if (!strcmp(argv[1], "-c")) {
            if (!psn_cli_load_file(&myPsn, argv[2])) {
                psn_connect(&myPsn);
            } else {
                printf("* ERROR: File not found\n");
            }
        }
    }

    while (!end) {
        printf("> ");

        gets(line);
        token = strtok(line, delims);

        if (token == NULL) {
            continue;
        }

        if (!strcmp(token, "exit")) {
            printf("Exiting ...\n");
            end = 1;
        } else if (!strcmp(token, "set")) {
            token = strtok(NULL, delims);
            if (token == NULL) {
                printf("*ERROR: missing argument\n");
                continue;
            }
            if (!strcmp(token, "name")) {
                //set name here
                char *username = strtok(NULL, delims);
                char *password = strtok(NULL, delims);

                if (username == NULL || password == NULL) {
                    printf("* ERROR: missing argument\n");
                    continue;
                }
                psn_set_username(&myPsn, username, password);
            } else if (!strcmp(token, "shown")) {
                //set shown name here
                char *shown_name = strtok(NULL, delims);
                if (shown_name == NULL) {
                    printf("* ERROR: missing argument\n");
                    continue;
                }
                psn_set_shown_name(&myPsn, shown_name);
            } else if (!strcmp(token, "server")) {
                //set server/pass here
                char *hostname = strtok(NULL, delims);
                char *port_str = strtok(NULL, delims);
                if (hostname == NULL || port_str == NULL) {
                    printf("* ERROR: missing argument\n");
                    continue;
                }
                int port = atoi(port_str);
                psn_set_server(&myPsn, hostname, port);
            } else if (!strcmp(token, "user")) {
                //char * username = strtok(NULL, delims);
                token = strtok(NULL, delims);

                //----->
                //set settings for other users here (shown_name etc)
            } else {
                printf("* ERROR: wrong argument\n");
            }
        } else if (!strcmp(token, "show")) {
            token = strtok(NULL, delims);
            if (token == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            if (!strcmp(token, "friends")) {
                psn_cli_print_friend_list(&myPsn);
            } else if (!strcmp(token, "network")) {
                printf("hostname: %s\n"
                       "port: %d\n", myPsn.hostname, myPsn.port);
            } else if (!strcmp(token, "user")) {
                printf("username: %s\n"
                       "shown name: %s\n", myPsn.username, myPsn.shown_name);
            } else {
                printf("* ERROR: wrong argument\n");
            }
        } else if (!strcmp(token, "add")) {
            //add user here
            char *target = strtok(NULL, delims);
            char *message = strtok(NULL, delims);
            if (target == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }

            if (message != NULL) {
                psn_make_friend_req(&myPsn, target, message);
            } else {
                psn_make_friend_req(&myPsn, target, "");
            }
        } else if (!strcmp(token, "accept")) {
            //accept friend request here
            char *target = strtok(NULL, delims);
            if (target == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_accept_friend_req(&myPsn, target);
        } else if (!strcmp(token, "refuse")) {
            //refuse friend request here
            char *target = strtok(NULL, delims);
            if (target == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_refuse_friend_req(&myPsn, target);
        } else if (!strcmp(token, "delete")) {
            //delete friend here
            char *target = strtok(NULL, delims);
            if (target == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_delete_friend(&myPsn, target);
        } else if (!strcmp(token, "msg")) {
            //write message to user here
            char *target_user = strtok(NULL, delims);
            char *message = strtok(NULL, "");
            if (target_user == NULL || message == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_send_message(&myPsn, target_user, message);
        } else if (!strcmp(token, "enc")) {
            char *target_user = strtok(NULL, delims);
            char *message = strtok(NULL, "");

            if (target_user == NULL || message == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_send_encrypted_message(&myPsn, target_user, message);

        } else if (!strcmp(token, "genkey")) {
            psn_generate_new_key(&myPsn);
        } else if (!strcmp(token, "connect")) {
            //connect to server here
            psn_connect(&myPsn);
        } else if (!strcmp(token, "disconnect")) {
            //disconnect from server here
            psn_disconnect(&myPsn);
        } else if (!strcmp(token, "load_file")) {
            char *filename = strtok(NULL, delims);
            if (filename == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_cli_load_file(&myPsn, filename);
        } else if (!strcmp(token, "save_file")) {
            char *filename = strtok(NULL, delims);
            if (filename == NULL) {
                printf("* ERROR: missing argument\n");
                continue;
            }
            psn_cli_save_file(&myPsn, filename);
        } else if (!strcmp(token, "me")) {
            printf("%s\n", myPsn.username);
        } else if (!strcmp(token, "testenv")) {
            psn_set_username(&myPsn, strtok(NULL, delims), "");
            psn_set_server(&myPsn, "localhost", 1335);
            psn_connect(&myPsn);
        } else if (!strcmp(token, "usage") || !strcmp(token, "help")) {
            psn_cli_print_help();
        } else {
            printf("%s: command not found type 'usage' for help\n", token);
        }
    }
    return 0;
}

int psn_cli_load_file(struct psn_s *psn, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return -1;
    }

    char config_string[2048];
    int len =   fread(config_string, 1, 2048, file);

    config_string[len] = '\0';
    psn_deserialize_config(psn, config_string);

    fclose(file);
    return 0;
}

int psn_cli_save_file(struct psn_s *psn, const char *filename)
{
    FILE *file = fopen(filename, "w");

    char config_string[2048];
    psn_serialize_config(psn, config_string);
    fwrite(config_string, 1, strlen(config_string), file);

    fclose(file);
    return 0;
}

int psn_cli_print_friend_list(struct psn_s *psn)
{
    char **friends;
    int len = psn_get_friend_list(psn, &friends);

    for (int i = 0; i < len; i++) {
        printf("* %s\n", friends[i]);
        free(friends[i]);
    }

    free(friends);
    return 0;
}

int psn_cli_print_help()
{
    printf("* Commands:\n"
           "* set [ name | shown | server ] <str>\n"
           "* show [ friends | user | network ]\n"
           "* add <username> <message>\n"
           "* delete <username>\n"
           "* accept <username>\n"
           "* refuse <username>\n"
           "* msg <username> <message>\n"
           "* connect\n"
           "* disconnect\n"
           "* save_file <filename>\n"
           "* load_file <filename>\n");
    return 0;
}
