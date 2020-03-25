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
    int i;
    int sendTimes = 100;

    if (world_size < 2)
    {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (world_rank == 0)
    {
        int number = -1;
        MPI_Barrier(MPI_COMM_WORLD);
        for (i = 0; i < sendTimes; i++)
        {
            double start = MPI_Wtime();
            MPI_Ssend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            double end = MPI_Wtime();
            printf("%f\n", (end - start) * 1000 / sendTimes);
        }
    }
    else if (world_rank == 1)
    {
        int number;
        MPI_Barrier(MPI_COMM_WORLD);
        for (i = 0; i < sendTimes; i++)
        {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    MPI_Finalize();
}
