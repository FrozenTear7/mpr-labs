#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

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

void bucketSort(int array[], int n)
{
    struct bucket buckets[3];
    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        buckets[i].count = 0;
        buckets[i].value = (int *)malloc(sizeof(int) * n);
    }

    start_timer = omp_get_wtime();

    for (i = 0; i < n; i++)
    {
        if (array[i] < 0)
        {
            buckets[0].value[buckets[0].count++] = array[i];
        }
        else if (array[i] > 10)
        {
            buckets[2].value[buckets[2].count++] = array[i];
        }
        else
        {
            buckets[1].value[buckets[1].count++] = array[i];
        }
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

    for (k = 0, i = 0; i < 3; i++)
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
    int max_rand = atoi(argv[2]);

    int *array = (int *)malloc(n * sizeof(int));

    start_timer = omp_get_wtime();

    // Losuję wartości w odpowiednim zakresie do wektora
    int i;
#pragma omp parallel for
    for (i = 0; i < n; i++)
    {
        array[i] = rand() % max_rand;
    }

    end_timer = omp_get_wtime();
    printf("Populating the array took: %lf\n", end_timer - start_timer);

    bucketSort(array, n);

    // for (i = 0; i < n; i++)
    // {
    //     printf("%d\n", array[i]);
    // }

    free(array);

    return 0;
}
