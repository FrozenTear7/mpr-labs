#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:30:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr20

module add plgrid/tools/openmpi

mpicc -o ex1_c ex1.c

echo -e "Ex1 - non-scaling - n: 300000000" > 1_medium.txt

for ((i=1; i<=12; i++)); do
    echo -e "\nNP = $i\n" >> 1_medium.txt
    mpiexec -np $i ./ex1_c 300000000 >> 1_medium.txt
    mpiexec -np $i ./ex1_c 300000000 >> 1_medium.txt
    mpiexec -np $i ./ex1_c 300000000 >> 1_medium.txt
done