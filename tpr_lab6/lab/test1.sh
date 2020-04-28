gcc -Wall ex1.c -o ex1 -fopenmp -lm -g
echo -e "Testing ex1" > ex1.txt

# env OMP_NUM_THREADS=4 ./ex1 100 100 >> ex1.txt

for (( i=1; i<=20; i+=1 ))
    do
        env OMP_NUM_THREADS=$i ./ex1 1000000 1000000 >> ex1.txt
    done