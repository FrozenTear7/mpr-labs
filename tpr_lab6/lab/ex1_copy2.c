#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

double start_timer, end_timer;

struct bucket
{
    int count;
    int *value;
};

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

void bucketSort(int array[], int n, int max_range)
{
    int i, j, k;
    int max_buckets = sqrt(n);
    int buckets_size = max_buckets / omp_get_max_threads();
    struct bucket *buckets;

#pragma omp parallel
    {
        buckets = (struct bucket *)malloc(sizeof(struct bucket) * buckets_size);
        for (i = 0; i < buckets_size; i++)
        {
            buckets[i].count = 0;
            buckets[i].value = (int *)malloc(sizeof(int) * n);
        }
    }

    start_timer = omp_get_wtime();

    for (i = 0; i < n; i++)
    {
        int bucketIndex = (double)array[i] / max_range * buckets_size;
        buckets[bucketIndex].value[buckets[bucketIndex].count++] = array[i];
    }

    end_timer = omp_get_wtime();
    printf("Splitting into buckets took: %lf\n", end_timer - start_timer);

    start_timer = omp_get_wtime();

    for (k = 0, i = 0; i < 3; i++)
    {
        // now using quicksort to sort the elements of buckets
        qsort(buckets[i].value, buckets[i].count, sizeof(int), &compareIntegers);
    }

    end_timer = omp_get_wtime();
    printf("Sorting the buckets took: %lf\n", end_timer - start_timer);

    start_timer = omp_get_wtime();

    for (k = 0, i = 0; i < buckets_size; i++)
    {
        for (j = 0; j < buckets[i].count; j++)
        {
            array[k + j] = buckets[i].value[j];
        }
        k += buckets[i].count;
        free(buckets[i].value);
    }

    end_timer = omp_get_wtime();
    printf("Merging into initial array took: %lf\n", end_timer - start_timer);
}

int main(int argc, char **argv)
{
    printf("\nN of threads: %d\n", omp_get_max_threads());
    time_t t;
    srand((unsigned)time(&t));

    int n = atoi(argv[1]);
    int max_range = atoi(argv[2]);

    int *array = (int *)malloc(n * sizeof(int));

    start_timer = omp_get_wtime();

    // Losuję wartości w odpowiednim zakresie do wektora
    int i;
    for (i = 0; i < n; i++)
    {
        array[i] = rand() % max_range;
    }

    end_timer = omp_get_wtime();
    printf("Populating the array took: %lf\n", end_timer - start_timer);

    bucketSort(array, n, max_range);

    // for (i = 0; i < n; i++)
    // {
    //     printf("%d\n", array[i]);
    // }

    free(array);

    return 0;
}
