#include "pch.h"
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <iostream>
#include <windows.h>
using namespace std;

#pragma warning(disable: 4996 )
#define FILE_NAME "matrix.txt"
#define MAX_N 12
#define PROCESS_COUNT 4
#define ROOT_PROCESS 0

int main(int argc, char **argv) {
	int rank, process_count, iterations_count = 0;
	int first_row, last_row;
	MPI_Status status;
	double diffnorm, gdiffnorm;
	double xlocal[(MAX_N / 4) + 2][MAX_N];
	double xnew[(MAX_N / 3) + 2][MAX_N];
	double x[MAX_N][MAX_N];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);

	if (rank == ROOT_PROCESS) {
		FILE *file_pointer;
		file_pointer = fopen(FILE_NAME, "r");
		for (int i = MAX_N - 1; i >= 0; i--) {
			for (int j = 0; j < MAX_N; j++) {
				fscanf(file_pointer, "%lf", &x[i][j]);
			}
			fscanf(file_pointer, "\n");
		}
	}

	MPI_Scatter(x[0], MAX_N * (MAX_N / process_count), MPI_DOUBLE, xlocal[1], MAX_N * (MAX_N / process_count), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	first_row = 1;
	last_row = MAX_N / process_count;

	if (rank == ROOT_PROCESS)
		first_row++;

	if (rank == process_count - 1)
		last_row--;

	do {
		if (rank < process_count - 1) {
			MPI_Send(xlocal[MAX_N / process_count], MAX_N, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
		}
		if (rank > 0) {
			MPI_Recv(xlocal[0], MAX_N, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status);
			MPI_Send(xlocal[1], MAX_N, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);
		}

		if (rank < process_count - 1) {
			MPI_Recv(xlocal[MAX_N / process_count + 1], MAX_N, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &status);
		}

		iterations_count++;
		diffnorm = 0.0;
		for (int row = first_row; row <= last_row; row++)
			for (int column = 1; column < MAX_N - 1; column++) {
				xnew[row][column] = (xlocal[row][column + 1] + xlocal[row][column - 1] + xlocal[row + 1][column] + xlocal[row - 1][column]) / 4.0;
				diffnorm += (xnew[row][column] - xlocal[row][column]) *	(xnew[row][column] - xlocal[row][column]);
			}

		for (int row = first_row; row <= last_row; row++) {
			for (int column = 1; column < MAX_N - 1; column++) {
				xlocal[row][column] = xnew[row][column];
			}
		}

		MPI_Allreduce(&diffnorm, &gdiffnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		gdiffnorm += gdiffnorm;

		if (rank == ROOT_PROCESS) {
			cout << "At iteration " << iterations_count << ", diff is " << gdiffnorm << endl;
		}
	} while (gdiffnorm > 0.01 && iterations_count < 100);

	MPI_Gather(xlocal[1], MAX_N * (MAX_N / process_count), MPI_DOUBLE, x, MAX_N * (MAX_N / process_count), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (rank == ROOT_PROCESS) {
		cout << "Final solution is" << endl;
		for (int i = MAX_N - 1; i >= 0; i--) {
			for (int j = 0; j < MAX_N; j++) {
				cout << x[i][j] << ' ';
			}
			cout << endl;
		}
	}

	MPI_Finalize();
	return 0;
}