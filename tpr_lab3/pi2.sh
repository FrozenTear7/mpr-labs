#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:10:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex1_c ex1.c

echo -e "Ex1" > test1.txt

for ((i=10; i<=100000000; i *= 10)); do
    mpiexec -np 1 ./ex1_c $i >> test1.txt
done