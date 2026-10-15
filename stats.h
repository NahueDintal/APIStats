#ifndef STATS_H
#define STATS_H

typedef struct {
    double value;
    int    lower;
    int    upper;
    double frac;
    double v_lower;
    double v_upper;
    int    clamped;
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

/* Visualización */
void stats_pct_bar(double pct, int width);
void stats_sparkline(const int *values, int n);
void stats_boxplot(double min, double p50, double p90, double p95, double p99,
                   double max, int width);

#endif
