#include "pch.h"
#include <iostream>
#include "mpi.h"
using namespace std;

int mainw(int argc, char *argv[])
{
	int rank, size, resultlen;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &resultlen);

	int sizeArr = 0;
	char *buffer = new char[30];

	if (rank == 0) {
		cout << "Enter text to array: " << endl;
		cin >> buffer;
		double starttime = MPI_Wtime();
		MPI_Send(buffer, 30, MPI_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD);
		cout << "Sending data to loop. Sending time: " << MPI_Wtime() - starttime << endl;
		MPI_Recv(buffer, 30, MPI_CHAR, size - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		cout << "Process nummber: " << rank << " receive data: " << buffer << " from process: " << size - 1
			<< ". Total running time: " << MPI_Wtime() - starttime << " sec" << endl;
	}
	else {
		double starttime = MPI_Wtime();
		MPI_Recv(buffer, 30, MPI_CHAR, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		cout << "Process nummber: " << rank << " receive data: " << buffer << " from process: " << rank - 1 << endl;
		if (rank == size - 1) {
			MPI_Send(buffer, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			cout <<"Sending data to: " << 0 << " process from: " << rank << " process. Sending time: " << MPI_Wtime() - starttime << " sec" << endl;
		}
		else {
			MPI_Send(buffer, 30, MPI_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD);
			cout << "Sending data to: " << rank + 1 << " process from: " << rank << " process. Sending time: " << MPI_Wtime() - starttime << " sec" << endl;
		}
	}

	MPI_Finalize();
	return 0;
}
