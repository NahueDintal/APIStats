# APIStats
Trabajo practico para la materia Probabilidad y estadistica aplicada.

El proyecto consiste en simular request a una api, con id, latencia y respuesta del servicio.

Con la posibilidad de hacer diferentes calculos con esos datos, como percentiles, media y desviación, probabilidades de exito o error, y un histograma.

Y también un modo verboso para hacer todos.

Y luego exportar resultados de la muestra en .CSV o en .JSON

Ejemplo de salida con 1000 datos simulados.
```
./api_stats -n 1000 -v    
Estadísticas de API simulada
============================

Requests totales:  1000
Latencia mínima:   17.36 ms
Latencia máxima:   441.97 ms
Media:             112.65 ms
Mediana (p50):     97.54 ms
Desviación std:    60.15 ms
Varianza:          3617.97

Percentiles:
  p90:  190.04 ms
  p95:  229.15 ms
  p99:  331.22 ms

Códigos de estado:
  Exitosos (2xx):  934
  Errores (5xx):   33
  P(éxito):        0.9340

Histograma de latencias (10 bins):

     17.4 -    59.8 ms | ██████████████████ 146
     59.8 -   102.3 ms | ██████████████████████████████████████████████████ 396
    102.3 -   144.7 ms | ████████████████████████████ 222
    144.7 -   187.2 ms | ████████████████ 132
    187.2 -   229.7 ms | ██████ 54
    229.7 -   272.1 ms | ███ 28
    272.1 -   314.6 ms | █ 9
    314.6 -   357.0 ms |  6
    357.0 -   399.5 ms |  4
    399.5 -   442.0 ms |  3
```
Ejemplo del modo ayuda. --help
```
Uso: ./api_stats [OPCIONES]

Simulación:
  -n <int>        Número de requests (default: 1000)
  -s <int>        Semilla para reproducibilidad
  -e <float>      Tasa de error 0.0-1.0 (default: 0.05)

Salida:
  -o <archivo>    Exportar JSON
  --csv <arch>    Exportar CSV
  --raw           Mostrar datos en crudo

Análisis aislado:
  --percentiles   Solo percentiles
  --media         Solo media y desviación
  --prob          Solo probabilidades
  --hist [bins]   Solo histograma

Otros:
  -v              Modo verbose (todo)
  -h, --help      Mostrar ayuda
```
