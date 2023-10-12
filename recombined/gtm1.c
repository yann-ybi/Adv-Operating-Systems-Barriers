#include <omp.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmp.h"

static unsigned int counter;
static unsigned int ready;
static unsigned int total;
omp_lock_t lock;
unsigned int num_processes_global;
int my_id;

void gtmp_init(int num_processes, int num_threads){
    counter = num_threads;
    total = num_threads;
    ready = 0;
    num_processes_global = num_processes;
    omp_init_lock(&lock);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
}

void gtmp_barrier() {
    // OpenMP barrier
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


    // If it's thread 0, participate in the MPI barrier
    if(omp_get_thread_num() == 0) {
        int token;
        MPI_Status status;

        int child1 = 2 * my_id + 1;
        int child2 = 2 * my_id + 2;
        int parent = (my_id - 1) / 2;
        
        if(child1 < num_processes_global)
            MPI_Recv(&token, 1, MPI_INT, child1, 0, MPI_COMM_WORLD, &status);
        if(child2 < num_processes_global)
            MPI_Recv(&token, 1, MPI_INT, child2, 0, MPI_COMM_WORLD, &status);
        
        if(my_id != 0)
            MPI_Send(&token, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
        
        if(my_id == 0) {
            if(child1 < num_processes_global)
                MPI_Send(&token, 1, MPI_INT, child1, 0, MPI_COMM_WORLD);
            if(child2 < num_processes_global)
                MPI_Send(&token, 1, MPI_INT, child2, 0, MPI_COMM_WORLD);
        } else {
            MPI_Recv(&token, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, &status);
            if(child1 < num_processes_global)
                MPI_Send(&token, 1, MPI_INT, child1, 0, MPI_COMM_WORLD);
            if(child2 < num_processes_global)
                MPI_Send(&token, 1, MPI_INT, child2, 0, MPI_COMM_WORLD);
        }
    }

    // Again, use OpenMP barrier to synchronize threads within each MPI process
    omp_set_lock(&lock);
    counter--;
    local_counter = counter;
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
