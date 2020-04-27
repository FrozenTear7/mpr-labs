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

void bucketSort(int array[], int n, int max_range, int result[])
{
    int i, j, k;
    int buckets_size = sqrt(n) / omp_get_max_threads();
    int max_buckets = buckets_size * omp_get_max_threads();
    printf("Max %d per thread %d\n", max_buckets, buckets_size);
    struct bucket *buckets;
    int *bucketSizes = (int *)malloc(sizeof(int) * max_buckets);

    int bucketIndex, bucketLocalIndex, lowerRange, upperRange;

#pragma omp parallel private(buckets, i, j, k, bucketIndex, bucketLocalIndex, lowerRange, upperRange) shared(result)
    {
        buckets = (struct bucket *)malloc(sizeof(struct bucket) * buckets_size);
        for (i = 0; i < buckets_size; i++)
        {
            buckets[i].count = 0;
            buckets[i].value = (int *)malloc(sizeof(int) * n);
        }

        for (i = 0; i < n; i++)
        {
            bucketIndex = (double)array[i] / max_range * max_buckets;
            bucketLocalIndex = bucketIndex % buckets_size;
            lowerRange = buckets_size * omp_get_thread_num();
            upperRange = buckets_size * (omp_get_thread_num() + 1) - 1;

            if (bucketIndex >= lowerRange && bucketIndex <= upperRange)
            {
                printf("%d %d %d %d %d %d\n", omp_get_thread_num(), bucketIndex, bucketLocalIndex, lowerRange, upperRange, array[i]);
                buckets[bucketLocalIndex].value[buckets[bucketLocalIndex].count++] = array[i];
            }
        }

        for (i = 0; i < buckets_size; i++)
        {
            bucketSizes[buckets_size * omp_get_thread_num() + i] = buckets[i].count;
            qsort(buckets[i].value, buckets[i].count, sizeof(int), &compareIntegers);
        }

#pragma omp barrier

        // for (i = 0; i < buckets_size; i++)
        // {
        //     for (j = 0; j < n; j++)
        //     {
        //         if (buckets[i].value[j] == 0)
        //             break;

        //         printf("ID %d - %d\n", omp_get_thread_num(), buckets[i].value[j]);
        //     }
        // }

        for (i = 0; i < buckets_size; i++)
        {
            printf("Thread %d bucket %d size %d\n", omp_get_thread_num(), i, buckets[i].count);
        }

        int offset = 0;

        for (i = 0; i < omp_get_thread_num(); i++)
        {
            offset += bucketSizes[i];
        }

        k = offset;
        for (i = 0; i < buckets_size; i++)
        {
            for (j = 0; j < buckets[i].count; j++)
            {
                result[k + j] = buckets[i].value[j];
            }
            k += buckets[i].count;
            // free(buckets[i].value);
        }

        // k = 0;
        // for (i = 0; i < buckets_size; i++)
        // {
        //     for (j = 0; j < buckets[i].count; j++)
        //     {
        //         array[k + j] = buckets[i].value[j];
        //     }
        //     k += buckets[i].count;
        //     // free(buckets[i].value);
        // }

        // #pragma omp for
        //         for (i = 0; i < buckets_size; i++)
        //         {
        //             for (j = 0; j < n; j++)
        //             {
        //                 if (buckets[i].value[j] == 0)
        //                     break;

        //                 printf("%d\n", buckets[i].value[j]);
        //             }
        //         }
    }

    // start_timer = omp_get_wtime();

    //     end_timer = omp_get_wtime();
    //     printf("Splitting into buckets took: %lf\n", end_timer - start_timer);

    //     start_timer = omp_get_wtime();

    //     end_timer = omp_get_wtime();
    //     printf("Sorting the buckets took: %lf\n", end_timer - start_timer);

    //     start_timer = omp_get_wtime();

    //     end_timer = omp_get_wtime();
    //     printf("Merging into initial array took: %lf\n", end_timer - start_timer);
}

int main(int argc, char **argv)
{
    printf("\nN of threads: %d\n", omp_get_max_threads());
    time_t t;
    srand((unsigned)time(&t));

    int n = atoi(argv[1]);
    int max_range = atoi(argv[2]);

    int *array = (int *)malloc(n * sizeof(int));
    int *result = (int *)malloc(n * sizeof(int));

    start_timer = omp_get_wtime();

    int i;
    for (i = 0; i < n; i++)
    {
        array[i] = rand() % max_range + 1;
        result[i] = 0;
    }

    // for (i = 0; i < n; i++)
    // {
    //     printf("%d\n", array[i]);
    // }
    // printf("\n\n\n");

    // end_timer = omp_get_wtime();
    // printf("Populating the array took: %lf\n", end_timer - start_timer);

    bucketSort(array, n, max_range, result);

    for (i = 0; i < n; i++)
    {
        printf("%d\n", result[i]);
    }

    free(array);
    free(result);

    return 0;
}
