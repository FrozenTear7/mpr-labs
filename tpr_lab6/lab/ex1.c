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

void splitToBuckets(int array[], int n, int maxRange, struct bucket buckets[], int bucketsSize, int maxBuckets)
{
    int i;

#pragma omp parallel private(i)
    for (i = 0 - omp_get_thread_num(); i < n; i++)
    {
        if (i < 0)
            continue;

        // Przydzielam wartość do bucketu biorac zakres wartości i ilości bucketów
        int bucketIndex = (double)array[i] / maxRange * maxBuckets;

        // Thread rozpatrza wartości tylko w zakresie swoich bucketów
        if (bucketIndex >= bucketsSize * omp_get_thread_num() && bucketIndex <= bucketsSize * (omp_get_thread_num() + 1) - 1)
        {
            buckets[bucketIndex].value[buckets[bucketIndex].count++] = array[i];
        }
    }
}

void sortBuckets(int array[], struct bucket buckets[], int bucketsSize)
{
    int i;

#pragma omp parallel private(i)
    {
        int startIndex = bucketsSize * omp_get_thread_num(), endIndex = bucketsSize * (omp_get_thread_num() + 1) - 1;

        for (i = startIndex; i <= endIndex; i++)
        {
            qsort(buckets[i].value, buckets[i].count, sizeof(int), &compareIntegers);
        }
    }
}

void mergeBuckets(int array[], struct bucket buckets[], int bucketsSize)
{
    int i, j, k;

#pragma omp parallel private(i, j, k)
    {
        // Obliczanie punktu startowego od którego thread ma wypełniać tablicę posortowanymi danymi
        int offset = 0;

        for (i = 0; i < omp_get_thread_num() * bucketsSize; i++)
        {
            offset += buckets[i].count;
        }

        int startIndex = bucketsSize * omp_get_thread_num(), endIndex = bucketsSize * (omp_get_thread_num() + 1) - 1;

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

void bucketSort(int array[], int n, int maxRange)
{
    int i;
    // Przykładowy rozkład danych do bucketów
    int bucketsSize = sqrt(n) / omp_get_max_threads();
    int maxBuckets = bucketsSize * omp_get_max_threads();

    // Inicjalizacja bucketów i ich tablic danych
    struct bucket *buckets = (struct bucket *)malloc(sizeof(struct bucket) * maxBuckets);
    for (i = 0; i < maxBuckets; i++)
    {
        buckets[i].count = 0;
        buckets[i].value = (int *)malloc(sizeof(int) * n);
    }

    startTimer = omp_get_wtime();

    splitToBuckets(array, n, maxRange, buckets, bucketsSize, maxBuckets);

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
    for (i = 0; i < maxBuckets; i++)
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

    int *array = (int *)malloc(n * sizeof(int));

    startTimer = omp_get_wtime();
    startTotalTimer = omp_get_wtime();

    populateArray(array, n, maxRange);

    endTimer = omp_get_wtime();
    printf("Populating the array took: %lf\n", endTimer - startTimer);

    bucketSort(array, n, maxRange);

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