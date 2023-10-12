#!/bin/bash

#SBATCH -J cs6210-proj2-combinedhello
#SBATCH -N 2 --ntasks-per-node=6
#SBATCH --mem-per-cpu=1G
#SBATCH -t 99
#SBATCH -q coc-ice
#SBATCH -o combined.out

echo "Started on `/bin/hostname`"

cd ~/combined

module load gcc/12.1.0-qgxpzk mvapich2/2.3
export OMP_NUM_THREADS=6

mpicc -Wall -lm -fopenmp -lgomp harness.c gtm1.c -o combined

srun combined 2 6
