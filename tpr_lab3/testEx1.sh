#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:30:00
#SBATCH --partition=plgrid
#SBATCH --account=plgfrozentear72020a

cc -o ex1_c ex1.c

echo -e "Ex1"

for ((i=100; i<=100000000; i *= 10)); do
    echo -e "\nn = $i\n"
    ./ex1_c $i
done