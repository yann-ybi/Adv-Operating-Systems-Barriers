#!/bin/bash

#SBATCH -J cs6210-proj2-mpihello
#SBATCH -N 2 --ntasks-per-node=1
#SBATCH --mem-per-cpu=1G
#SBATCH -t 99
#SBATCH -q coc-ice
#SBATCH -o mpi_hello.out

echo "Started on `/bin/hostname`"


cd ~/mpi

module load gcc/12.1.0-qgxpzk mvapich2/2.3
mpicc -Wall harness.c gtmpi1.c -o mpi1

srun mpi1 2
