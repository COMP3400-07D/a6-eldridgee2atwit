#include "parta.h"
#include <stdlib.h>
#include <stdio.h>



/**
 * Initializes an array of PCBs based on the given CPU bursts.
 *  @param bursts An array of CPU bursts for each process
 *  @param blen The length of the bursts array
 *  @return A pointer to an array of PCBs initialized with the given bursts
 */
struct pcb* init_procs(int* bursts, int blen) {
    // if no processes requested, return NULL
    if (blen <= 0) {
        return NULL;
    }

    // Allocate an array of PCBs on the heap
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (procs == NULL) {
        return NULL; // malloc failed
    }

    // Initialize each PCB: pid = index, burst_left from bursts[], wait = 0
    for (int i = 0; i < blen; ++i) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }

    return procs;
}

/**
 * Prints the current state of all processes.
 *  @param procs An array of PCBs
 * @param plen The length of the procs array
 */
void printall(struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) return;
    for (int i = 0; i < plen; ++i) {
        printf("P%d: burst=%d wait=%d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * Runs the current process for a given amount of time, updating burst_left and wait times.
    *  @param procs An array of PCBs
    * @param plen The length of the procs array
    * @param current The index of the current process to run
    * @param amount The amount of time to run the current process
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (procs == NULL || plen <= 0) {
        return;
    }
    if (current < 0 || current >= plen) {
        return;
    }

    // If the current process has no burst left, nothing happens
    if (procs[current].burst_left <= 0 || amount <= 0) {
        return;
    }

    // The actual time run is limited by the remaining burst
    int actual = amount;
    if (procs[current].burst_left < actual) {
        actual = procs[current].burst_left;
    }

    // Reduce the current process's burst
    procs[current].burst_left -= actual;

    // Increase wait for all other processes that are still not finished
    for (int i = 0; i < plen; ++i) {
        if (i == current) continue;
        if (procs[i].burst_left > 0) {
            procs[i].wait += actual;
        }
    }
}




/**
 * Runs the processes using First-Come-First-Serve scheduling.
 *  @param procs An array of PCBs
 *  @param plen The length of the procs array
 *  @return The total elapsed time after all processes have completed
 */
int fcfs_run(struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) return 0;

    int elapsed = 0;

    for (int i = 0; i < plen; ++i) {
        if (procs[i].burst_left <= 0) {
            // already finished, leave wait as-is (assumed 0)
            continue;
        }

        // The process waits for all previously executed bursts
        procs[i].wait = elapsed;

        // Run the process to completion
        elapsed += procs[i].burst_left;
        procs[i].burst_left = 0;
    }

    return elapsed;
}



/**
 * Finds the next process to run in Round-Robin scheduling.
 *  @param current The index of the current process
 *  @param procs An array of PCBs
 *  @param plen The length of the procs array
 *  @return The index of the next process to run, or -1 if none are runnable
 */
int rr_next(int current, struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) return -1;
    if (current < 0 || current >= plen) return -1;

    // Search for the next runnable process after `current`, wrapping around.
    for (int offset = 1; offset < plen; ++offset) {
        int idx = (current + offset) % plen;
        if (procs[idx].burst_left > 0) {
            return idx;
        }
    }

    // No other runnable processes found. If current is runnable, return it.
    if (procs[current].burst_left > 0) return current;

    // All done
    return -1;
}


/**
 * Runs the processes using Round-Robin scheduling.
 *  @param procs An array of PCBs
 *  @param plen The length of the procs array
 *  @param quantum The time quantum for each process
 *  @return The total elapsed time after all processes have completed
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    if (procs == NULL || plen <= 0 || quantum <= 0) return 0;

    int elapsed = 0;

    // Start with process 0 if it's runnable, otherwise find the next runnable
    int current = 0;
    if (procs[current].burst_left <= 0) {
        current = rr_next(current, procs, plen);
    }

    while (current != -1) {
        if (procs[current].burst_left <= 0) {
            // shouldn't happen, but find next
            current = rr_next(current, procs, plen);
            continue;
        }

        int amount = quantum;
        if (procs[current].burst_left < amount) amount = procs[current].burst_left;

        run_proc(procs, plen, current, amount);
        elapsed += amount;

        // Find next process according to RR policy
        current = rr_next(current, procs, plen);
    }

    return elapsed;
}

