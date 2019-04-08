#include "pch.h"
#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[])
{
	int rank, size, resultlen;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &resultlen);

	int sizeArr = 0;
	char *send = new char[sizeArr];
	char *req = new char[sizeArr];

	if (rank == 0) {
		cout << "Enter array size: ";
		cin >> sizeArr; cout << endl;
		for (int i = 0; i < size; i++)
		{
			send[i] = 147;
		}
		double starttime = MPI_Wtime();
		MPI_Send(send, sizeArr, MPI_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD);
		cout << "0  Sending data to loop. Sending time: " << MPI_Wtime() - starttime << endl;
		MPI_Recv(req, sizeArr, MPI_CHAR, size - 1, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		cout << "Process nummber: " << rank << " receive data: " << req << " from process: " << size - 1
			<< ". Total running time: " << MPI_Wtime() - starttime << " sec" << endl;
	}
	else {
		double starttime = MPI_Wtime();
		MPI_Recv(req, sizeArr, MPI_CHAR, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		cout << "Process nummber: " << rank << " receive data: " << req << " from process: " << size - 1 << endl;
		MPI_Send(req, sizeArr, MPI_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD);
		cout << rank + "  Sending data to: " << rank + 1 << " process from: " << rank << " process. Sending time: " << MPI_Wtime() - starttime << " sec" << endl;
	}

	MPI_Finalize();
	return 0;
}
