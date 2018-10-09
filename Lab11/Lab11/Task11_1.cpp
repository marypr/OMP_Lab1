#include "pch.h"

//#include <stdio.h>
//#include <malloc.h>
//#include <stdlib.h>
//#include <omp.h>
//#include <iostream>
//#include <fstream>
//#include <windows.h>
//#include <locale.h>
//
//using namespace std;
//
//void ReadArray(double* pdata, size_t nm, const char* file_path)
//{
//	ifstream is(file_path, ios::binary | ios::in);
//	if (!is.is_open())
//		return;
//	is.read((char*)(pdata), nm * sizeof(double));
//	is.close();
//}
//
//int main()
//{
//	setlocale(0, "RUS");
//
//	LARGE_INTEGER cons_frequency, cons_start, cons_finish;
//	LARGE_INTEGER parall_frequency, parall_start, parall_finish;
//	double *cons_mass, *parall_mass;
//	double sum1, sum2;
//	int i, j, n, m, q = 0;
//
//	printf("Enter rows number: 5000\n");
//	//scanf_s("%d", &n);
//	n = 5000;
//	printf("Enter cols number: 5000\n");
//	//scanf_s("%d", &m);
//	m = 5000;
//
//	printf("Multiplier: ");
//	scanf_s("%d", &q);
//
//	parall_mass = (double*)malloc(n * m * sizeof(double));
//	cons_mass = (double*)malloc(n * m * sizeof(double));
//
//	ReadArray(parall_mass, n*m, "C:\\Users\\nniki\\source\\repos\\Univ\\5000.bin");
//	ReadArray(cons_mass, n*m, "C:\\Users\\nniki\\source\\repos\\Univ\\5000.bin");
//
//	// --------
//	sum1 = 0;
//	QueryPerformanceFrequency(&parall_frequency);
//	QueryPerformanceCounter(&parall_start);
//
//#pragma omp parallel for private(i,j) reduction(+:sum1) num_threads(30)
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < m; j++)
//		{
//			parall_mass[i * m + j] *= q;
//			sum1 += parall_mass[i * m + j];
//		}
//	}
//
//	QueryPerformanceCounter(&parall_finish);
//	printf("Summ: %f\n", sum1);
//	double time1 = (parall_finish.LowPart - parall_start.LowPart) * 1000.0f / parall_frequency.LowPart;
//	printf("Parallel execution time = %f\n", time1);
//	free(parall_mass);
//	// ===========
//
//	// -----------
//	sum2 = 0;
//	QueryPerformanceFrequency(&cons_frequency);
//	QueryPerformanceCounter(&cons_start);
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < m; j++)
//		{
//			cons_mass[i * m + j] *= q;
//			sum2 += cons_mass[i * m + j];
//		}
//	}
//
//	QueryPerformanceCounter(&cons_finish);
//	printf("Summ: %f\n", sum2);
//	double time2 = (cons_finish.LowPart - cons_start.LowPart) * 1000.0f / cons_frequency.LowPart;
//	printf("Consistent execution time = %f\n", time2);
//	free(cons_mass);
//	// ------------
//}