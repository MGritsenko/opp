/**
  * MPI types example.
 **/

#include <mpi.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

const int MATRIX_SIZE_DEFAULT = 10;

// Create 2D matrix as 1D array.
std::vector<double> make_matrix(int size) {
	std::vector<double> matrix(size*size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i*size + j] = i + j;
		}
	}
	return matrix;
}

void make_matrix_and_send_columns(int size, int matrix_size) {
	std::vector<double> matrix = make_matrix(matrix_size);		
		
	// Make MPI type for matrix column.
	MPI_Datatype col_type;
	MPI_Type_vector(matrix_size, 1, matrix_size, MPI_DOUBLE, &col_type);
	MPI_Type_commit(&col_type);
				
	// Send each matrix's column to corresponding rank.	
	for (int dest_rank = 1; dest_rank < size; dest_rank++) {
		// For the case when there are more ranks than columns.
		const int col_index = std::min(dest_rank, matrix_size - 1); 
		MPI_Send(&matrix[0] + col_index, 1, col_type, dest_rank, 123, MPI_COMM_WORLD);
	}
	
	MPI_Type_free(&col_type);	
}

std::vector<double> recv_column() {
	// Probe recv from any rank to get recv parameters.
	MPI_Status status;
	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	
	int src_rank = status.MPI_SOURCE;
	int tag = status.MPI_TAG;
	int col_size;
	MPI_Get_count(&status, MPI_DOUBLE, &col_size);
	
	// Recv and print column.
	std::vector<double> column(col_size);
	MPI_Recv(&column[0], col_size, MPI_DOUBLE, src_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
	return column;
}

std::string array_to_string(const std::vector<double> &data) {
	std::ostringstream out;
	out << "[";
	const int data_size = data.size();
	for (int i = 0; i < data_size - 1; i++) {
		out << data[i] << ", ";
	}
	out << data.back() << "]";
	return out.str();
}

int main(int argc, char **argv) {    
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);    	
		
	if (rank == 0) {
		int matrix_size = (argc > 1 ? atoi(argv[1]) : MATRIX_SIZE_DEFAULT);      
		make_matrix_and_send_columns(size, matrix_size);
	} else {
		std::vector<double> column = recv_column();
		std::string column_str = array_to_string(column);
		printf("Rank %d received column %s\n", rank, column_str.c_str());
	}        

    MPI_Finalize();
	
    return 0;
}
