gcc -Wall ex2.c -o ex2 -fopenmp
env OMP_NUM_THREADS=2 ./ex2 10000 10000 > ex2_1.txt
env OMP_NUM_THREADS=4 ./ex2 10000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=5 ./ex2 10000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=10 ./ex2 10000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=20 ./ex2 10000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=50 ./ex2 10000 10000 >> ex2_1.txt

env OMP_NUM_THREADS=2 ./ex2 1000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=4 ./ex2 1000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=5 ./ex2 1000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=10 ./ex2 1000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=20 ./ex2 1000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=50 ./ex2 1000000 10000 >> ex2_1.txt

env OMP_NUM_THREADS=2 ./ex2 100000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=4 ./ex2 100000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=5 ./ex2 100000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=10 ./ex2 100000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=20 ./ex2 100000000 10000 >> ex2_1.txt
env OMP_NUM_THREADS=50 ./ex2 100000000 10000 >> ex2_1.txt