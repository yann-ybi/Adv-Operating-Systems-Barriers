#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"

// av time in micro 0.003910
// av time in micro 0.003767

// av time in micro 0.007939
// av time in micro 0.007343
// av time in micro 0.007749
// av time in micro 0.007629

// av time in micro 0.038147
// av time in micro 0.037527
// av time in micro 0.038409
// av time in micro 0.038099
// av time in micro 0.038314
// av time in micro 0.037909

// av time in micro 0.031805
// av time in micro 0.031304
// av time in micro 0.031257
// av time in micro 0.031447
// av time in micro 0.031257
// av time in micro 0.031495
// av time in micro 0.031447
// av time in micro 0.031519

// av time in micro 0.033259
// av time in micro 0.033855
// av time in micro 0.033450
// av time in micro 0.033927
// av time in micro 0.033450
// av time in micro 0.033879
// av time in micro 0.033998
// av time in micro 0.033665
// av time in micro 0.033927
// av time in micro 0.033879

// av time in micro 0.050926
// av time in micro 0.051212
// av time in micro 0.051093
// av time in micro 0.051069
// av time in micro 0.050759
// av time in micro 0.051451
// av time in micro 0.050998
// av time in micro 0.051188
// av time in micro 0.051308
// av time in micro 0.051260
// av time in micro 0.051188
// av time in micro 0.050974

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
