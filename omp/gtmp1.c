#include <omp.h>
#include "gtmp.h"

// Implementation of a counting (decrementing) spin barrier

// Time taken for 2 threads: 0.000056 seconds
// Time taken for 4 threads: 0.000064 seconds
// Time taken for 6 threads: 0.000082 seconds
// Time taken for 8 threads: 0.000102 seconds

// Time taken for 2 threads: 0.000052 seconds
// Time taken for 4 threads: 0.000066 seconds
// Time taken for 6 threads: 0.000078 seconds
// Time taken for 8 threads: 0.000099 seconds

// Time taken for 2 threads: 0.000049 seconds
// Time taken for 4 threads: 0.000064 seconds
// Time taken for 6 threads: 0.000084 seconds
// Time taken for 8 threads: 0.000098 seconds

// Time taken for 2 threads: 0.000048 seconds
// Time taken for 4 threads: 0.000062 seconds
// Time taken for 6 threads: 0.000073 seconds
// Time taken for 8 threads: 0.000096 seconds

// Time taken for 2 threads: 0.000048 seconds
// Time taken for 4 threads: 0.000062 seconds
// Time taken for 6 threads: 0.000077 seconds
// Time taken for 8 threads: 0.000096 seconds

// av time in micro 0.037050
// av time in micro 0.037193
// av time in micro 0.037098
// av time in micro 0.037122
// av time in micro 0.037074
// av time in micro 0.037217
// av time in micro 0.037146
// av time in micro 0.037360

// av time in micro 0.043893
// av time in micro 0.043941
// av time in micro 0.043869
// av time in micro 0.044298
// av time in micro 0.044155
// av time in micro 0.044107


// av time in micro 0.071478
// av time in micro 0.071836
// av time in micro 0.071263
// av time in micro 0.071764
// av time in micro 0.071478
// av time in micro 0.071645
// av time in micro 0.071144
// av time in micro 0.071025
// av time in micro 0.071692
// av time in micro 0.071740
// av time in micro 0.071406
// av time in micro 0.071597

// declare global variables
static unsigned int counter;
static unsigned int ready;
static unsigned int total;

omp_lock_t lock;

// We initialize each variable and the lock
void gtmp_init(int num_thread) {
    counter = num_thread;
    total = num_thread;
    ready = 0;
    omp_init_lock(&lock);
}

void gtmp_barrier() {
    
    // Atomicaly track the state of the threads and the individual state of each thread with local variable
    omp_set_lock(&lock);
    counter--;
    unsigned local_counter = counter;
    omp_unset_lock(&lock);

    // If it is the last thread based on its counter.
    if (!local_counter) {

        // Wait until the first threads realize that we have the last thread and stop spinning on counter
        while((ready % total)  != (total - 1));

        // Reset the counter for next calls of the barrier function
        counter = total;
    }
    else {
        // Wait until we have the last thread
        while (counter);
    }

    // Atomicaly signal all thread there I am ready to go
    omp_set_lock(&lock);
    ready++;
    omp_unset_lock(&lock);

    // Wait until everyone is ready to go then go !
    while((ready % total));
}

void gtmp_finalize() {
    omp_destroy_lock(&lock);
}
