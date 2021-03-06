#include "pch.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <locale.h>

using namespace std;

void saveArr(const double* pdata, size_t nm, const char* file_path)
{
	ofstream os(file_path, ios::binary | ios::out);
	if (!os.is_open())
		return;
	os.write((const char*)(pdata), nm * sizeof(double));
	os.close();
}

void loadArr(double* pdata, size_t nm, const char* file_path)
{
	ifstream is(file_path, ios::binary | ios::in);
	if (!is.is_open())
		return;
	is.read((char*)(pdata), nm * sizeof(double));
	is.close();
}

void saveCSVthreads(const char *file_path)
{
	ofstream os(file_path, ios::app);
	if (!os.is_open())
		return;
	for (int i = 5;i <= 50;i += 5)
		os << i << ";";
	os << "\n";
	os.close();
}

double random(double min, double max)
{
	return min + (double)(rand()) / RAND_MAX * max;
}

int main()
{
	setlocale(0, "RUS");

	LARGE_INTEGER cons_frequency, cons_start_p, cons_finish_p;
	LARGE_INTEGER parall_frequency, parall_start_p, parall_finish_p;

	double *cons_mass, *parall_mass;
	double sum1, sum2;
	int i, j, n, m, ths, chunk, q = 0;

	printf("Enter rows number: ");
	scanf_s("%d", &n);
	printf("Enter cols number: ");
	scanf_s("%d", &m);
	/*printf("Введите количество потоков: ");
	scanf_s("%d", &ths);*/

	printf("Multiplier: ");
	scanf_s("%d", &q);

	//a = (double*)malloc(n * m * sizeof(double));
	//int count = 0;
	//for (i = 0; i < n; i++)
	//{
	//	for (j = 0; j < m; j++)
	//	{
	//		a[i * m + j] = random(-100, 100);
	//		count++;
	//	}
	//}


	//saveArr(a, n*m, "D:\\Stuff\\5000.bin");
	//saveArr(a, n*m, "C:\\100.bin");
	//saveArr(a, n*m, "D:\\1000.bin");
	//saveArr(a, n*m, "C:\\2000.bin");
	//saveArr(a, n*m, "C:\\4000.bin");
	//saveArr(a, n*m, "C:\\6000.bin");
	//saveArr(a, n*m, "C:\\8000.bin");
	//saveArr(a, n*m, "C:\\10000.bin");

	//ofstream os("C:\\lab11_100.csv", ios::app);
	//saveCSVthreads("C:\\lab11_100.csv");
	//os << "\n\n";
	//for (ths = 5; ths <= 50; ths += 5) {
	parall_mass = (double*)malloc(n * m * sizeof(double));
	cons_mass = (double*)malloc(n * m * sizeof(double));


	//loadArr(a, n*m, "C:\\100.bin");
	//loadArr(a, n*m, "C:\\1000.bin");
	//loadArr(a, n*m, "C:\\2000.bin");
	//loadArr(a, n*m, "C:\\4000.bin");
	//loadArr(a, n*m, "C:\\6000.bin");
	//loadArr(a, n*m, "C:\\8000.bin");
	loadArr(parall_mass, n*m, "D:\\Stuff\\5000.bin");
	loadArr(cons_mass, n*m, "D:\\Stuff\\5000.bin");

	// --------
	sum1 = 0;
	//chunk = n * 0.1;

	QueryPerformanceFrequency(&parall_frequency);
	QueryPerformanceCounter(&parall_start_p);

#pragma omp parallel for private(i,j) reduction(+:sum) /*schedule(dynamic,chunk) num_threads(ths)*/
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			parall_mass[i * m + j] *= q;
			sum1 += parall_mass[i * m + j];
		}
	}

	QueryPerformanceCounter(&parall_finish_p);

	printf("Summ: %f\n", sum1);

	double time = (parall_finish_p.LowPart - parall_start_p.LowPart) * 1000.0f / parall_frequency.LowPart;
	printf("Parallel execution time = %f\n", time);

	//os << time << ";";
	free(parall_mass);

	// -----------

	// ===========
	sum2 = 0;

	//chunk = n * 0.1;

	QueryPerformanceFrequency(&cons_frequency);
	QueryPerformanceCounter(&cons_start_p);
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			cons_mass[i * m + j] *= q;
			sum2 += cons_mass[i * m + j];
		}
	}	

	QueryPerformanceCounter(&cons_finish_p);

	printf("Summ: %f\n", sum2);

	time = (cons_finish_p.LowPart - cons_start_p.LowPart) * 1000.0f / cons_frequency.LowPart;
	printf("Consistent execution time = %f\n", time);
	//os << time << ";";
	free(cons_mass);

	// ===========


//}
//	os << "\n";
//	os.close();
}