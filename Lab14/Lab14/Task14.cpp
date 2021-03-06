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

double ExecuteParallel(int n, int m, int el, int threads, double *parall_mass)
{
	LARGE_INTEGER parall_frequency, parall_start, parall_finish;

	int i, j, chunk, count1 = 0;
	chunk = n * 0.1;
	QueryPerformanceFrequency(&parall_frequency);
	QueryPerformanceCounter(&parall_start);

	//#pragma omp parallel shared(parall_mass) private(i, j) num_threads(threads)
	//	{
	//#pragma omp for reduction(+:count1) schedule(dynamic, chunk) nowait
#pragma omp parallel for private(i) schedule(dynamic, threads) reduction(+:count1)
	for (i = 0; i < n; i++)
	{
		if (parall_mass[i*m + el] == 0)
		{
			//printf("\nindex = %d, elem_value = %f, rownum = %d\n", i*m + el, parall_mass[i*m + el], n);
			count1++;
		}
	}
	//}

	QueryPerformanceCounter(&parall_finish);

	printf("Elements quantity: %d\n", count1);

	double time = (parall_finish.LowPart - parall_start.LowPart) * 1000.0f / parall_frequency.LowPart;
	printf("Parallel execution time for %dx%d matrix with %d threads = %f\n", n, m, threads, time);
	return time;
}

double ExecuteConsistent(int n, int m, int el, double *cons_mass)
{
	LARGE_INTEGER cons_frequency, cons_start, cons_finish;
	int i, count2 = 0;

	QueryPerformanceFrequency(&cons_frequency);
	QueryPerformanceCounter(&cons_start);
#pragma omp single
	for (i = 0; i < n; i++)
	{
		if (cons_mass[i*m + el] == 0)
		{
			count2++;
		}
	}
	QueryPerformanceCounter(&cons_finish);

	printf("Counter value: %d\n", count2);
	double time2 = (cons_finish.LowPart - cons_start.LowPart) * 1000.0f / cons_frequency.LowPart;
	printf("Consistent execution time for %dx%d matrix = %f\n", n, m, time2);
	return time2;
}

int main()
{
	double *parall_mass, *cons_mass;
	int n, m, threads, el;

	printf("Enter element number in row: ");
	scanf_s("%d", &el);

	ofstream os("D:\\Stuff\\OpenMP\\CSVs\\task14.csv", ios::app);
	string path;
	int switchvar = 0;

	os << 4 << "," << 12 << "," << 20 << "," << 28 << "," << 36 << "," << 44 << "," << 1 << "\n";

	while (switchvar <= 6) {
		switch (switchvar)
		{
			switchvar = 1;
			//case(0): path = "C:\\Data\\Stuff\\OpenMP\\bin\\100.bin", n = 100, m = 100;
			//	if (el < m) break; else switchvar++;
			//case(1): path = "C:\\Data\\Stuff\\OpenMP\\bin\\1000.bin", n = 1000, m = 1000;
			//	if (el < m) break; else switchvar++;
			//case(2): path = "C:\\Data\\Stuff\\OpenMP\\bin\\2000.bin", n = 2000, m = 2000;
			//	if (el < m) break; else switchvar++;
			//case(3): path = "C:\\Data\\Stuff\\OpenMP\\bin\\4000.bin", n = 4000, m = 4000;
			//	if (el < m) break; else switchvar++;
			//case(4): path = "C:\\Data\\Stuff\\OpenMP\\bin\\6000.bin", n = 6000, m = 6000;
			//	if (el < m) break; else switchvar++;
			//case(5): path = "C:\\Data\\Stuff\\OpenMP\\bin\\8000.bin", n = 8000, m = 8000;
			//	if (el < m) break; else switchvar++;
			//case(6): path = "C:\\Data\\Stuff\\OpenMP\\bin\\10000.bin", n = 10000, m = 10000;
			//	if (el < m) break; else throw new exception("Element is out row");
		case(0): path = ":\\Stuff\\OpenMP\\bin\\100.bin", n = 100, m = 100;
			if (el < m) break; else switchvar++;
		case(1): path = "D:\\Stuff\\OpenMP\\bin\\1000.bin", n = 1000, m = 1000;
			if (el < m) break; else switchvar++;
		case(2): path = "D:\\Stuff\\OpenMP\\bin\\2000.bin", n = 2000, m = 2000;
			if (el < m) break; else switchvar++;
		case(3): path = "D:\\Stuff\\OpenMP\\bin\\4000.bin", n = 4000, m = 4000;
			if (el < m) break; else switchvar++;
		case(4): path = "D:\\Stuff\\OpenMP\\bin\\6000.bin", n = 6000, m = 6000;
			if (el < m) break; else switchvar++;
		case(5): path = "D:\\Stuff\\OpenMP\\bin\\8000.bin", n = 8000, m = 8000;
			if (el < m) break; else switchvar++;
		case(6): path = "D:\\Stuff\\OpenMP\\bin\\10000.bin", n = 10000, m = 10000;
			if (el < m) break; else switchvar++;
		}

		//Execute parallel
		for (threads = 4; threads <= 44; threads += 8)
		{
			parall_mass = (double*)malloc(n * m * sizeof(double));
			ReadArray(parall_mass, n*m, path);
			double time1 = ExecuteParallel(n, m, el, threads, parall_mass);
			os << time1 << ",";
			free(parall_mass);
		}

		//Execute consistent
		cons_mass = (double*)malloc(n * m * sizeof(double));
		ReadArray(cons_mass, n*m, path);
		double time2 = ExecuteConsistent(n, m, el, cons_mass);
		os << time2 << "\n";
		free(cons_mass);
		switchvar++;
	}
	os << "\n";
	os.close();
}