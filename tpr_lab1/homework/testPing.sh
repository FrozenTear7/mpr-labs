#!/bin/bash

mpicc -o ex2_c ex2.c
mpiexec -machinefile ./nodes1 -np 2 ./ex2_c > pingTest1_1.txt
mpiexec -machinefile ./nodes2 -np 2 ./ex2_c > pingTest1_2.txt
mpiexec -machinefile ./nodes3 -np 2 ./ex2_c > pingTest1_3.txt
mpiexec -machinefile ./nodes4 -np 2 ./ex2_c > pingTest1_4.txt