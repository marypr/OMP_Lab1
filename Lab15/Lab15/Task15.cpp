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

double ExecuteParallel(int n, int m, int avg, int threads, double *parall_mass1, double *parall_mass2)
{
	LARGE_INTEGER parall_frequency, parall_start, parall_finish;
	int i, j, count1 = 0, chunk = avg;
	double sum1 = 0;

	QueryPerformanceFrequency(&parall_frequency);
	QueryPerformanceCounter(&parall_start);

#pragma omp parallel for private(i) reduction(+:count1) schedule(dynamic,chunk) num_threads(threads)
	for (i = 0; i < n*m; i++)
	{
		if (i%avg == avg - 1) {
			sum1 += parall_mass1[i];
			parall_mass2[(i + 1) / avg - 1] = sum1;
			sum1 = 0;
			count1++;
		}
		else
		{
			sum1 += parall_mass1[i];
		}
	}

	QueryPerformanceCounter(&parall_finish);

	printf("Number of average values: %d\n", count1);
	double time1 = (parall_finish.LowPart - parall_start.LowPart) * 1000.0f / parall_frequency.LowPart;
	printf("Parallel execution time for %d threads = %f\n", threads, time1);
	return time1;
}

double ExecuteConsistent(int n, int m, int avg, double *cons_mass1, double *cons_mass2)
{
	LARGE_INTEGER cons_frequency, cons_start, cons_finish;
	int  i, j, count2 = 0;
	double sum2 = 0;


	QueryPerformanceFrequency(&cons_frequency);
	QueryPerformanceCounter(&cons_start);

	for (i = 0; i < n*m; i++)
	{
		if (i%avg == avg - 1) {
			sum2 += cons_mass1[i];
			cons_mass2[(i + 1) / avg - 1] = sum2;
			sum2 = 0;
			count2++;
		}
		else
		{
			sum2 += cons_mass1[i];
		}
	}
	QueryPerformanceCounter(&cons_finish);

	printf("Number of average values : %d\n", count2);
	double time2 = (cons_finish.LowPart - cons_start.LowPart) * 1000.0f / cons_frequency.LowPart;
	printf("Consistent execution time = %f\n", time2);
	return time2;
}

int main()
{
	double *parall_mass1, *parall_mass2, *cons_mass1, *cons_mass2;
	int n, m, threads, avg;

	printf("Enter number of elements: ");
	scanf_s("%d", &avg);

	ofstream os("C:\\Data\\Stuff\\OpenMP\\CSVs\\task15.csv", ios::app);
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
			//case(0): path = "d:\\stuff\\openmp\\bin\\100.bin", n = 100, m = 100;
			//	break;
			//case(1): path = "d:\\stuff\\openmp\\bin\\1000.bin", n = 1000, m = 1000;
			//	break;
			//case(2): path = "d:\\stuff\\openmp\\bin\\2000.bin", n = 2000, m = 2000;
			//	break;
			//case(3): path = "d:\\stuff\\openmp\\bin\\4000.bin", n = 4000, m = 4000;
			//	break;
			//case(4): path = "d:\\stuff\\openmp\\bin\\6000.bin", n = 6000, m = 6000;
			//	break;
			//case(5): path = "d:\\stuff\\openmp\\bin\\8000.bin", n = 8000, m = 8000;
			//	break;
			//case(6): path = "d:\\stuff\\openmp\\bin\\10000.bin", n = 10000, m = 10000;
			//	break;
		}

		for (threads = 4; threads <= 44; threads += 8) {
			parall_mass1 = (double*)malloc(n * m * sizeof(double));
			ReadArray(parall_mass1, n*m, path);
			parall_mass2 = (double*)calloc(ceil((n*m) / avg), sizeof(double));
			double time1 = ExecuteParallel(n, m, avg, threads, parall_mass1, parall_mass2);
			os << time1 << ";";
			free(parall_mass1);
			free(parall_mass2);
		}

		// Execute consistent	
		cons_mass1 = (double*)malloc(n * m * sizeof(double));
		ReadArray(cons_mass1, n*m, path);
		cons_mass2 = (double*)calloc(ceil((n*m) / avg), sizeof(double));
		double time2 = ExecuteConsistent(n, m, avg, cons_mass1, cons_mass2);
		os << time2 << "\n";
		free(cons_mass1);
		free(cons_mass2);
		switchvar++;
	}
	os << "\n";
	os.close();
}