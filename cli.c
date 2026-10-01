#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cli_init_globals(cli_globals_t *g) {
    g->n = 10000;
    g->seed = 0xC0FFEEULL;
    g->verbose = false;
    g->json = false;
    g->color = true;
    g->output = NULL;
}

void cli_print_header(const char *title) {
    printf("\n=== %s ===\n", title);
}

void cli_print_rule(void) {
    printf("----------------------------------------\n");
}

int cli_parse_size(const char *s, size_t *out) {
    char *end;
    unsigned long long v = strtoull(s, &end, 10);
    if (*end != '\0' || end == s) { fprintf(stderr, "Error: '%s' no es entero\n", s); return -1; }
    *out = (size_t)v;
    return 0;
}

int cli_parse_u64(const char *s, uint64_t *out) {
    char *end;
    // Acepta 0x... y decimal
    unsigned long long v = strtoull(s, &end, 0);
    if (*end != '\0' || end == s) { fprintf(stderr, "Error: '%s' no es u64\n", s); return -1; }
    *out = (uint64_t)v;
    return 0;
}

int cli_parse_double(const char *s, double *out) {
    char *end;
    double v = strtod(s, &end);
    if (*end != '\0' || end == s) { fprintf(stderr, "Error: '%s' no es número\n", s); return -1; }
    *out = v;
    return 0;
}
