#include <stdio.h>
#include "parta.h"

int main() {
    struct pcb* procs = init_procs((int[]){5,8}, 2);
    int plen = 2;
    int quantum = 4;
    int elapsed = 0;
    int current = rr_next(0, procs, plen);
    printf("Start: P0=%d P1=%d\n", procs[0].burst_left, procs[1].burst_left);
    while (current != -1) {
        int amount = quantum;
        if (procs[current].burst_left < amount) amount = procs[current].burst_left;
        printf("Running P%d for %d\n", current, amount);
        run_proc(procs, plen, current, amount);
        elapsed += amount;
        printf("After run: elapsed=%d P0=(b=%d,w=%d) P1=(b=%d,w=%d)\n",
               elapsed,
               procs[0].burst_left, procs[0].wait,
               procs[1].burst_left, procs[1].wait);
        current = rr_next(current, procs, plen);
    }
    printf("Final elapsed=%d\n", elapsed);
    return 0;
}
