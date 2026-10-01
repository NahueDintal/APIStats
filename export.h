#ifndef EXPORT_H
#define EXPORT_H
#include "stats.h"

void export_json(const char *filename, Stats *s, double *latencias, int *codigos, int n);
void export_csv(const char *filename, double *latencias, int *codigos, int n);

#endif
