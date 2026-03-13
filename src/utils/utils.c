#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils/utils.h"

void print_metrics_table(Process *processes, int num_processes) {
    printf("\n==================================================================================\n");
    printf("                              TABLA DE METRICAS (PROCESOS)                        \n");
    printf("==================================================================================\n");
    printf("PID\tArrival\tBurst\tResponse\tTurnaround\tWaiting\n");
    printf("----------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < num_processes; i++) {
        int response = processes[i].first_response_time - processes[i].arrival_time;
        int turnaround = processes[i].finish_time - processes[i].arrival_time;
        int waiting = turnaround - processes[i].burst_time;
        
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", 
            processes[i].pid, 
            processes[i].arrival_time, 
            processes[i].burst_time, 
            response, 
            turnaround, 
            waiting);
    }
    printf("==================================================================================\n\n");
}

void export_results_csv(Process *processes, int num_processes, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error: No se pudo crear el archivo %s\n", filename);
        return;
    }
    
    // This is the header row for our CSV file
    fprintf(f, "PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting\n");
    
    for (int i = 0; i < num_processes; i++) {
        int response_time = processes[i].first_response_time - processes[i].arrival_time;
        int turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        int waiting_time = turnaround_time - processes[i].burst_time;
        
        fprintf(f, "P%d,%d,%d,%d,%d,%d,%d,%d\n",
            processes[i].pid,
            processes[i].arrival_time,
            processes[i].burst_time,
            processes[i].first_response_time, // We're using first_response_time for the "Start" metric here
            processes[i].finish_time,
            response_time,
            turnaround_time,
            waiting_time);
    }
    
    fclose(f);
    printf("Resultados guardados exitosamente en %s\n", filename);
}
