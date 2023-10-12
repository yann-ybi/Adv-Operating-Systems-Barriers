#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include "gtmp.h"

int main(int argc, char** argv)
{
    int num_processes, num_threads, num_rounds = 3;
    int my_id;
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    
    if (argc < 3) {
        fprintf(stderr, "Usage: ./combined_harness [NUM_PROCS] [NUM_THREADS]\n");
        exit(EXIT_FAILURE);
    }

    num_processes = strtol(argv[1], NULL, 10);
    num_threads = strtol(argv[2], NULL, 10);

    gtmp_init(num_threads);

    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);

#pragma omp parallel shared(num_processes, num_threads)
    {
        int myNum = omp_get_thread_num();
        for(int i = 0; i < num_rounds; i++){
            printf("Process rank: %d, Thread: %d, at iteration %d\n", my_id, myNum, i);
            
            gtmp_barrier();     // OpenMP barrier
            
            printf("Process rank: %d, Thread: %d, passed iteration %d\n", my_id, myNum, i);
        }
    }

    gtmp_finalize();

    MPI_Finalize();

    return 0;
}
