#!/bin/bash

mpicc -o ex4_c ex4.c
mpiexec -np 2 ./ex4_c 100 > pingTest2_1.txt
mpiexec -machinefile ./nodes2 -np 2 ./ex4_c 100 > pingTest2_2.txt
mpiexec -machinefile ./nodes3 -np 2 ./ex4_c 100 > pingTest2_3.txt
mpiexec -machinefile ./nodes4 -np 2 ./ex4_c 100 > pingTest2_4.txt