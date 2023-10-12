#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <sys/time.h>
#include "gtmp.h"

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

int main(int argc, char** argv)
{
    int num_processes, num_threads, num_rounds = 10;
    int my_id;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    
    // if (argc < 3) {
    //     fprintf(stderr, "Usage: ./combined_harness [NUM_PROCS] [NUM_THREADS]\n");
    //     exit(EXIT_FAILURE);
    // }

    // num_processes = strtol(argv[1], NULL, 10);
    // num_threads = strtol(argv[2], NULL, 10);

    for (num_processes = 2; num_processes <= 8; num_processes += 2) {
        for (num_threads = 2; num_threads <= 12; num_threads += 2) {

            gtmp_init(num_processes, num_threads);
            omp_set_dynamic(0);
            omp_set_num_threads(num_threads);
            start_time = get_time();

            #pragma omp parallel shared(num_processes, num_threads)
            {
                int myNum = omp_get_thread_num();
                for(int i = 0; i < num_rounds; i++){
                    printf("Process rank: %d, Thread: %d, at iteration %d\n", my_id, myNum, i);
                    
                    gtmp_barrier();     // Combined barrier
            
                    printf("Process rank: %d, Thread: %d, passed iteration %d\n", my_id, myNum, i);
                }
            }

            end_time = get_time();

            printf("Time taken for %d MPI processes and %d OpenMP threads: %lf seconds\n", num_processes, num_threads, end_time - start_time);
            gtmp_finalize();
        }
    }

    MPI_Finalize();

    return 0;
}
