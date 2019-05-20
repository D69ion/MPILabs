#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <stdio.h>

#define NUM_DIMS 1

void task1(int argc, char **argv)
{
	int rank, size, i, A, B, dims[NUM_DIMS];
	int periods[NUM_DIMS], source, dest;
	int reorder = 0;
	MPI_Comm comm_cart;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	A = rank;
	B = -1;
	/* �������� ������ dims � ��������� ������ periods ��� ��������� "������" */
	for (i = 0; i < NUM_DIMS; i++) {
		dims[i] = 0;
		periods[i] = 1;
	}

	/* ��������� ������ dims, � ������� ����������� ������� ������� */
	MPI_Dims_create(size, NUM_DIMS, dims);

	/* ������� ��������� "������" � communicator-�� comm_cart */
	MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, reorder, &comm_cart);

	/* ������ ����� ������� ����� ������� ����� ������, � ����������� ������� �������� ������ */
	MPI_Cart_shift(comm_cart, 0, 1, &source, &dest);

	/* ������ ����� �������� ���� ������ (�������� ���������� A) �������� �����
	� ������� ������ � ��������� ������ � B �� �������� ����� � ������� ������.
	����� � ������ size-1 �������� ���� ������ ����� � ������ 0, � ����� 0 ���������
	������ �� ����� size-1. */
	MPI_Sendrecv(&A, 1, MPI_INT, dest, 12, &B, 1, MPI_INT, source, 12, comm_cart, &status);

	/* ������ ����� �������� ���� ���� (�� �� � ��� ������ �������� ����� � ������� ������ � ��������
	���������� B (���� �������� ����� � ������� ������). */
	printf("rank=%d B=%d\n", rank, B);

	/* ��� ����� ��������� ��������� ��������, ��������� � ���������� comm_cart � ���������
	���������� ��������� */
	MPI_Comm_free(&comm_cart);

	MPI_Finalize();
}

void task2(int argc, char **argv)
{
	int rank, size, i, A, B, dims[NUM_DIMS],
		periods[NUM_DIMS], source, dest,
		reorder = 0;
	MPI_Comm comm_cart;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	A = rank;
	dims[0] = 0;
	periods[0] = 1;

	MPI_Dims_create(size, NUM_DIMS, dims);
	MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, reorder, &comm_cart);
	MPI_Cart_shift(comm_cart, 0, 1, &source, &dest);

	if (rank == 0) {
		MPI_Sendrecv(&A, 1, MPI_INT, rank + 1, 0, &B, 1, MPI_INT, size - 1, 0, comm_cart, &status);
	}
	else {
		int recivefrom = rank - 1;
		int sendto;
		if (rank == size - 1) 
			sendto = 0;
		else 
			sendto = rank + 1;
		MPI_Sendrecv(&A, 1, MPI_INT, sendto, 0, &B, 1, MPI_INT, recivefrom, 0, comm_cart, &status);
	}
	printf("rank=%d A=%d\n", rank, B);
	fflush(stdout);

	MPI_Comm_free(&comm_cart);
	MPI_Finalize();
}

int main(int argc, char **argv)
{
	//task1(argc, argv);
	task2(argc, argv);
	return 0;
}
/*	MPI_Cart_shift(comm_cart, 0, -1, &source, &dest);
	if (rank == 0) {
		MPI_Sendrecv(&A, 1, MPI_INT, size - 1, 0, &B, 1, MPI_INT, rank + 1, 0, comm_cart, &status);
	}
	else {
		int recivefrom;
		int sendto = rank - 1;
		if (rank == size - 1) 
			recivefrom = 0;
		else 
			recivefrom = rank + 1;
		MPI_Sendrecv(&A, 1, MPI_INT, sendto, 0, &B, 1, MPI_INT, recivefrom, 0, comm_cart, &status);
	}
	printf("rank=%d B=%d\n", rank, B);
	fflush(stdout);

	MPI_Comm_free(&comm_cart);
	MPI_Finalize();
*/
