#ifndef GTMP_H
#define GTMP_H

void gtmp_init(int num_processes, int num_threads);
void gtmp_barrier();				\
void gtmp_finalize();
#endif