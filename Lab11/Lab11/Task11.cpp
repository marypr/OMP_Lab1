#include "pch.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <locale.h>
#include <string>

using namespace std;

void ReadArray(double* pdata, size_t nm, string file_path)
{
	ifstream is(file_path, ios::binary | ios::in);
	if (!is.is_open())
		return;
	is.read((char*)(pdata), nm * sizeof(double));
	is.close();
}


double ExecuteParallel(int n, int m, int q, int threads, double *parall_mass)
{
	int i, j;
	double sum;
	int chunk = n * 0.1;
	LARGE_INTEGER parall_frequency, parall_start, parall_finish;

	// =========
	sum = 0;
	QueryPerformanceFrequency(&parall_frequency);
	QueryPerformanceCounter(&parall_start);

#pragma omp parallel shared(parall_mass) private(i, j) num_threads(threads)
	{
#pragma omp for reduction(+:sum) schedule(dynamic, chunk) nowait
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				parall_mass[i * m + j] *= q;
				sum += parall_mass[i * m + j];
			}
		}
	}
	QueryPerformanceCounter(&parall_finish);
	printf("Summ: %f\n", sum);
	double time1 = (parall_finish.LowPart - parall_start.LowPart) * 1000.0f / parall_frequency.LowPart;
	printf("Parallel execution time for %dx%d matrix with %d threads = %f\n", n, m, threads, time1);
	return floor(time1 + 0.5);
}

double ExecuteConsistent(int n, int m, int q, double *cons_mass)
{
	int i, j;
	double sum;
	LARGE_INTEGER cons_frequency, cons_start, cons_finish;

	sum = 0;
	QueryPerformanceFrequency(&cons_frequency);
	QueryPerformanceCounter(&cons_start);
#pragma omp single
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			cons_mass[i * m + j] *= q;
			sum += cons_mass[i * m + j];
		}
	}

	QueryPerformanceCounter(&cons_finish);
	printf("Summ: %f\n", sum);
	double time2 = (cons_finish.LowPart - cons_start.LowPart) * 1000.0f / cons_frequency.LowPart;
	printf("Consistent execution time for %dx%d matrix = %f\n", n, m, time2);
	return floor(time2 + 0.5);
	// ------------
}

int main()
{
	double *cons_mass, *parall_mass;
	double sum1, sum2;
	int n, m, threads, q = 0;

	printf("Multiplier: ");
	scanf_s("%d", &q);

	ofstream os("C:\\Data\\Stuff\\OpenMP\\CSVs\\task11.csv", ios::app);
	string path;
	int switchvar = 0;

	os << 4 << ";" << 12 << ";" << 20 << ";" << 28 << ";" << 36 << ";" << 44 << ";" << 1 << "\n";
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
		for (threads = 4; threads <= 44; threads += 8)
		{
			parall_mass = (double*)malloc(n * m * sizeof(double));
			ReadArray(parall_mass, n*m, path);
			double time1 = ExecuteParallel(n, m, q, threads, parall_mass);
			os << time1 << ";";
			free(parall_mass);
		}

		//Execute consistent
		cons_mass = (double*)malloc(n * m * sizeof(double));
		ReadArray(cons_mass, n*m, path);
		double time2 = ExecuteConsistent(n, m, q, cons_mass);
		os << time2 << "\n";
		free(cons_mass);
		switchvar++;
	}
	os << "\n";
	os.close();
}
