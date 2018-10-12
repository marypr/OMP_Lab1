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

double ExecuteParallel(int n, int m, int threads, char *a, double *b, double *c, double q)
{
	LARGE_INTEGER parall_frequency, parall_start, parall_finish;

	int i, j, chunk, counter1 = 0;

	chunk = n * 0.1;

	for (j = 0; j < m; j++)
	{
		c[j] = q;
	}

	QueryPerformanceFrequency(&parall_frequency);
	QueryPerformanceCounter(&parall_start);

#pragma omp parallel for private(i,j) schedule(dynamic,chunk) num_threads(threads)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			a[i] += (char)(b[i*m + j] * c[j]);
		}
	}
	QueryPerformanceCounter(&parall_finish);

	printf("Enter average value: %f\n", a[n - 1]);

	double time = (parall_finish.LowPart - parall_start.LowPart) * 1000.0f / parall_frequency.LowPart;
	printf("Execution time for %dx%d matrix with %d threads = %f\n", n, m, threads, time);
	return time;
}

double ExecuteConsistent(int n, int m, char *a, double *b, double *c, double q)
{
	LARGE_INTEGER cons_frequency, cons_start, cons_finish;

	int i, j, counter1 = 0;

	for (j = 0; j < m; j++)
	{
		c[j] = q;
	}

	QueryPerformanceFrequency(&cons_frequency);
	QueryPerformanceCounter(&cons_start);

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			a[i] += (char)(b[i*m + j] * c[j]);
		}
	}
	QueryPerformanceCounter(&cons_finish);

	printf("Enter average value: %f\n", a[n - 1]);

	double time = (cons_finish.LowPart - cons_start.LowPart) * 1000.0f / cons_frequency.LowPart;
	printf("Execution time for %dx%d matrix = %f\n", n, m, time);
	return time;
}


int main()
{
	LARGE_INTEGER frequency, start_p, finish_p;

	double q, *b, *c;
	char *a;
	int n, m, threads;

	printf("Enter vector value: ");
	scanf_s("%f", &q);

	ofstream os("C:\\Data\\Stuff\\OpenMP\\CSVs\\task16.csv", ios::app);
	string path;
	int switchvar = 0;

	os << 4 << ";" << 12 << ";" << 20 << ";" << 28 << ";" << 36 << ";" << 44 << ";" << 1 << "\n";

	while (switchvar <= 6)
	{
		switch (switchvar)
		{
		case(0): path = "C:\\Data\\Stuff\\OpenMP\\bin\\100.bin", n = 100, m = 100;
			break;
		case(1): path = "C:\\Data\\Stuff\\OpenMP\\bin\\1000.bin", n = 1000, m = 1000;
			break;
		case(2): path = "C:\\Data\\Stuff\\OpenMP\\bin\\2000.bin", n = 2000, m = 2000;
			break;
		case(3): path = "C:\\Data\\Stuff\\OpenMP\\bin\\4000.bin", n = 4000, m = 4000;
			break;
		case(4): path = "C:\\Data\\Stuff\\OpenMP\\bin\\6000.bin", n = 6000, m = 6000;
			break;
		case(5): path = "C:\\Data\\Stuff\\OpenMP\\bin\\8000.bin", n = 8000, m = 8000;
			break;
		case(6): path = "C:\\Data\\Stuff\\OpenMP\\bin\\10000.bin", n = 10000, m = 10000;
			break;
			//case(0): path = "D:\\Stuff\\OpenMP\\bin\\100.bin", n = 100, m = 100;
			//	break;
			//case(1): path = "D:\\Stuff\\OpenMP\\bin\\1000.bin", n = 1000, m = 1000;
			//	break;
			//case(2): path = "D:\\Stuff\\OpenMP\\bin\\2000.bin", n = 2000, m = 2000;
			//	break;
			//case(3): path = "D:\\Stuff\\OpenMP\\bin\\4000.bin", n = 4000, m = 4000;
			//	break;
			//case(4): path = "D:\\Stuff\\OpenMP\\bin\\6000.bin", n = 6000, m = 6000;
			//	break;
			//case(5): path = "D:\\Stuff\\OpenMP\\bin\\8000.bin", n = 8000, m = 8000;
			//	break;
			//case(6): path = "D:\\Stuff\\OpenMP\\bin\\10000.bin", n = 10000, m = 10000;
			//	break;
		}

		for (threads = 4; threads <= 44; threads += 8)
		{
			a = (char*)malloc(n * sizeof(char));
			b = (double*)malloc(m * n * sizeof(double));
			c = (double*)malloc(m * sizeof(double));
			ReadArray(b, n*m, path);
			double time = ExecuteParallel(n, m, threads, a, b, c, q);
			os << time << ";";
			free(a);
			free(b);
			free(c);
		}

		a = (char*)malloc(n * sizeof(char));
		b = (double*)malloc(m * n * sizeof(double));
		c = (double*)malloc(m * sizeof(double));
		ReadArray(b, n*m, path);
		double time = ExecuteConsistent(n, m, a, b, c, q);
		os << time << "\n";
		free(a);
		free(b);
		free(c);
		switchvar++;
	}
	os << "\n";
	os.close();
}