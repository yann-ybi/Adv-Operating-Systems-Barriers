#include <omp.h>
#include "gtmp.h"

// Implementation of a sense reversal spin barrier

// Time taken for 2 threads: 0.000044 seconds
// Time taken for 4 threads: 0.000050 seconds
// Time taken for 6 threads: 0.000057 seconds
// Time taken for 8 threads: 0.000069 seconds

// Time taken for 2 threads: 0.000063 seconds
// Time taken for 4 threads: 0.000072 seconds
// Time taken for 6 threads: 0.000090 seconds
// Time taken for 8 threads: 0.000117 seconds

Time taken for 2 threads: 0.000060 seconds
Time taken for 4 threads: 0.000085 seconds
Time taken for 6 threads: 0.005265 seconds
Time taken for 8 threads: 0.146140 seconds

Time taken for 2 threads: 0.000059 seconds
Time taken for 4 threads: 0.000068 seconds
Time taken for 6 threads: 0.005268 seconds
Time taken for 8 threads: 0.005295 seconds

Time taken for 2 threads: 0.000063 seconds
Time taken for 4 threads: 0.078740 seconds
Time taken for 6 threads: 0.001313 seconds
Time taken for 8 threads: 0.000108 seconds


// declare global variables
static unsigned int counter;
static unsigned int sense;
static unsigned int total;

omp_lock_t lock;

// We initialize each variable and the lock
void gtmp_init(int num_thread) {
    counter = num_thread;
    total = num_thread;
    sense = 0;
    omp_init_lock(&lock);
}

void gtmp_barrier() {
    // Atomicaly track the state of the threads and the individual state of each thread with local variable
    omp_set_lock(&lock);
    counter--;
    unsigned local_counter = counter;
    unsigned local_sense = sense;
    omp_unset_lock(&lock);

    // If it is the last thread based on its counter.
    if (!local_counter) {
        // Reset the counter for next calls of the barrier function
        counter = total;
        sense++;
    }
    else { while((sense % 2) == (local_sense % 2)); }
}

void gtmp_finalize() {
    omp_destroy_lock(&lock);
}

