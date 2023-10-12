#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"

unsigned int num_processes_global;
int my_id;

void gtmpi_init(int num_processes){
    num_processes_global = num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
}

void gtmpi_barrier() {
    int token;
    MPI_Status status;
    
    int child1 = 2 * my_id + 1;
    int child2 = 2 * my_id + 2;
    int parent = (my_id - 1) / 2;
    
    // Receive messages from children if they exist
    if(child1 < num_processes_global)
        MPI_Recv(&token, 1, MPI_INT, child1, 0, MPI_COMM_WORLD, &status);
    if(child2 < num_processes_global)
        MPI_Recv(&token, 1, MPI_INT, child2, 0, MPI_COMM_WORLD, &status);
    
    // If not root, send message to parent
    if(my_id != 0)
        MPI_Send(&token, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
    
    // Root sends release messages
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

void gtmpi_finalize(){

}
