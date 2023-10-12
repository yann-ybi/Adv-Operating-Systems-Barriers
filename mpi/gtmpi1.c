#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"


// av time in micro 0.003839
// av time in micro 0.003839

// av time in micro 0.004172
// av time in micro 0.004005

// av time in micro 0.003958
// av time in micro 0.004196


// av time in micro 0.033569
// av time in micro 0.033164
// av time in micro 0.033498
// av time in micro 0.033426


// av time in micro 0.028491
// av time in micro 0.028586
// av time in micro 0.028419
// av time in micro 0.028586
// av time in micro 0.028563
// av time in micro 0.028515

unsigned int num_processes_global;
int my_id;

void gtmpi_init(int num_processes) {
    num_processes_global = num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
}

void gtmpi_barrier() {
    int token = 1;
    MPI_Status status;

    if (my_id != 0) {
        MPI_Send(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        for(int k = 1; k < num_processes_global; k++) {
            
            MPI_Recv(&token, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        }
    }
    // Process 0 sends an acknowledgment to all processes 
    if(my_id == 0) {
        for(int k = 1; k < num_processes_global; k++) {
            MPI_Send(&token, 1, MPI_INT, k, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
}

void gtmpi_finalize() {

}
