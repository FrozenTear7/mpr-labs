#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --time=00:05:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2 - non-scaling - tests"

for ((i=100000; i<=10000000000000; i *= 10)); do
    mpiexec -np 1 ./ex2_c $i 
done