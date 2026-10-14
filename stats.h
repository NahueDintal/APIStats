#ifndef STATS_H
#define STATS_H

typedef struct {
    double value;    /* resultado final */
    int    lower;    /* índice lower */
    int    upper;    /* índice upper */
    double frac;     /* fracción de interpolación */
    double v_lower;  /* sorted_data[lower] */
    double v_upper;  /* sorted_data[upper] */
    int    clamped;  /* 1 si cayó en el caso upper >= n */
} Percentile;

typedef struct {
    int    n;
    double min, max;
    double mean;
    Percentile median, p90, p95, p99;
    double std_dev, variance;
    int    success_count, error_count;
    double p_success;
} Stats;

void stats_sort(double *data, int n);
Percentile percentile(double *sorted_data, int n, double p);
double stats_mean(double *data, int n);
double stats_std_dev(double *data, int n, double mean);
void stats_compute(Stats *s, double *data, int n, int *codes, int total_codes);
void stats_print(Stats *s);
void stats_histogram(double *data, int n, int bins);

#endif
