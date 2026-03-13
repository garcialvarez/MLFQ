#ifndef UTILS_H
#define UTILS_H

#include "../scheduler/scheduler.h"

// Imprime la tabla de metricas en consola
void print_metrics_table(Process *processes, int num_processes);

// Exporta los resultados a un archivo CSV
void export_results_csv(Process *processes, int num_processes, const char *filename);

#endif
