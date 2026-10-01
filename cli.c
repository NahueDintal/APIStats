#ifndef CLI_H
#define CLI_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    size_t   n;
    uint64_t seed;
    bool     verbose;
    bool     json;
    bool     color;
    const char *output;
} cli_globals_t;

void cli_init_globals(cli_globals_t *g);
void cli_print_header(const char *title);
void cli_print_rule(void);
int  cli_parse_size (const char *s, size_t  *out);
int  cli_parse_u64  (const char *s, uint64_t *out);
int  cli_parse_double(const char *s, double *out);

#endif
