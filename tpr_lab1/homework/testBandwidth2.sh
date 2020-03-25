#!/bin/bash

mpicc -o ex3_c ex3.c
mpiexec -machinefile ./nodes1 -np 2 ./ex3_c > bandwidthTest2_1.txt
mpiexec -machinefile ./nodes2 -np 2 ./ex3_c > bandwidthTest2_2.txt
mpiexec -machinefile ./nodes3 -np 2 ./ex3_c > bandwidthTest2_3.txt
mpiexec -machinefile ./nodes4 -np 2 ./ex3_c > bandwidthTest2_4.txt