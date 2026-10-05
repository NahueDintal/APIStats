#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stats.h"
#include "data.h"
#include "export.h"

void print_help(void) {
    printf("Uso: apist [OPCIONES]\n\n");
    printf("Simulación:\n");
    printf("  -n <int>        Número de requests (default: 1000)\n");
    printf("  -e <float>      Tasa de error 0.0-1.0 (default: 0.05)\n\n");
    printf("Salida:\n");
    printf("  -o <archivo>    Exportar JSON\n");
    printf("  --csv <arch>    Exportar CSV\n");
    printf("  --raw           Mostrar datos en crudo\n\n");
    printf("Análisis aislado:\n");
    printf("  --percentiles   Solo percentiles\n");
    printf("  --media         Solo media y desviación\n");
    printf("  --prob          Solo probabilidades\n");
    printf("  --hist [bins]   Solo histograma\n\n");
    printf("Otros:\n");
    printf("  -v              Modo verbose (todo)\n");
    printf("  -h, --help      Mostrar ayuda\n");
    printf("  --version       Versión número.\n");
}

int main(int argc, char *argv[]) {
    int n = 0;
    unsigned int seed = time(NULL);
    double error_rate = 0;
    char *json_out = NULL;
    char *csv_out = NULL;
    int show_raw = 0, show_percentiles = 0, show_media = 0;
    int show_prob = 0, show_hist = 0, verbose = 0;
    int hist_bins = 10;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = (unsigned int)atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            error_rate = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            json_out = argv[++i];
        }
        else if (strcmp(argv[i], "--csv") == 0 && i + 1 < argc) {
            csv_out = argv[++i];
        }
        else if (strcmp(argv[i], "--raw") == 0) {
            show_raw = 1;
        }
        else if (strcmp(argv[i], "--percentiles") == 0) {
            show_percentiles = 1;
        }
        else if (strcmp(argv[i], "--media") == 0) {
            show_media = 1;
        }
        else if (strcmp(argv[i], "--prob") == 0) {
            show_prob = 1;
        }
        else if (strcmp(argv[i], "--hist") == 0) {
            show_hist = 1;
            if (i + 1 < argc && argv[i+1][0] != '-') {
                hist_bins = atoi(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        }
        else if (strcmp(argv[i], "--version") == 0) {
            fprintf(stderr, "Versión: 1.0.0\n");
            return 0;
        }
        else {
            fprintf(stderr, "Opción desconocida: %s.\n", argv[i]);
            fprintf(stderr, "Usa -h para ayuda. \n");
            return 1;
        }
    }
    
    if (!show_raw && !show_percentiles && !show_media && !show_prob && !show_hist && !json_out && !csv_out) {
      fprintf(stderr, "Usa -h ó --help para ayuda.\n");
    }
    
    srand(seed);
    double *latencias = malloc(n * sizeof(double));
    int *codigos = malloc(n * sizeof(int));
    
    data_simulate(latencias, codigos, n, error_rate);
    
    if (show_raw) {
        printf("# Datos simulados (semilla=%u, n=%d)\n", seed, n);
        printf("id,latencia(milisegundos),codigo.\n");
        for (int i = 0; i < n; i++) {
            printf("%d,%.2f,%d\n", i+1, latencias[i], codigos[i]);
        }
        printf("\n");
    }
    
    Stats s;
    stats_compute(&s, latencias, n, codigos, n);
    
    if (verbose) {
        stats_print(&s);
        stats_histogram(latencias, n, hist_bins);
    }
    if (show_percentiles) {
        printf("Percentiles:\n");
        printf("  p50: %.2f ms\n", s.median);
        printf("  p90: %.2f ms\n", s.p90);
        printf("  p95: %.2f ms\n", s.p95);
        printf("  p99: %.2f ms\n\n", s.p99);
    }
    if (show_media) {
        printf("Media: %.2f ms\n", s.mean);
        printf("Desviación estándar: %.2f ms\n", s.std_dev);
        printf("Varianza: %.2f\n\n", s.variance);
    }
    if (show_prob) {
        printf("Probabilidades:\n");
        printf("  P(éxito 2xx): %.4f (%d/%d)\n", s.p_success, s.success_count, n);
        printf("  P(error 5xx): %.4f (%d/%d)\n\n", 
               (double)s.error_count/n, s.error_count, n);
    }
    if (show_hist) {
        stats_histogram(latencias, n, hist_bins);
    }
    
    if (json_out) {
        export_json(json_out, &s, latencias, codigos, n);
        printf("JSON exportado a: %s\n", json_out);
    }
    if (csv_out) {
        export_csv(csv_out, latencias, codigos, n);
        printf("CSV exportado a: %s\n", csv_out);
    }
    
    free(latencias);
    free(codigos);
    return 0;
}
