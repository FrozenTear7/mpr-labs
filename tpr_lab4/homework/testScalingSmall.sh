#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:00:30
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr20

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2 - scaling - n: 100000" > 2_small.txt

for ((i=1; i<=12; i++)); do
    echo -e "\nNP = $i\n" >> 2_small.txt
    mpiexec -np $i ./ex2_c 100000 >> 2_small.txt
    mpiexec -np $i ./ex2_c 100000 >> 2_small.txt
    mpiexec -np $i ./ex2_c 100000 >> 2_small.txt
done