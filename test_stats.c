#include "stats.h"
#include <stdio.h>

int main() {
    // Datos de prueba: latencias simuladas
    double latencias[] = {120.5, 45.2, 890.1, 234.0, 67.8, 156.3, 89.0, 445.7};
    int codigos[] = {200, 200, 500, 200, 404, 200, 200, 503};
    
    Stats s;
    stats_compute(&s, latencias, 8, codigos, 8);
    stats_print(&s);
    
    // Probar histograma
    double mas_datos[] = {10, 20, 20, 30, 30, 30, 40, 50, 60, 70, 80, 90, 100};
    stats_histogram(mas_datos, 13, 5);
    
    return 0;
}
