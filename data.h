#ifndef DATA_H
#define DATA_H

void data_simulate(double *latencias, int *codigos, int n, double error_rate);
double data_generate_latency(void);
int data_generate_code(double error_rate);

#endif
