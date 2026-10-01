#ifndef STATS_H
#define STATS_H

// Estructura con todos los resultados estadísticos
typedef struct {
    int n;              // cantidad de datos
    double mean;        // media aritmética
    double median;      // mediana (p50)
    double std_dev;     // desviación estándar (muestral)
    double variance;    // varianza
    double min, max;    // extremos
    double p90, p95, p99;  // percentiles clave
    int success_count;  // requests exitosos (código 2xx)
    int error_count;    // requests con error (código 5xx)
    double p_success;   // probabilidad empírica de éxito
} Stats;

// Funciones principales
void stats_compute(Stats *s, double *data, int n, int *codes, int total_codes);
double percentile(double *sorted_data, int n, double p);
void stats_print(Stats *s);
void stats_histogram(double *data, int n, int bins);

// Helpers
double stats_mean(double *data, int n);
double stats_std_dev(double *data, int n, double mean);
void stats_sort(double *data, int n);

#endif
