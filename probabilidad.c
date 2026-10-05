// probabilidad.c
#include <stdio.h>
#include "probabilidad.h"

// a) Probabilidad clásica: P(A) = casos favorables / casos posibles
double prob_clasica(int favorables, int posibles) {
    if (posibles <= 0) return 0.0;
    if (favorables < 0 || favorables > posibles) return 0.0;
    return (double)favorables / posibles;
}

// b) Frecuencia relativa: fr(A) = veces que ocurrió A / total de experimentos
double frecuencia_relativa(int eventos, int experimentos) {
    if (experimentos <= 0) return 0.0;
    return (double)eventos / experimentos;
}

// c) Independencia: A y B son independientes si P(A∩B) = P(A)·P(B)
int son_independientes(double p_a, double p_b, double p_a_y_b, double tolerancia) {
    double esperado = p_a * p_b;
    double diff = p_a_y_b - esperado;
    if (diff < 0) diff = -diff;
    return diff < tolerancia;
}

void prob_print_clasica(int favorables, int posibles) {
    double p = prob_clasica(favorables, posibles);
    printf("Probabilidad clásica:\n");
    printf("  Casos favorables: %d\n", favorables);
    printf("  Casos posibles:   %d\n", posibles);
    printf("  P(A) = %d/%d = %.4f (%.2f%%)\n\n",
           favorables, posibles, p, p * 100);
}

void prob_print_frecuencia(int eventos, int experimentos) {
    double fr = frecuencia_relativa(eventos, experimentos);
    printf("Frecuencia relativa:\n");
    printf("  Eventos observados: %d\n", eventos);
    printf("  Experimentos:       %d\n", experimentos);
    printf("  fr(A) = %d/%d = %.4f (%.2f%%)\n\n",
           eventos, experimentos, fr, fr * 100);
}

void prob_print_independencia(double p_a, double p_b, double p_a_y_b) {
    double esperado = p_a * p_b;
    int indep = son_independientes(p_a, p_b, p_a_y_b, 0.01);
    printf("Independencia de eventos:\n");
    printf("  P(A)    = %.4f\n", p_a);
    printf("  P(B)    = %.4f\n", p_b);
    printf("  P(A∩B)  = %.4f\n", p_a_y_b);
    printf("  P(A)·P(B) = %.4f\n", esperado);
    printf("  → %s\n\n", indep ? "SON independientes" : "NO son independientes");
}
