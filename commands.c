#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "commands.h"
#include "data_source.h"
#include "stats.h"

// Reutilizable: opciones comunes parseadas al inicio de cada subcomando
static const struct option COMMON_LONG[] = {
    { "n",      required_argument, 0, 'n' },
    { "seed",   required_argument, 0, 's' },
    { "verbose",no_argument,       0, 'v' },
    { "json",   no_argument,       0, 1000 },
    { "output", required_argument, 0, 'o' },
    { "help",   no_argument,       0, 'h' },
    { 0, 0, 0, 0 }
};

int cmd_rate_limit(int argc, char **argv, cli_globals_t *g) {
    double lambda = 20.0;
    int    limit  = 25;

    // Opciones específicas del subcomando
    static const struct option long_opts[] = {
        { "lambda", required_argument, 0, 'l' },
        { "limit",  required_argument, 0, 'L' },
        { "n",      required_argument, 0, 'n' },
        { "seed",   required_argument, 0, 's' },
        { "verbose",no_argument,       0, 'v' },
        { "json",   no_argument,       0, 1000 },
        { "output", required_argument, 0, 'o' },
        { "help",   no_argument,       0, 'h' },
        { 0, 0, 0, 0 }
    };

    optind = 1;  // reset por si se llama varias veces
    int c;
    while ((c = getopt_long(argc, argv, "l:L:n:s:vo:h", long_opts, NULL)) != -1) {
        switch (c) {
            case 'l': if (cli_parse_double(optarg, &lambda) < 0) return 1; break;
            case 'L': if (cli_parse_size(optarg, (size_t*)&limit) < 0) return 1; break;
            case 'n': if (cli_parse_size(optarg, &g->n) < 0) return 1; break;
            case 's': if (cli_parse_u64(optarg, &g->seed) < 0) return 1; break;
            case 'v': g->verbose = true; break;
            case 'o': g->output = optarg; break;
            case 1000: g->json = true; break;
            case 'h':
                printf("Uso: apistats rate-limit [flags]\n"
                       "  -l, --lambda <f>   Tasa Poisson (req/s) [default: 20]\n"
                       "  -L, --limit <i>    Límite del gateway    [default: 25]\n");
                return 0;
            default: return 1;
        }
    }

    // --- Simulación ---
    sample_t reqs = sim_poisson_requests(g->n, lambda);
    summary_t s   = summarize(&reqs);
    size_t rechazos = 0;
    for (size_t i = 0; i < g->n; i++)
        if (reqs.values[i] > limit) rechazos++;

    double p_exceder = (double)rechazos / g->n;

    if (g->json) {
        printf("{\"lambda\":%.4f,\"limit\":%d,\"n\":%zu,"
               "\"p_exceder\":%.6f,\"media\":%.4f,\"p95\":%.4f,\"p99\":%.4f}\n",
               lambda, limit, g->n, p_exceder, s.mean, s.p95, s.p99);
    } else {
        cli_print_header("Rate Limit Survival");
        printf("λ = %.2f req/s | límite = %d | n = %zu | seed = 0x%llX\n",
               lambda, limit, g->n, (unsigned long long)g->seed);
        cli_print_rule();
        printf("P(exceder límite) : %.4f  (%.2f%%)\n", p_exceder, p_exceder*100);
        printf("Rechazos estimados: %.2f por segundo\n", p_exceder * g->n / g->n * lambda);
        printf("Media llegadas    : %.4f\n", s.mean);
        printf("p95 / p99         : %.4f / %.4f\n", s.p95, s.p99);
        if (g->verbose) {
            printf("\nDesvío estándar   : %.4f\n", s.sd);
            printf("Mín / Máx         : %.0f / %.0f\n", s.min, s.max);
        }
    }

    if (g->output) {
        FILE *f = fopen(g->output, "w");
        if (!f) { perror("fopen"); sample_free(&reqs); return 1; }
        fprintf(f, "requests\n");
        for (size_t i = 0; i < g->n; i++) fprintf(f, "%.0f\n", reqs.values[i]);
        fclose(f);
        if (!g->json) printf("\n→ Exportado a %s\n", g->output);
    }

    sample_free(&reqs);
    return 0;
}
