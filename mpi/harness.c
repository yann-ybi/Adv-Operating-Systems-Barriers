#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "gtmpi.h"

int main(int argc, char** argv)
{
    int num_processes, num_rounds = 10;
    int my_id;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    //   if (argc < 2){
    //     fprintf(stderr, "Usage: ./harness [NUM_PROCS]\n");
    //     exit(EXIT_FAILURE);
    //   }

    //   num_processes = strtol(argv[1], NULL, 10);
    num_processes = 2;

    gtmpi_init(num_processes);
    double start_time, end_time;
    start_time = MPI_Wtime();
    int k;
    for(k = 0; k < num_rounds; k++){
    // printf("Process rank: %d, at round %d -- \n", my_id, k);
    gtmpi_barrier();
    // printf("Process rank: %d, passed round %d ++ \n", my_id, k);
    }
    end_time = MPI_Wtime();
    printf("av time in micro %f\n", (end_time - start_time) * 100);
    gtmpi_finalize();  

    MPI_Finalize();

    return 0;
}

