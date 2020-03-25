#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:10:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2 - ntasks: 12"

for ((i=10; i<=1000000000; i *= 10)); do
    echo -e "\nn = $i\n"
    mpiexec -np 2 ./ex2_c $i
done