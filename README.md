# MLFQ CPU Scheduler Simulator

This is a fun little project that simulates a CPU process scheduler using the Multi-Level Feedback Queue (MLFQ) algorithm, written entirely in C. 

## What the scheduler does
This simulator acts like an operating system's brain when deciding which process gets to use the CPU. It handles incoming processes, assigns them to execution queues based on priority, and manages the active tasks on every simulated clock cycle.

## How the MLFQ works in this project
Our MLFQ is built around a few core rules to make it fair and snappy:

* **Queues & Priorities:** We have three queues: `Q0` (highest priority), `Q1`, and `Q2` (lowest priority). The scheduler always picks the next available process from the highest priority queue that isn't empty.
* **Quantums:** Each queue has its own time limit (quantum) before it kicks the process out. Q0 gives 2 cycles, Q1 gives 4, and Q2 gives 8.
* **Round Robin:** Once inside a queue, processes take turns using Round Robin.
* **Demotion:** If a process hogs the CPU and uses up its entire quantum in a single go, we demote it to the next queue down (except for Q2, which is the bottom tier).
* **Preemption:** If a high-priority process drops into Q0 while a lower-priority process is running, we immediately pause the current one and let the new arrival take over. The paused process gets to keep its remaining quantum for later!
* **Priority Boost:** To prevent long jobs from starving at the bottom, we throw everything back up to `Q0` every 20 clock cycles. It's like a free reset to keep things moving.

## How to compile
To get everything ready to go, just pop open your terminal in the project root and run:
```bash
make
```

## How to run
After compiling, you can run the whole simulation right away with this command:
```bash
make run
```
If you ever want to wipe the slate clean and clear out compiled files or old results, just hit it with:
```bash
make clean
```

## What the output file contains
When the simulation finishes, it dumps a neat little file called `results.csv`. 
This file breaks down exactly what happened, cycle by cycle, for every process we ran! 

Here is what you will find inside:
* **PID:** The actual process ID (e.g. P1, P2)
* **Arrival:** The exact cycle when the process showed up
* **Burst:** How many cycles the process needed to finish completely
* **Start:** The very first cycle when the process got to touch the CPU
* **Finish:** The cycle when the process finally completed
* **Response, Turnaround, Waiting:** The final metrics calculated for the process

## Small explanation of the metrics
To see how well our scheduler is doing, we calculate a few key metrics at the end:

* **Response Time:** How long a process had to wait from when it arrived to when it first got CPU time (`Start - Arrival`).
* **Turnaround Time:** The total time it took from arrival until the process completely finished (`Finish - Arrival`).
* **Waiting Time:** The total amount of time the process spent just sitting around in queue, not executing (`Turnaround - Burst`).
