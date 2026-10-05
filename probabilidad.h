#ifndef PROBABILIDAD_H
#define PROBABILIDAD_H

double prob_clasica(int favorables, int posibles);
double frecuencia_relativa(int eventos, int experimentos);
int son_independientes(double p_a, double p_b, double p_a_y_b, double tolerancia);
void prob_print_clasica(int favorables, int posibles);
void prob_print_frecuencia(int eventos, int experimentos);
void prob_print_independencia(double p_a, double p_b, double p_a_y_b);

#endif
