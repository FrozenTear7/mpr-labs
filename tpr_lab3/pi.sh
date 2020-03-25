#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:30:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2" > test2.txt

for ((i=10; i<=100000; i *= 10)); do
    mpiexec -np $i ./ex2_c >> test2.txt
done