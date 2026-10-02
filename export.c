#include "export.h"
#include <stdio.h>
#include <stdlib.h>

void export_json(const char *filename, Stats *s, double *latencias, int *codigos, int n) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: no se pudo crear %s\n", filename);
        return;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"api_stats\": {\n");
    fprintf(f, "    \"n_requests\": %d,\n", s->n);
    fprintf(f, "    \"latencia\": {\n");
    fprintf(f, "      \"min_ms\": %.2f,\n", s->min);
    fprintf(f, "      \"max_ms\": %.2f,\n", s->max);
    fprintf(f, "      \"mean_ms\": %.2f,\n", s->mean);
    fprintf(f, "      \"median_ms\": %.2f,\n", s->median);
    fprintf(f, "      \"std_dev_ms\": %.2f,\n", s->std_dev);
    fprintf(f, "      \"variance\": %.2f,\n", s->variance);
    fprintf(f, "      \"percentiles\": {\n");
    fprintf(f, "        \"p90_ms\": %.2f,\n", s->p90);
    fprintf(f, "        \"p95_ms\": %.2f,\n", s->p95);
    fprintf(f, "        \"p99_ms\": %.2f\n", s->p99);
    fprintf(f, "      }\n");
    fprintf(f, "    },\n");
    fprintf(f, "    \"codigos_estado\": {\n");
    fprintf(f, "      \"exitosos_2xx\": %d,\n", s->success_count);
    fprintf(f, "      \"errores_5xx\": %d,\n", s->error_count);
    fprintf(f, "      \"prob_exito\": %.4f\n", s->p_success);
    fprintf(f, "    },\n");
    
    fprintf(f, "    \"datos_muestra\": [\n");
    int muestra = (n < 100) ? n : 100;
    for (int i = 0; i < muestra; i++) {
        fprintf(f, "      {\"id\": %d, \"latencia_ms\": %.2f, \"codigo\": %d}%s\n",
                i+1, latencias[i], codigos[i], (i < muestra-1) ? "," : "");
    }
    fprintf(f, "    ]\n");
    fprintf(f, "  }\n");
    fprintf(f, "}\n");
    
    fclose(f);
}

void export_csv(const char *filename, double *latencias, int *codigos, int n) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: no se pudo crear %s\n", filename);
        return;
    }
    
    fprintf(f, "id,latencia_ms,codigo\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%.2f,%d\n", i+1, latencias[i], codigos[i]);
    }
    
    fclose(f);
}
