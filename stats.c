#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int cmp_double(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}

void stats_sort(double *data, int n) {
    qsort(data, n, sizeof(double), cmp_double);
}

Percentile percentile(double *sorted_data, int n, double p) {
    Percentile r = {0};
    if (n == 0) return r;

    double rank = (p / 100.0) * (n - 1);
    r.lower = (int)rank;
    r.upper = r.lower + 1;
    r.frac  = rank - r.lower;
    r.v_lower = sorted_data[r.lower];

    if (r.upper >= n) {
        r.upper   = n - 1;
        r.v_upper = sorted_data[n - 1];
        r.frac    = 0.0;
        r.clamped = 1;
        r.value   = sorted_data[n - 1];
        return r;
    }

    r.v_upper = sorted_data[r.upper];
    r.value = r.v_lower + r.frac * (r.v_upper - r.v_lower);
    return r;
}

double stats_mean(double *data, int n) {
    if (n == 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

double stats_std_dev(double *data, int n, double mean) {
    if (n < 2) return 0.0;
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (n - 1));
}

void stats_compute(Stats *s, double *data, int n, int *codes, int total_codes) {
    s->n = n;
    s->min = data[0];
    s->max = data[0];

    for (int i = 0; i < n; i++) {
        if (data[i] < s->min) s->min = data[i];
        if (data[i] > s->max) s->max = data[i];
    }

    stats_sort(data, n);

    s->mean     = stats_mean(data, n);
    s->median   = percentile(data, n, 50);
    s->p90      = percentile(data, n, 90);
    s->p95      = percentile(data, n, 95);
    s->p99      = percentile(data, n, 99);
    s->std_dev  = stats_std_dev(data, n, s->mean);
    s->variance = s->std_dev * s->std_dev;

    s->success_count = 0;
    s->error_count = 0;
    for (int i = 0; i < total_codes; i++) {
        if (codes[i] >= 200 && codes[i] < 300) s->success_count++;
        if (codes[i] >= 500) s->error_count++;
    }
    s->p_success = (double)s->success_count / total_codes;
}

void stats_print(Stats *s) {
    printf("Estadísticas de API simulada\n");
    printf("============================\n\n");

    printf("Requests totales:  %d\n", s->n);
    printf("Latencia mínima:   %.2f ms\n", s->min);
    printf("Latencia máxima:   %.2f ms\n", s->max);
    printf("Media:             %.2f ms\n", s->mean);
    printf("Mediana (p50):     %.2f ms\n", s->median.value);
    printf("Desviación std:    %.2f ms\n", s->std_dev);
    printf("Varianza:          %.2f\n\n", s->variance);

    printf("Percentiles:\n");
    printf("  p90:  %.2f ms\n", s->p90.value);
    printf("  p95:  %.2f ms\n", s->p95.value);
    printf("  p99:  %.2f ms\n\n", s->p99.value);

    printf("Códigos de estado:\n");
    printf("  Exitosos (2xx):  %d\n", s->success_count);
    printf("  Errores (5xx):   %d\n", s->error_count);
    printf("  P(éxito):        %.4f\n\n", s->p_success);
}

void stats_histogram(double *data, int n, int bins) {
    if (n == 0 || bins <= 0) return;

    double min = data[0], max = data[0];
    for (int i = 0; i < n; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }

    double range = max - min;
    if (range == 0) range = 1;

    int *counts = calloc(bins, sizeof(int));
    int max_count = 0;

    for (int i = 0; i < n; i++) {
        int bin = (int)((data[i] - min) / range * bins);
        if (bin >= bins) bin = bins - 1;
        if (bin < 0) bin = 0;
        counts[bin]++;
        if (counts[bin] > max_count) max_count = counts[bin];
    }

    printf("\nHistograma de latencias (%d bins):\n\n", bins);
    for (int b = 0; b < bins; b++) {
        double bin_start = min + (range / bins) * b;
        double bin_end   = min + (range / bins) * (b + 1);

        printf("  %7.1f - %7.1f ms | ", bin_start, bin_end);

        int bar_len = (max_count > 0) ? (counts[b] * 50 / max_count) : 0;
        for (int j = 0; j < bar_len; j++) printf("█");
        printf(" %d\n", counts[b]);
    }

    printf("\n  Tendencia: ");
    stats_sparkline(counts, bins);
    printf("\n");

    free(counts);
}

/* ─────────── Visualización ─────────── */

void stats_pct_bar(double pct, int width) {
    if (width <= 0) width = 40;
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;

    int fill = (int)(pct / 100.0 * width + 0.5);
    if (fill > width) fill = width;

    for (int i = 0; i < fill; i++) printf("█");
    for (int i = fill; i < width; i++) printf("░");
}

void stats_sparkline(const int *values, int n) {
    static const char *bars[] = {"▁","▂","▃","▄","▅","▆","▇","█"};
    if (n <= 0) return;

    int max = values[0];
    for (int i = 1; i < n; i++)
        if (values[i] > max) max = values[i];
    if (max == 0) max = 1;

    for (int i = 0; i < n; i++) {
        int level = values[i] * 7 / max;
        if (level < 0) level = 0;
        if (level > 7) level = 7;
        printf("%s", bars[level]);
    }
}

void stats_boxplot(double min, double p50, double p90, double p95, double p99,
                   double max, int width) {
    if (width < 20) width = 20;
    double range = max - min;
    if (range <= 0) range = 1;

    int pos_min = 0;
    int pos_p50 = (int)((p50 - min) / range * (width - 1) + 0.5);
    int pos_p90 = (int)((p90 - min) / range * (width - 1) + 0.5);
    int pos_p95 = (int)((p95 - min) / range * (width - 1) + 0.5);
    int pos_p99 = (int)((p99 - min) / range * (width - 1) + 0.5);
    int pos_max = width - 1;

    printf("  Box plot (rango %.1f – %.1f ms)\n", min, max);
    printf("    ├ min   █ p50   ═ rango p50-p90   ╪ p90   ┼ p95   · p99   ┤ max\n\n");

    printf("  ");
    for (int i = 0; i < width; i++) {
        if      (i == pos_min) printf("├");
        else if (i == pos_p50) printf("█");
        else if (i == pos_p90) printf("╪");
        else if (i == pos_p95) printf("┼");
        else if (i == pos_p99) printf("·");
        else if (i == pos_max) printf("┤");
        else if (i > pos_p50 && i < pos_p90) printf("═");
        else printf("─");
    }
    printf("\n");

    printf("  %*.1f%*.1f%*.1f%*.1f%*.1f%*.1f\n",
           pos_min + 1, min,
           pos_p50 - pos_min, p50,
           pos_p90 - pos_p50, p90,
           pos_p95 - pos_p90, p95,
           pos_p99 - pos_p95, p99,
           pos_max - pos_p99, max);
}
