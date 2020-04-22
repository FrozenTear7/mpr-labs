echo -e "$1\n" >> ex1.txt
echo $1
for i in {1..12}
do
    for (( j=10000; j<=100000000; j*=100 ))
    do
        env OMP_NUM_THREADS=$i OMP_SCHEDULE=$1 ./ex1 $j 10000 >> ex1.txt
    done
    echo -e "\n" >> ex1.txt
done
echo -e "\n" >> ex1.txt