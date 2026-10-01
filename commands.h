#ifndef COMMANDS_H
#define COMMANDS_H
#include "cli.h"

int cmd_rate_limit(int argc, char **argv, cli_globals_t *g);
int cmd_ab_test   (int argc, char **argv, cli_globals_t *g);
int cmd_retry     (int argc, char **argv, cli_globals_t *g);
int cmd_cache     (int argc, char **argv, cli_globals_t *g);
int cmd_hist      (int argc, char **argv, cli_globals_t *g);
int cmd_chi2      (int argc, char **argv, cli_globals_t *g);

#endif
