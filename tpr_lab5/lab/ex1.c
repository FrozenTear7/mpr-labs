#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
    time_t t;
    srand((unsigned)time(&t));

    int vec_size = atoi(argv[1]);
    int max_rand = atoi(argv[2]);

    double *vector = (double *)malloc(vec_size * sizeof(double));

    double start_timer = omp_get_wtime();

    // Losuję wartości w odpowiednim zakresie do wektora
    int i;
#pragma omp parallel for
    for (i = 0; i < vec_size; i++)
    {
        *(vector + i) = rand() * max_rand;
    }

    double end_timer = omp_get_wtime();
    printf("Calculations took: %lfs, for %d size and %d threads\n", end_timer - start_timer, vec_size, omp_get_max_threads());

    return 0;
}
