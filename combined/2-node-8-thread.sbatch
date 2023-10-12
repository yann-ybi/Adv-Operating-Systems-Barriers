#!/bin/bash

#SBATCH -J cs6210-proj2-combined
#SBATCH -N 2 --ntasks-per-node=8
#SBATCH --mem-per-cpu=1G
#SBATCH -t 9999:00:00
#SBATCH -q coc-ice
#SBATCH -o combined.out

echo "Started on `/bin/hostname`"

cd ~/examples

module load gcc/12.1.0-qgxpzk mvapich2/2.3
export OMP_NUM_THREADS=8

mpicc -Wall -lm -fopenmp -lgomp harness.c gtm1.c -o combined

srun combined
