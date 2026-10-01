#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Ordenar con qsort (para percentiles y mediana)
void stats_sort(double *data, int n) {
    // qsort necesita una función comparadora
    int cmp(const void *a, const void *b) {
        double da = *(const double *)a;
        double db = *(const double *)b;
        return (da > db) - (da < db);
    }
    qsort(data, n, sizeof(double), cmp);
}

// Calcular percentil p (0-100) sobre datos YA ORDENADOS
double percentile(double *sorted_data, int n, double p) {
    if (n == 0) return 0.0;
    
    double rank = (p / 100.0) * (n - 1);  // posición interpolada
    int lower = (int)rank;
    int upper = lower + 1;
    double frac = rank - lower;
    
    if (upper >= n) return sorted_data[n - 1];
    return sorted_data[lower] + frac * (sorted_data[upper] - sorted_data[lower]);
}

// Media aritmética
double stats_mean(double *data, int n) {
    if (n == 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

// Desviación estándar muestral (divide por n-1)
double stats_std_dev(double *data, int n, double mean) {
    if (n < 2) return 0.0;
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (n - 1));
}

// Calcular TODO de una vez
void stats_compute(Stats *s, double *data, int n, int *codes, int total_codes) {
    s->n = n;
    s->min = data[0];
    s->max = data[0];
    
    // Encontrar min/max
    for (int i = 0; i < n; i++) {
        if (data[i] < s->min) s->min = data[i];
        if (data[i] > s->max) s->max = data[i];
    }
    
    // Ordenar para percentiles (modifica el array original!)
    stats_sort(data, n);
    
    s->mean = stats_mean(data, n);
    s->median = percentile(data, n, 50);
    s->p90 = percentile(data, n, 90);
    s->p95 = percentile(data, n, 95);
    s->p99 = percentile(data, n, 99);
    s->std_dev = stats_std_dev(data, n, s->mean);
    s->variance = s->std_dev * s->std_dev;
    
    // Contar éxitos y errores por código HTTP
    s->success_count = 0;
    s->error_count = 0;
    for (int i = 0; i < total_codes; i++) {
        if (codes[i] >= 200 && codes[i] < 300) s->success_count++;
        if (codes[i] >= 500) s->error_count++;
    }
    s->p_success = (double)s->success_count / total_codes;
}

// Imprimir bonito en consola
void stats_print(Stats *s) {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║       ESTADÍSTICAS DE API SIMULADA       ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Requests totales:     %15d   ║\n", s->n);
    printf("║  Latencia mínima:      %11.2f ms   ║\n", s->min);
    printf("║  Latencia máxima:      %11.2f ms   ║\n", s->max);
    printf("║  Media:                %11.2f ms   ║\n", s->mean);
    printf("║  Mediana (p50):        %11.2f ms   ║\n", s->median);
    printf("║  Desviación estándar:  %11.2f ms   ║\n", s->std_dev);
    printf("║  Varianza:             %11.2f      ║\n", s->variance);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Percentiles:                            ║\n");
    printf("║    p90:                %11.2f ms   ║\n", s->p90);
    printf("║    p95:                %11.2f ms   ║\n", s->p95);
    printf("║    p99:                %11.2f ms   ║\n", s->p99);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Códigos de estado:                      ║\n");
    printf("║    Exitosos (2xx):     %15d   ║\n", s->success_count);
    printf("║    Errores (5xx):      %15d   ║\n", s->error_count);
    printf("║    P(éxito):           %14.4f   ║\n", s->p_success);
    printf("╚══════════════════════════════════════════╝\n\n");
}

// Histograma ASCII para el modo -v
void stats_histogram(double *data, int n, int bins) {
    if (n == 0 || bins <= 0) return;
    
    double min = data[0], max = data[0];
    for (int i = 0; i < n; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }
    
    double range = max - min;
    if (range == 0) range = 1;  // evitar división por cero
    
    int *counts = calloc(bins, sizeof(int));
    int max_count = 0;
    
    // Contar en cada bin
    for (int i = 0; i < n; i++) {
        int bin = (int)((data[i] - min) / range * bins);
        if (bin >= bins) bin = bins - 1;
        if (bin < 0) bin = 0;
        counts[bin]++;
        if (counts[bin] > max_count) max_count = counts[bin];
    }
    
    printf("\n📊 Histograma de latencias (%d bins):\n\n", bins);
    for (int b = 0; b < bins; b++) {
        double bin_start = min + (range / bins) * b;
        double bin_end = min + (range / bins) * (b + 1);
        
        printf("  %7.1f - %7.1f ms | ", bin_start, bin_end);
        
        // Barra proporcional (máximo 50 caracteres)
        int bar_len = (max_count > 0) ? (counts[b] * 50 / max_count) : 0;
        for (int j = 0; j < bar_len; j++) printf("█");
        printf(" %d\n", counts[b]);
    }
    printf("\n");
    
    free(counts);
}
