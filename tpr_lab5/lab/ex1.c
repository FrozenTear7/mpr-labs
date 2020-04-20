#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
    time_t t;
    srand((unsigned)time(&t));

    int dim = atoi(argv[1]);

    int *matrix = (int *)malloc(dim * dim * sizeof(int));
    int *vector = (int *)malloc(dim * sizeof(int));
    int *result = (int *)malloc(dim * sizeof(int));

    // Wypelniam macierz losowymi intami
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            *(matrix + i * dim + j) = rand() % 50;

    // A nastepnie wektor oraz wypelnienie wynikowego wektora zerami
    for (i = 0; i < dim; i++)
    {
        *(vector + i) = rand() % 50;
        *(result + i) = 0;
    }

    // printf("Matrix\n");
    // for (i = 0; i < dim; i++)
    // {
    //     for (j = 0; j < dim; j++)
    //         printf("%d ", *(matrix + i * dim + j));

    //     printf("\n");
    // }

    // printf("\nVector\n");
    // for (i = 0; i < dim; i++)
    //     printf("%d ", *(vector + i));

    // printf("\n\n");

    double start_timer = omp_get_wtime();

#pragma omp parallel for
    for (i = 0; i < dim; i++)
#pragma omp parallel for
        for (j = 0; j < dim; j++)
            *(result + i) += *(matrix + i * dim + j) * *(vector + j);

    double end_timer = omp_get_wtime();
    printf("Calculations took: %lfs, for %d dim and %d threads\n", end_timer - start_timer, dim, omp_get_max_threads());

    // printf("Result\n");
    // for (i = 0; i < dim; i++)
    //     printf("%d ", *(result + i));

    // printf("\n\n");

    return 0;
}
