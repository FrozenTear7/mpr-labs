#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int arraySize = 1000;
    int sendTimes = 100;
    int mBit = 125000;

    if (world_size < 2)
    {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (world_rank == 0)
    {
        int i;
        int *dataArray = malloc(arraySize * sizeof(int));
        for (i = 0; i < arraySize; i++)
        {
            dataArray[i] = 9;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        for (i = 0; i < sendTimes; i++)
        {
            double start = MPI_Wtime();
            MPI_Send(dataArray, arraySize, MPI_INT, 1, 0, MPI_COMM_WORLD);
            double end = MPI_Wtime();
            double timeElapsed = end - start;
            printf("%f\n", arraySize * sizeof(int) / timeElapsed / mBit);
        }
    }
    else if (world_rank == 1)
    {
        int i;
        int *dataArray = malloc(arraySize * sizeof(int));

        MPI_Barrier(MPI_COMM_WORLD);

        for (i = 0; i < sendTimes; i++)
        {
            MPI_Recv(dataArray, arraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    MPI_Finalize();
}
