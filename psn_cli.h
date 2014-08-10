#include "psn.h"

int psn_cli_load_file(struct psn_s *psn, const char *filename);
int psn_cli_save_file(struct psn_s *psn, const char *filename);
int psn_cli_print_friend_list(struct psn_s *psn);

int psn_cli_print_help();