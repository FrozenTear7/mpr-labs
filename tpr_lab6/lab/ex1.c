/*
Kompilacja i przykładowe uruchomienie (ilość wątków przekazuję przez env):
gcc -Wall ex1.c -o ex1 -fopenmp -lm
env OMP_NUM_THREADS=4 ./ex1 1000000 10000
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

double startTimer, endTimer, startTotalTimer, endTotalTimer;

// Struktura bucketa do trzymania danych i rozmiaru
struct bucket
{
    int count;
    int *value;
};

// Funkcja porównywawcza dla quicksorta
int compareIntegers(const void *first, const void *second)
{
    int x = *((int *)first), y = *((int *)second);
    if (x == y)
    {
        return 0;
    }
    else if (x < y)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void splitToBuckets(int array[], int n, int maxRange, struct bucket buckets[], int bucketsSize)
{
#pragma omp parallel
    {
        int startIndex = (double)omp_get_thread_num() / omp_get_max_threads() * n;
        int i, indexCounter;

        for (i = startIndex, indexCounter = 0; indexCounter < n; i++, indexCounter++)
        {
            // Przydzielam wartość do bucketu biorac zakres wartości i ilości bucketów
            int bucketIndex = (double)array[i % n] / maxRange * bucketsSize;
            int startBucketIndex = ceil((double)omp_get_thread_num() / omp_get_max_threads() * bucketsSize);
            int endBucketIndex = ceil((double)(omp_get_thread_num() + 1) / omp_get_max_threads() * bucketsSize - 1);

            // Thread rozpatrza wartości tylko w zakresie swoich bucketów
            if (bucketIndex >= startBucketIndex && bucketIndex <= endBucketIndex)
            {
                buckets[bucketIndex].value[buckets[bucketIndex].count++] = array[i % n];
            }
        }
    }
}

void sortBuckets(int array[], struct bucket buckets[], int bucketsSize)
{
#pragma omp parallel
    {
        int startIndex = ceil((double)omp_get_thread_num() / omp_get_max_threads() * bucketsSize);
        int endIndex = ceil((double)(omp_get_thread_num() + 1) / omp_get_max_threads() * bucketsSize - 1);

        int i;

        for (i = startIndex; i <= endIndex; i++)
        {
            qsort(buckets[i].value, buckets[i].count, sizeof(int), &compareIntegers);
        }
    }
}

void mergeBuckets(int array[], struct bucket buckets[], int bucketsSize)
{
#pragma omp parallel
    {
        int i, j, k;

        // Obliczanie punktu startowego od którego thread ma wypełniać tablicę posortowanymi danymi
        int offset = 0;
        int startIndex = ceil((double)omp_get_thread_num() / omp_get_max_threads() * bucketsSize);
        int endIndex = ceil((double)(omp_get_thread_num() + 1) / omp_get_max_threads() * bucketsSize - 1);

        for (i = 0; i < startIndex; i++)
        {
            offset += buckets[i].count;
        }

        // Thread wypełnia od punktu startowego równego rozmiarom poprzednich bucketów przechodząc przez swoje buckety
        for (k = offset, i = startIndex; i <= endIndex; i++)
        {
            for (j = 0; j < buckets[i].count; j++)
            {
                array[k + j] = buckets[i].value[j];
            }
            k += buckets[i].count;
        }
    }
}

void bucketSort(int array[], int n, int maxRange, int bucketsSize)
{
    int i;

    // Inicjalizacja bucketów i ich tablic danych
    struct bucket *buckets = (struct bucket *)malloc(sizeof(struct bucket) * bucketsSize);
    for (i = 0; i < bucketsSize; i++)
    {
        buckets[i].count = 0;
        buckets[i].value = (int *)malloc(sizeof(int) * n);
    }

    startTimer = omp_get_wtime();

    splitToBuckets(array, n, maxRange, buckets, bucketsSize);

    endTimer = omp_get_wtime();
    printf("Splitting into buckets took: %lf\n", endTimer - startTimer);

    startTimer = omp_get_wtime();

    // Sortowanie konkretnych bucketów przy pomocy np. quicksorta
    sortBuckets(array, buckets, bucketsSize);

    endTimer = omp_get_wtime();
    printf("Sorting the buckets took: %lf\n", endTimer - startTimer);

    startTimer = omp_get_wtime();

    mergeBuckets(array, buckets, bucketsSize);

    endTimer = omp_get_wtime();
    printf("Merging into initial array took: %lf\n", endTimer - startTimer);

    // Czyszczenie
    for (i = 0; i < bucketsSize; i++)
    {
        free(buckets[i].value);
    }

    free(buckets);
}

void sortCheck(int array[], int n)
{
    int i;

    for (i = 0; i < n - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            printf("Array is not sorted\n");
            return;
        }
    }

    printf("Array is sorted\n");
}

void populateArray(int array[], int n, int maxRange)
{
    int i;
    unsigned short xi[3];

#pragma omp parallel private(xi)
    {
        unsigned threadSeed = (unsigned)(time(NULL)) ^ omp_get_thread_num();
        xi[0] = threadSeed;
        xi[1] = threadSeed;
        xi[2] = threadSeed;

#pragma omp for
        // Wypełnianie tablicy losowymi danymi w zakresie podanym w argumentach programu
        for (i = 0; i < n; i++)
        {
            array[i] = (int)(erand48(xi) * maxRange);
        }
    }
}

int main(int argc, char **argv)
{
    printf("\nN of threads: %d\n", omp_get_max_threads());

    int n = atoi(argv[1]);
    int maxRange = atoi(argv[2]);
    int bucketsSize = atoi(argv[3]);

    if (bucketsSize < omp_get_max_threads())
    {
        printf("Please provide more buckets than threads\n");
        return -1;
    }

    int *array = (int *)malloc(n * sizeof(int));

    startTimer = omp_get_wtime();
    startTotalTimer = omp_get_wtime();

    populateArray(array, n, maxRange);

    endTimer = omp_get_wtime();
    printf("Populating the array took: %lf\n", endTimer - startTimer);

    bucketSort(array, n, maxRange, bucketsSize);

    endTotalTimer = omp_get_wtime();
    printf("The whole algorithm took: %lf\n", endTotalTimer - startTotalTimer);

    // Sprawdzenie czy tablica jest posortowana
    sortCheck(array, n);

    // int i;

    // for (i = 0; i < n; i++)
    // {
    //     printf("%d\n", array[i]);
    // }

    free(array);

    return 0;
}