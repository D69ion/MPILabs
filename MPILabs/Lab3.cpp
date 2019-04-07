#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "mpi.h"
#include <Windows.h>
#include <iostream>
#include <math.h>

using namespace std;
#define PmpiI25DT 3.141592653589793238462643

int mainn(int argc, char *argv[])
{
	int rank, rankc, resultlen, n = 1, i;
	double result, h, sum, x, starttime;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &rankc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &resultlen);

	cout << "Proccess number " << rank << " started on computer: " << name << endl;
	MPI_Barrier(MPI_COMM_WORLD);
	while (n != 0) {
		starttime = MPI_Wtime();

		if (rank == 0) {
			cout << "Enter number of the intervals (0 for exit): " << endl;
			cin >> n;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

		sum = 0.0;
		h = 2 * n;
		i = rank + 1;

		while (i <= n) {
			x = (2 * i - 1) / h;
			sum += 4.0 / (1.0 + x * x);
			i += rankc;
		}
		sum = 1.0 / n * sum;
		MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			cout << "Result: " << result << ", calculation error: " << result - PmpiI25DT << " runtime: " << MPI_Wtime() - starttime << endl;
		}
	}
	MPI_Finalize();
	return 0;
}
