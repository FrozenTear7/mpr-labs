#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --time=00:05:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex1_c ex1.c

echo -e "Ex1 - non-scaling - tests"

for ((i=100000; i<=10000000000000; i *= 10)); do
    mpiexec -np 1 ./ex1_c $i 
done