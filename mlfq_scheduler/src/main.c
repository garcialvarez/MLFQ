#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler/scheduler.h"
#include "../include/utils/utils.h"

int main() {
    // Escenario de prueba hardcodeado
    // P1: arrival=0, burst=8
    // P2: arrival=1, burst=4
    // P3: arrival=2, burst=9
    // P4: arrival=3, burst=5
    // Format: {pid, arrival_time, burst_time, remaining_time, start_time, finish_time, first_response_time, current_queue, has_started}
    Process processes[4] = {
        {1, 0, 8, 8, 0, 0, 0, 0, 0},
        {2, 1, 4, 4, 0, 0, 0, 0, 0},
        {3, 2, 9, 9, 0, 0, 0, 0, 0},
        {4, 3, 5, 5, 0, 0, 0, 0, 0}
    };
    int num_processes = 4;

    // Ejecuta simulador iterando por el arreglo principal de procesos
    run_scheduler(processes, num_processes);

    // Muestra impresiones y genera el archivo CSV usando las funciones de utilidad
    print_metrics_table(processes, num_processes);
    export_results_csv(processes, num_processes, "results.csv");

    return 0;
}
