#include <mpi.h>
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
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    srand(time(NULL) * world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    long countInside = 0;
    long countProcessInside = 0;
    long nOfPoints = atol(argv[1]);
    long i;

    MPI_Barrier(MPI_COMM_WORLD);

    double startTime = MPI_Wtime();

    for (i = 0; i < nOfPoints; i++)
    {
        float x = (float)rand() / RAND_MAX;
        float y = (float)rand() / RAND_MAX;
        countProcessInside = countProcessInside + isInside(x, y);
    }

    MPI_Reduce(&countProcessInside, &countInside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        double endTime = MPI_Wtime();
        double timeElapsed = endTime - startTime;
        printf("\nTime: %lf\n", timeElapsed);
    }

    MPI_Finalize();
}
