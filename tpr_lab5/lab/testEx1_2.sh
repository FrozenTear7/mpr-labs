gcc -Wall ex1.c -o ex1 -fopenmp
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=2 ./ex1 100 > ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=4 ./ex1 100 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=5 ./ex1 100 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=10 ./ex1 100 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=20 ./ex1 100 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=50 ./ex1 100 >> ex1_2.txt

env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=2 ./ex1 1000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=4 ./ex1 1000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=5 ./ex1 1000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=10 ./ex1 1000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=20 ./ex1 1000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=50 ./ex1 1000 >> ex1_2.txt

env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=2 ./ex1 10000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=4 ./ex1 10000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=5 ./ex1 10000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=10 ./ex1 10000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=20 ./ex1 10000 >> ex1_2.txt
env OMP_SCHEDULE="guided,4" OMP_NUM_THREADS=50 ./ex1 10000 >> ex1_2.txt