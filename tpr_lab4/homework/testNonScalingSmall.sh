#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:00:10
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex1_c ex1.c

echo -e "Ex1 - non-scaling - n: 100000"

for ((i=1; i<=12; i++)); do
    echo -e "\nNP = $i\n"
    mpiexec -np $i ./ex1_c 100000
done