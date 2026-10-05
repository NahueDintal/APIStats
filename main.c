#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stats.h"
#include "data.h"
#include "export.h"
#include "probabilidad.h"


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
    printf("  --perc   Solo percentiles\n");
    printf("  --med         Solo media y desviación\n");
    printf("  --prob          Solo probabilidades\n");
    printf("  --hist [bins]   Solo histograma\n\n");
    printf("Probabilidad:\n");
    printf("  --clas <fav> <pos>       P(A) = favorables/posibles\n");
    printf("  --frec <ev> <exp>     fr(A) = eventos/experimentos\n");
    printf("  --indep <pa> <pb> <pab>  ¿A y B independientes?\n");
    printf("  --ej                  Ejemplos de prueba de cada función\n\n");
    printf("Otros:\n");
    printf("  -v              Modo verbose (todo)\n");
    printf("  -h, --help      Mostrar ayuda\n");
    printf("  --version       Versión número.\n");

}

int main(int argc, char *argv[]) {
    int n = 100;
    unsigned int seed = time(NULL);
    double error_rate = 0.2;
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
        else if (strcmp(argv[i], "--perc") == 0) {
            show_percentiles = 1;
        }
        else if (strcmp(argv[i], "--med") == 0) {
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
            fprintf(stderr, "Versión: 1.0.4\n");
            return 0;
        }
        else if (strcmp(argv[i], "--clas") == 0 && i + 2 < argc) {
            int fav = atoi(argv[++i]);
            int pos = atoi(argv[++i]);
            prob_print_clasica(fav, pos);
            return 0;
        }
        else if (strcmp(argv[i], "--frec") == 0 && i + 2 < argc) {
            int ev = atoi(argv[++i]);
            int exp = atoi(argv[++i]);
            prob_print_frecuencia(ev, exp);
            return 0;
        }
        else if (strcmp(argv[i], "--indep") == 0 && i + 3 < argc) {
            double pa = atof(argv[++i]);
            double pb = atof(argv[++i]);
            double pab = atof(argv[++i]);
            prob_print_independencia(pa, pb, pab);
            return 0;
        }
        else if (strcmp(argv[i], "--ej") == 0) {
            // Ejemplos de prueba exigidos por la consigna (punto e)
            prob_print_clasica(3, 6);          // dado: P(sacar par) = 3/6 = 0.5
            prob_print_frecuencia(45, 100);    // 45 éxitos en 100 intentos = 0.45
            prob_print_independencia(0.5, 0.5, 0.25);  // monedas independientes
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
        double fr_exito = frecuencia_relativa(s.success_count, n);
        double fr_error = frecuencia_relativa(s.error_count, n);
        printf("Probabilidades empíricas (frecuencia relativa):\n");
        printf("  P(éxito 2xx): %.4f (%d/%d)\n", fr_exito, s.success_count, n);
        printf("  P(error 5xx): %.4f (%d/%d)\n\n", fr_error, s.error_count, n);
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
