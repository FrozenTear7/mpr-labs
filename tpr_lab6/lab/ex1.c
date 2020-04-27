#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

double startTimer, endTimer;

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

void bucketSort(int array[], int n, int maxRange)
{
    int i, j, k;
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

#pragma omp parallel private(i)
    for (i = 0; i < n; i++)
    {
        // Przydzielam wartość do bucketu biorac zakres wartości i ilości bucketów
        int bucketIndex = (double)array[i] / maxRange * maxBuckets;

        // Thread rozpatrza wartości tylko w zakresie swoich bucketów
        if (bucketIndex >= bucketsSize * omp_get_thread_num() && bucketIndex <= bucketsSize * (omp_get_thread_num() + 1) - 1)
        {
            buckets[bucketIndex].value[buckets[bucketIndex].count++] = array[i];
        }
    }

    endTimer = omp_get_wtime();
    printf("Splitting into buckets took: %lf\n", endTimer - startTimer);

    startTimer = omp_get_wtime();

    // Sortowanie konkretnych bucketów przy pomocy np. quicksorta
#pragma omp parallel private(i)
    for (i = bucketsSize * omp_get_thread_num(); i <= bucketsSize * (omp_get_thread_num() + 1) - 1; i++)
    {
        qsort(buckets[i].value, buckets[i].count, sizeof(int), &compareIntegers);
    }

    endTimer = omp_get_wtime();
    printf("Sorting the buckets took: %lf\n", endTimer - startTimer);

    startTimer = omp_get_wtime();

#pragma omp parallel private(i, j, k)
    {
        // Obliczanie punktu startowego od którego thread ma wypełniać tablicę posortowanymi danymi
        int offset = 0;

        for (i = 0; i < omp_get_thread_num() * bucketsSize; i++)
        {
            offset += buckets[i].count;
        }

        // Thread wypełnia od punktu startowego równego rozmiarom poprzednich bucketów przechodząc przez swoje buckety
        for (k = offset, i = omp_get_thread_num() * bucketsSize; i <= bucketsSize * (omp_get_thread_num() + 1) - 1; i++)
        {
            for (j = 0; j < buckets[i].count; j++)
            {
                array[k + j] = buckets[i].value[j];
            }
            k += buckets[i].count;
        }
    }

    endTimer = omp_get_wtime();
    printf("Merging into initial array took: %lf\n", endTimer - startTimer);

    // Czyszczenie
    for (i = 0; i < maxBuckets; i++)
    {
        free(buckets[i].value);
    }

    free(buckets);
}

int main(int argc, char **argv)
{
    time_t t;
    srand((unsigned)time(&t));

    int n = atoi(argv[1]);
    int maxRange = atoi(argv[2]);

    int *array = (int *)malloc(n * sizeof(int));

    startTimer = omp_get_wtime();

    int i;
    // Wypełnianie tablicy losowymi danymi w zakresie podanym w argumentach programu
    for (i = 0; i < n; i++)
    {
        array[i] = rand() % maxRange;
    }

    endTimer = omp_get_wtime();
    printf("Populating the array took: %lf\n", endTimer - startTimer);

    bucketSort(array, n, maxRange);

    // for (i = 0; i < n; i++)
    // {
    //     printf("%d\n", array[i]);
    // }

    free(array);

    return 0;
}
