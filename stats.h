#ifndef STATS_H
#define STATS_H

typedef struct {
    int n;
    double mean;
    double median;
    double std_dev;
    double variance;
    double min, max;
    double p90, p95, p99;
    int success_count;
    int error_count;
    double p_success;
} Stats;

void stats_compute(Stats *s, double *data, int n, int *codes, int total_codes);
double percentile(double *sorted_data, int n, double p);
void stats_print(Stats *s);
void stats_histogram(double *data, int n, int bins);

double stats_mean(double *data, int n);
double stats_std_dev(double *data, int n, double mean);
void stats_sort(double *data, int n);

#endif
