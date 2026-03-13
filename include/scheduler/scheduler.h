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
    int start_time;       // When the process first gets CPU time
    int finish_time;
    int first_response_time;
    int current_queue;
    int has_started;      // Just a flag to check if the process has been picked up yet
    int time_in_queue;    // Keeps track of the quantum used, so it doesn't reset when preempted
} Process;

// Main process loop for our simulator
void run_scheduler(Process *processes, int num_processes);

#endif
