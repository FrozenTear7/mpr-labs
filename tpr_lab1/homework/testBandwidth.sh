#!/bin/bash

mpicc -o ex1_c ex1.c
mpiexec -np 2 ./ex1_c $1 100 > bandwidthTest1_1.txt
mpiexec -machinefile ./nodes2 -np 2 ./ex1_c $1 100 > bandwidthTest1_2.txt
mpiexec -machinefile ./nodes3 -np 2 ./ex1_c $1 100 > bandwidthTest1_3.txt
mpiexec -machinefile ./nodes4 -np 2 ./ex1_c $1 100 > bandwidthTest1_4.txt