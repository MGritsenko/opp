/**
  * MPI Isend and Irecv example.
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
			MPI_Request send_req, recv_req;
			double send_value = value;
			// Start sending and receiving.			
			MPI_Isend(&send_value, 1, MPI_DOUBLE, send_rank, 1, MPI_COMM_WORLD, &send_req);
			MPI_Irecv(&value, 1, MPI_DOUBLE, recv_rank, 1, MPI_COMM_WORLD, &recv_req); // value will be overwritten
			
			// May do other actions (computations, etc.) here.
			
			// Wait for send and recv to finish.
			MPI_Wait(&send_req, MPI_STATUS_IGNORE);
			MPI_Wait(&recv_req, MPI_STATUS_IGNORE);
			printf("Rank %d send value %e to %d\n", rank, send_value, send_rank);
			printf("Rank %d recv value %e from %d\n", rank, value, recv_rank);            
        }
    }

    MPI_Finalize();
	
    return 0;
}
