#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define R 1
 
int isInside(float x, float y)
{
    if (R * R < x * x + y * y)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
 
int main(int argc, char **argv)
{
    srand(time(NULL));
 
    long nOfPoints = atol(argv[1]);
    long i, j;
 
    for (i = 0; i < 100; i++)
    {
        long countInside = 0;
 
        for (j = 0; j < nOfPoints; j++)
        {
            float x = (float)rand() / RAND_MAX;
            float y = (float)rand() / RAND_MAX;
 
            countInside += isInside(x, y);
        }
 
        printf("%f\n", (float)countInside / (float)nOfPoints * 4.0);
    }
}
