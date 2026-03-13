#ifndef UTILS_H
#define UTILS_H

#include "../scheduler/scheduler.h"

// Prints the final metrics to the console so we can see how the processes did
void print_metrics_table(Process *processes, int num_processes);

// Dumps our results into a CSV file
void export_results_csv(Process *processes, int num_processes, const char *filename);

#endif
