gcc -Wall ex1.c -o ex1 -fopenmp
# ./ex1 10 10
env OMP_NUM_THREADS=1 ./ex1 1000000 10
env OMP_NUM_THREADS=4 ./ex1 1000000 10