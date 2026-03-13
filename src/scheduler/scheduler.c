#include <stdio.h>
#include <stdlib.h>
#include "../../include/scheduler/scheduler.h"

// Some local constants and our queue structure
#define MAX_QUEUE_SIZE 100

typedef struct {
    Process *processes[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
    int quantum;
} Queue;

Queue q0, q1, q2;
int current_time = 0;

void init_queue(Queue *q, int quantum) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
    q->quantum = quantum;
}

void enqueue(Queue *q, Process *p) {
    if (q->count == MAX_QUEUE_SIZE) {
        printf("Error: Cola llena\n");
        return;
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->processes[q->rear] = p;
    q->count++;
}

Process* dequeue(Queue *q) {
    if (q->count == 0) return NULL;
    Process *p = q->processes[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->count--;
    return p;
}

void run_scheduler(Process *processes, int num_processes) {
    init_queue(&q0, QUANTUM_Q0);
    init_queue(&q1, QUANTUM_Q1);
    init_queue(&q2, QUANTUM_Q2);

    int completed = 0;
    current_time = 0;
    
    Process *current_process = NULL;
    // At this point, time_in_queue is handled inside the Process struct, keeping it safe during preemptions.

    printf("\n=======================================================\n");
    printf("              INICIANDO SIMULACION MLFQ                \n");
    printf("=======================================================\n");

    while (completed < num_processes) {
        // Step 1: Check if any new processes just arrived
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == current_time) {
                processes[i].current_queue = 0; 
                enqueue(&q0, &processes[i]);
                printf("[Ciclo %3d] -> Llega Proceso P%d. Encolado en Q0.\n", current_time, processes[i].pid);
            }
        }

        // Step 2: Time for a priority boost? (Every 20 cycles we move everyone up to Q0)
        if (current_time > 0 && current_time % BOOST_PERIOD == 0) {
            printf("[Ciclo %3d] *** PRIORITY BOOST ***: Todos los procesos se mueven a Q0\n", current_time);
            
            if (current_process != NULL) {
                current_process->current_queue = 0;
                current_process->time_in_queue = 0;
                enqueue(&q0, current_process);
                current_process = NULL;
            }
            
            while (q1.count > 0) {
                Process *p = dequeue(&q1);
                p->current_queue = 0;
                p->time_in_queue = 0;
                enqueue(&q0, p);
            }
            while (q2.count > 0) {
                Process *p = dequeue(&q2);
                p->current_queue = 0;
                p->time_in_queue = 0;
                enqueue(&q0, p);
            }
        }

        // Step 3: Deal with preemption and pick the next process to run
        if (current_process != NULL) {
            int should_preempt = 0;
            if (current_process->current_queue == 1 && q0.count > 0) should_preempt = 1;
            if (current_process->current_queue == 2 && (q0.count > 0 || q1.count > 0)) should_preempt = 1;
            
            if (should_preempt) {
                printf("[Ciclo %3d] -> P%d INTERRUMPIDO por proceso de mayor prioridad. Retorna a Q%d (sin democion).\n", 
                    current_time, current_process->pid, current_process->current_queue);
                
                // Just putting it back in its current queue, no penalty here
                if (current_process->current_queue == 0) enqueue(&q0, current_process);
                else if (current_process->current_queue == 1) enqueue(&q1, current_process);
                else enqueue(&q2, current_process);
                
                current_process = NULL;
                // Note: We don't reset the quantum here, time_in_queue handles it naturally
            }
        }

        if (current_process == NULL) {
            if (q0.count > 0) current_process = dequeue(&q0);
            else if (q1.count > 0) current_process = dequeue(&q1);
            else if (q2.count > 0) current_process = dequeue(&q2);
        }

        // Step 4: Actually execute the chosen process for this cycle
        if (current_process != NULL) {
            if (!current_process->has_started) {
                current_process->has_started = 1;
                current_process->first_response_time = current_time;
                current_process->start_time = current_time;
            }

            printf("[Ciclo %3d] Ejecutando P%d en Q%d (Tiempo Restante: %d)\n", 
                current_time, current_process->pid, current_process->current_queue, current_process->remaining_time);

            current_process->remaining_time--;
            current_process->time_in_queue++;

            if (current_process->remaining_time == 0) {
                current_process->finish_time = current_time + 1; 
                printf("[Ciclo %3d] -> P%d COMPLETO su ejecucion.\n", current_time + 1, current_process->pid);
                completed++;
                current_process = NULL;
            } else {
                int current_q_quantum = (current_process->current_queue == 0) ? q0.quantum : 
                                        (current_process->current_queue == 1) ? q1.quantum : q2.quantum;

                if (current_process->time_in_queue >= current_q_quantum) {
                    // Time's up for this quantum! Let's demote it if possible.
                    if (current_process->current_queue < 2) {
                        current_process->current_queue++;
                        printf("[Ciclo %3d] -> P%d AGOTO quantum (Q%d). DEMOVIDO a Q%d.\n", 
                            current_time + 1, current_process->pid, current_process->current_queue - 1, current_process->current_queue);
                    } else {
                        printf("[Ciclo %3d] -> P%d AGOTO quantum (Q2). Se mantiene en Q2 (Round-Robin).\n", 
                            current_time + 1, current_process->pid);
                    }
                    
                    current_process->time_in_queue = 0;
                    
                    if (current_process->current_queue == 0) enqueue(&q0, current_process);
                    else if (current_process->current_queue == 1) enqueue(&q1, current_process);
                    else enqueue(&q2, current_process);
                    
                    current_process = NULL;
                }
            }
        } else {
            printf("[Ciclo %3d] CPU IDLE\n", current_time);
        }

        current_time++; // Tick the clock for the next cycle
    }
    
    printf("\n=======================================================\n");
    printf("         SIMULACION COMPLETADA EN %d CICLOS            \n", current_time);
    printf("=======================================================\n");
}
