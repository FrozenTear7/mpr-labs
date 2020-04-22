gcc -Wall ex1.c -o ex1 -fopenmp
echo -e "Testing all\n" > ex1.txt
./testSetup.sh "static"
./testSetup.sh "dynamic,4"
./testSetup.sh "dynamic,8"
./testSetup.sh "guided,4"
./testSetup.sh "guided,12"
./testSetup.sh "auto"