#ifndef SCHEDULER_H
#define SCHEDULER_H

#define QUANTUM_Q0 2
#define QUANTUM_Q1 4
#define QUANTUM_Q2 8
#define BOOST_PERIOD 20

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;         // Time it first started executing (could be same as first_response_time)
    int finish_time;
    int first_response_time;
    int current_queue;
    int has_started;      // flag para saber si ya tuvo su primera respuesta
} Process;

// Function signatures
void run_scheduler(Process *processes, int num_processes);

#endif
