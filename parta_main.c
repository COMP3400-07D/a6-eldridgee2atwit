#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parta.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "fcfs") == 0) {
        // Need at least one burst
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int blen = argc - 2;
        int* bursts = malloc(sizeof(int) * blen);
        if (bursts == NULL) return 1;
        for (int i = 0; i < blen; ++i) bursts[i] = atoi(argv[2 + i]);

        printf("Using FCFS\n\n");
        for (int i = 0; i < blen; ++i) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, blen);
        int total = fcfs_run(procs, blen);

        double sum = 0.0;
        for (int i = 0; i < blen; ++i) sum += procs[i].wait;
        double avg = (blen > 0) ? (sum / (double)blen) : 0.0;

        printf("Average wait time: %.2f\n", avg);

        free(bursts);
        free(procs);
        (void)total;
        return 0;

    } else if (strcmp(argv[1], "rr") == 0) {
        // Need quantum + at least one burst
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int quantum = atoi(argv[2]);
        int blen = argc - 3;
        int* bursts = malloc(sizeof(int) * blen);
        if (bursts == NULL) return 1;
        for (int i = 0; i < blen; ++i) bursts[i] = atoi(argv[3 + i]);

    printf("Using RR(%d)\n\n", quantum);
        for (int i = 0; i < blen; ++i) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, blen);
        int total = rr_run(procs, blen, quantum);

        double sum = 0.0;
        for (int i = 0; i < blen; ++i) sum += procs[i].wait;
        double avg = (blen > 0) ? (sum / (double)blen) : 0.0;

        printf("Average wait time: %.2f\n", avg);

        free(bursts);
        free(procs);
        (void)total;
        return 0;
    }

    printf("ERROR: Missing arguments\n");
    return 1;
}
