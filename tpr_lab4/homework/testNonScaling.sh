#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:30:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex1_c ex1.c

echo -e "Ex1 - non-scaling\n"

for ((i=1; i<=12; i++)); do
    echo -e "NP = $i\n"
    echo -e "Small\n"
    mpiexec -np $i ./ex1_c 100000
    echo -e "Medium\n"
    mpiexec -np $i ./ex1_c 300000000
    echo -e "Large\n"
    mpiexec -np $i ./ex1_c 1000000000
done