#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:30:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr20

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2 - scaling - n: 300000000" > 2_medium.txt

for ((i=1; i<=12; i++)); do
    echo -e "\nNP = $i\n" >> 2_medium.txt
    mpiexec -np $i ./ex2_c 300000000 >> 2_medium.txt
    mpiexec -np $i ./ex2_c 300000000 >> 2_medium.txt
    mpiexec -np $i ./ex2_c 300000000 >> 2_medium.txt
done