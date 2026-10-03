#include "data.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


double data_generate_latency(void) {
  double u1 = (rand() + 1.0) / (RAND_MAX + 2.0);
  double u2 = (rand() + 1.0) / (RAND_MAX + 2.0);
  double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
  return 100.0 * exp(0.5 * z);
}

int data_generate_code(double error_rate) {
  double r = (double)rand() / RAND_MAX;
  if (r < error_rate) {
    return 500 + rand() % 100;
  }
  else if (r < error_rate + 0.03) {
    return 404;
  }
  else {
    return 200 + rand() % 100;
  }
}

void data_simulate(double *latencias, int *codigos, int n, double error_rate) {
  for (int i = 0; i < n; i++) {
    latencias[i] = data_generate_latency();
    codigos[i] = data_generate_code(error_rate);
  }
}
