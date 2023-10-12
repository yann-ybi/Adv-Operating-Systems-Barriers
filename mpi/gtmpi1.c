#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"

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
