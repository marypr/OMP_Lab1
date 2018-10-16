#include "pch.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

void ReadArray(double* pdata, size_t nm, string file_path)
{
	ifstream is(file_path, ios::binary | ios::in);
	if (!is.is_open())
		return;
	is.read((char*)(pdata), nm * sizeof(double));
	is.close();
}

double* ExecuteParallel(int n, int m, int min, int max, int threads, double *parall_mass1, double *parall_mass2)
{
	LARGE_INTEGER parall_frequency, parall_start, parall_finish;

	int i, j, chunk, count1 = 0;
	chunk = n * 0.1;
	double *pulled_elems;
	QueryPerformanceFrequency(&parall_frequency);
	QueryPerformanceCounter(&parall_start);

#pragma omp parallel for private(i,j) reduction(+:count1) schedule(dynamic,chunk) num_threads(threads)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if (parall_mass1[i*m + j] >= min & parall_mass1[i*m + j] <= max)
			{
				parall_mass2[i*m + j] = (i*m + j);
				count1++;
			}
		}
	}

	pulled_elems = (double*)malloc(count1 * sizeof(double));
	int elems_count = 0;

#pragma omp parallel for private(i,j) schedule(dynamic,chunk) num_threads(threads)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if (parall_mass2[i*m + j] == 1)
			{
				pulled_elems[elems_count] = parall_mass1[i*m + j];
				elems_count++;
			}
		}
	}
	QueryPerformanceCounter(&parall_finish);

//double* ExecuteParallel(int n, int m, int min, int max, int threads, double *parall_mass1, char *parall_mass2)
//{
//	LARGE_INTEGER parall_frequency, parall_start, parall_finish;
//
//	int i, j, chunk, count1 = 0;
//	chunk = n * 0.1;
//	double *pulled_elems;
//	QueryPerformanceFrequency(&parall_frequency);
//	QueryPerformanceCounter(&parall_start);
//
//#pragma omp parallel for private(i,j) reduction(+:count1) schedule(dynamic,chunk) num_threads(threads)
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < m; j++)
//		{
//			if (parall_mass1[i*m + j] >= min & parall_mass1[i*m + j] <= max)
//			{
//				parall_mass2[i*m + j] = 1;
//				count1++;
//			}
//		}
//	}
//
//	pulled_elems = (double*)malloc(count1 * sizeof(double));
//	int elems_count = 0;
//#pragma omp parallel for private(i,j) reduction(+:elems_count) schedule(dynamic,chunk) num_threads(threads)
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < m; j++)
//		{
//			if (parall_mass2[i*m + j] == 1)
//			{
//				pulled_elems[elems_count] = parall_mass1[i*m + j];
//				elems_count++;
//			}
//		}
//	}
//	QueryPerformanceCounter(&parall_finish);

	//free(pulled_elems);
	printf("Elements quantity %d\n", count1);
	double time = (parall_finish.LowPart - parall_start.LowPart) * 1000.0f / parall_frequency.LowPart;
	printf("Parallel execution time for %dx%d matrix with %d threads = %f\n", n, m, threads, time);
	return pulled_elems;
}

bool CompareVectors(double *a, double *b, int elems_qty)
{
	for (int i = 0; i < elems_qty; i++)
	{
		if (a[i] != b[i]) {

			printf("\n%d = %d\n", a[i], b[i]);
			return false;
		}
	}
	return true;
}

double* ExecuteConsistent(int n, int m, int min, int max, double *cons_mass1, char *cons_mass2, int count2)
{
	LARGE_INTEGER cons_frequency, cons_start, cons_finish;
	int i, j;
	double *pulled_elems;
	QueryPerformanceFrequency(&cons_frequency);
	QueryPerformanceCounter(&cons_start);
#pragma omp single
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if (cons_mass1[i*m + j] >= min & cons_mass1[i*m + j] <= max)
			{
				cons_mass2[i*m + j] = 1;
				count2++;
			}
		}
	}
	QueryPerformanceCounter(&cons_finish);

	pulled_elems = (double*)malloc(count2 * sizeof(double));

	int elems_count = 0;
#pragma omp single
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if (cons_mass2[i*m + j] == 1)
			{
				pulled_elems[elems_count] = cons_mass1[i*m + j];
				elems_count++;
			}
		}
	}
	QueryPerformanceCounter(&cons_finish);

	//free(pulled_elems);
	printf("Elements quantity %d\n", count2);
	double time = (cons_finish.LowPart - cons_start.LowPart) * 1000.0f / cons_frequency.LowPart;
	printf("Consistent execution time for %dx%d matrix = %f\n", n, m, time);
	return pulled_elems;
}

int main()
{
	LARGE_INTEGER frequency, start_p, finish_p;

	double *cons_mass1, *parall_mass1, *parall_mass2;
	char  *cons_mass2;

	int n, m, threads, min, max;


	printf("Enter min element: ");
	scanf_s("%d", &min);
	printf("\nEnter max element: ");
	scanf_s("%d", &max);

	ofstream os("D:\\Stuff\\OpenMP\\CSVs\\task13.csv", ios::app);
	string path;
	int switchvar = 6;

	os << 4 << "," << 12 << "," << 20 << "," << 28 << "," << 36 << "," << 44 << "," << 1 << "\n";
	while (switchvar <= 6) {
		switch (switchvar)
		{
			//case(0): path = "C:\\Data\\Stuff\\OpenMP\\bin\\100.bin", n = 100, m = 100;
			//	break;
			//case(1): path = "C:\\Data\\Stuff\\OpenMP\\bin\\1000.bin", n = 1000, m = 1000;
			//	break;
			//case(2): path = "C:\\Data\\Stuff\\OpenMP\\bin\\2000.bin", n = 2000, m = 2000;
			//	break;
			//case(3): path = "C:\\Data\\Stuff\\OpenMP\\bin\\4000.bin", n = 4000, m = 4000;
			//	break;
			//case(4): path = "C:\\Data\\Stuff\\OpenMP\\bin\\6000.bin", n = 6000, m = 6000;
			//	break;
			//case(5): path = "C:\\Data\\Stuff\\OpenMP\\bin\\8000.bin", n = 8000, m = 8000;
			//	break;
			//case(6): path = "C:\\Data\\Stuff\\OpenMP\\bin\\10000.bin", n = 10000, m = 10000;
			//	break;
		case(0): path = "D:\\Stuff\\OpenMP\\bin\\100.bin", n = 100, m = 100;
			break;
		case(1): path = "D:\\Stuff\\OpenMP\\bin\\1000.bin", n = 1000, m = 1000;
			break;
		case(2): path = "D:\\Stuff\\OpenMP\\bin\\2000.bin", n = 2000, m = 2000;
			break;
		case(3): path = "D:\\Stuff\\OpenMP\\bin\\4000.bin", n = 4000, m = 4000;
			break;
		case(4): path = "D:\\Stuff\\OpenMP\\bin\\6000.bin", n = 6000, m = 6000;
			break;
		case(5): path = "D:\\Stuff\\OpenMP\\bin\\8000.bin", n = 8000, m = 8000;
			break;
		case(6): path = "D:\\Stuff\\OpenMP\\bin\\10000.bin", n = 10000, m = 10000;
			break;
		}

		//Execute parallel
		//for (threads = 4; threads <= 44; threads += 8)
		//{
		double *pulled_elems1, *pulled_elems2;
		int pulled_count = 0;
		threads = 4;
		parall_mass1 = (double*)malloc(n * m * sizeof(double));
		parall_mass2 = (double*)calloc(n * m, sizeof(double));
		ReadArray(parall_mass1, n*m, path);
		pulled_elems2 = ExecuteParallel(n, m, min, max, threads, parall_mass1, parall_mass2);
		//os << time1 << ",";
		free(parall_mass1);
		//free(parall_mass2);
	//}

	//Execute consistent
		cons_mass1 = (double*)malloc(n * m * sizeof(double));
		ReadArray(cons_mass1, n*m, path);
		cons_mass2 = (char*)calloc(n * m, sizeof(char));
		pulled_elems1 = ExecuteConsistent(n, m, min, max, cons_mass1, cons_mass2, pulled_count);
		//os << time2 << "\n";
		free(cons_mass1);
		//free(cons_mass2);
		printf(CompareVectors(pulled_elems1, pulled_elems2, pulled_count) ? "true" : "false");
		printf("\n\n%f = %f\n\n", pulled_elems1[pulled_count - 500], pulled_elems2[pulled_count - 500]);
		switchvar++;
	}
	os << "\n";
	os.close();
}