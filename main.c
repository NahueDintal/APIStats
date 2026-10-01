#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "commands.h"

typedef struct {
    const char *name;
    int (*fn)(int, char**, cli_globals_t*);
    const char *desc;
} cmd_entry_t;

static const cmd_entry_t COMMANDS[] = {
    { "rate-limit", cmd_rate_limit, "Simula llegadas Poisson vs límite del gateway" },
    { "ab-test",    cmd_ab_test,    "Compara latencias de dos endpoints (test t)"   },
    { "retry",      cmd_retry,      "Reintentos hasta éxito (geométrica)"           },
    { "cache",      cmd_cache,      "Cadena de Markov hit/miss"                     },
    { "hist",       cmd_hist,       "Histograma ASCII de una muestra"               },
    { "chi2",       cmd_chi2,       "Test chi-cuadrado empírico vs teórico"         },
    { NULL, NULL, NULL }
};

static void usage(const char *prog) {
    printf("Uso: %s <subcomando> [flags]\n\n", prog);
    printf("Subcomandos:\n");
    for (const cmd_entry_t *c = COMMANDS; c->name; c++)
        printf("  %-12s %s\n", c->name, c->desc);
    printf("\nFlags globales:\n");
    printf("  -n, --n <int>         Tamaño de muestra (default: 10000)\n");
    printf("  -s, --seed <u64>      Semilla del RNG\n");
    printf("  -v, --verbose         Modo detallado\n");
    printf("      --json            Salida JSON\n");
    printf("  -o, --output <file>   Exportar a archivo\n");
    printf("  -h, --help            Esta ayuda\n");
    printf("\nEjemplos:\n");
    printf("  %s rate-limit -l 20 -L 25 -n 50000\n", prog);
    printf("  %s ab-test --mean-a 120 --mean-b 100 --dist e\n", prog);
    printf("  %s retry -p 0.3 -m 20\n", prog);
}

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
        return argc < 2 ? 1 : 0;
    }

    const char *sub = argv[1];
    for (const cmd_entry_t *c = COMMANDS; c->name; c++) {
        if (strcmp(c->name, sub) == 0) {
            cli_globals_t g;
            cli_init_globals(&g);
            return c->fn(argc - 1, argv + 1, &g);
        }
    }

    fprintf(stderr, "Error: subcomando desconocido '%s'\n\n", sub);
    usage(argv[0]);
    return 1;
}
