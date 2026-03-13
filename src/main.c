#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler/scheduler.h"
#include "../include/utils/utils.h"

int main() {
    // We're doing a simple hardcoded test scenario here
    // P1: arrives at 0, needs 8 cycles
    // P2: arrives at 1, needs 4 cycles
    // P3: arrives at 2, needs 9 cycles
    // P4: arrives at 3, needs 5 cycles
    // Struct format: {pid, arrival_time, burst_time, remaining_time, start_time, finish_time, first_response_time, current_queue, has_started, time_in_queue}
    Process processes[3] = {
        {1, 0, 50, 50, 0, 0, 0, 0, 0, 0},
        {2, 1, 3, 3, 0, 0, 0, 0, 0, 0},
        {3, 2, 4, 4, 0, 0, 0, 0, 0, 0}
    };
    int num_processes = 3;

    // Let's kick off the simulator with our process array
    run_scheduler(processes, num_processes);

    // Print out the final stats and save everything to our CSV file
    print_metrics_table(processes, num_processes);
    export_results_csv(processes, num_processes, "results.csv");

    return 0;
}
