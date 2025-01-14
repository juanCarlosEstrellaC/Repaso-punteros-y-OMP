#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int value = 100;
    int rank_dest = rank + 1;
    int rank_origen = rank - 1;

    if (rank == nprocs - 1){
        rank_dest = 0;
    }

    if (rank == 0){
        rank_origen = nprocs - 1;
    }

    if (rank == 0){
        MPI_Send(&value,1, MPI_INT, rank_dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, rank_origen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }else{
        MPI_Recv(&value, 1, MPI_INT, rank_origen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value = value + 1;
        MPI_Send(&value,1, MPI_INT, rank_dest, 0, MPI_COMM_WORLD);
    }
    printf("Rank %d: %d\n", rank, value);

    MPI_Finalize();
}
