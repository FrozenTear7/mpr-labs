#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=02:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

module add plgrid/tools/openmpi

mpicc -o ex2_c ex2.c

echo -e "Ex2 - scaling - n: 1000000000"

for ((i=1; i<=12; i++)); do
    echo -e "\nNP = $i\n"
    mpiexec -np $i ./ex2_c 1000000000
done