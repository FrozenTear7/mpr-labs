#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=03:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr20

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2 - scaling - n: 6000000000" > 2_large.txt

for ((i=1; i<=12; i++)); do
    echo -e "\nNP = $i\n" >> 2_large.txt
    mpiexec -np $i ./ex2_c 6000000000 >> 2_large.txt
    mpiexec -np $i ./ex2_c 6000000000 >> 2_large.txt
    mpiexec -np $i ./ex2_c 6000000000 >> 2_large.txt
done