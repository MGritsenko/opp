/**
  * MPI Send-recv example.
 **/

#include <mpi.h>
#include <cstdlib>

int main(int argc, char **argv) {    
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);    

    double value = 0.0 + rank;

    const int send_rank = (rank + 1) % size;
    const int recv_rank = (rank - 1 + size) % size;

    if (size == 1) {
        printf("Must be more than 1 process!\n");
    } else {
        for (int i = 0; i < size; i++) {
            if (rank % 2 == 0) { // even ranks send first (to prevent hanging)
                double send_value = value;
                MPI_Send(&send_value, 1, MPI_DOUBLE, send_rank, 1, MPI_COMM_WORLD);
                MPI_Recv(&value, 1, MPI_DOUBLE, recv_rank, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE); // value will be overwritten
                printf("Rank %d send value %e to %d\n", rank, send_value, send_rank);
                printf("Rank %d recv value %e from %d\n", rank, value, recv_rank);
            } else { // odd ranks recv first
                double recv_value = value;
                MPI_Recv(&recv_value, 1, MPI_DOUBLE, recv_rank, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                MPI_Send(&value, 1, MPI_DOUBLE, send_rank, 1, MPI_COMM_WORLD);
                printf("Rank %d recv value %e from %d\n", rank, recv_value, recv_rank);
                printf("Rank %d send value %e to %d\n", rank, value, send_rank);
                value = recv_value;
            }
        }
    }

    MPI_Finalize();

    return 0;
}
