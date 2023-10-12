#include <stdio.h>
#include <stdlib.h>
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
  int num_threads, num_iter=30;

//   if (argc < 2){
//     fprintf(stderr, "Usage: ./harness [NUM_THREADS]\n");
//     exit(EXIT_FAILURE);
//   }
//   num_threads = strtol(argv[1], NULL, 10);

  omp_set_dynamic(0);
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

  double start_time, end_time;

  for (num_threads = 2; num_threads <= 8; num_threads +=2) {
    omp_set_num_threads(num_threads);
  
    gtmp_init(num_threads);

    start_time = get_time();

    #pragma omp parallel shared(num_threads)
    {
    //   int myNum = omp_get_thread_num();
      int i;
      for(i = 0; i < num_iter; i++) {
        // printf("Thread: %d, at iteration %d \n", myNum, i);
        gtmp_barrier();
        // printf("Thread: %d, passed that barrier %d hahaha\n", myNum, i);
      }
    }

    end_time = get_time();

    printf("Time taken for %d threads: %lf seconds\n", num_threads, end_time - start_time);
    
    gtmp_finalize();
  }

  return 0;
}
