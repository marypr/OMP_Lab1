#include "pch.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

void ReadArray(double* pdata, size_t nm, const char* file_path)
{
	ifstream is(file_path, ios::binary | ios::in);
	if (!is.is_open())
		return;
	is.read((char*)(pdata), nm * sizeof(double));
	is.close();
}

int main()
{
	
	LARGE_INTEGER frequency, start_p, finish_p;

	double *pulled_elems;
	char *parall_mass2;
	double *cons_mass1;
	char *cons_mass2;

	double sum = 0;
	int i, j, n, m, threads, chunk, min, max;

	setlocale(LC_ALL, "Rus");
	n = 10000, m = 10000;
	double *parall_mass1 = (double*)malloc(n * m * sizeof(double));

	double *b = (double*)malloc(n*m * sizeof(double));
	ReadArray(b, n*m, "C:\\Data\\Stuff\\OpenMP\\bin\\10000.bin");
	printf("%f %f %f %f %f %f %f %f \n\n\n", b[65454], b[8454], b[5454], b[654], b[454], b[9999994], b[54], b[6453]);
	ReadArray(parall_mass1, n*m, "C:\\Data\\Stuff\\OpenMP\\bin\\10000.bin");
	printf("%f %f %f %f %f %f %f %f", parall_mass1[65454], parall_mass1[8454], parall_mass1[5454], parall_mass1[654], parall_mass1[454], parall_mass1[9999994], parall_mass1[54], parall_mass1[6453]);


	
	printf("Enter rows number: 5000");
	//scanf_s("%d", &n);
	printf("\nEnter cols number: 5000");
	//scanf_s("%d", &m);	
	printf("Enter min element: ");
	scanf_s("%d", &min);
	printf("\nEnter max element: ");
	scanf_s("%d", &max);

	ofstream os("C:\\lab13_100.csv", ios::app);
	//saveCSVthreads("C:\\lab13_100.csv");
	os << "\n\n";

	

	for (threads = 4; threads <= 44; threads += 8) {
		parall_mass1 = (double*)malloc(n * m * sizeof(double));
		ReadArray(parall_mass1, n*m, "‪C:\\Data\\Stuff\\OpenMP\\bin\\10000.bin");
		

		int count1 = 0;
		chunk = n * 0.1;
		parall_mass2 = (char*)calloc(n * m, sizeof(char));

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_p);

#pragma omp parallel for private(i,j) reduction(+:count1) schedule(dynamic,chunk) num_threads(threads)
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				if (parall_mass1[i*m + j] >= min && parall_mass1[i*m + j] <= max)
				{
					//parall_mass2[i*m + j] = (char)parall_mass1[i*m + j];
					parall_mass2[i*m + j] = 1;
					count1++;
				}
				//else
				//{
				//	parall_mass2[i*m + j] = RAND_MAX + 1;
				//}
			}
		}

		QueryPerformanceCounter(&finish_p);

		j = 0;
		
			printf("%d", count1);
			//if (parall_mass2[i] != RAND_MAX + 1)
			//{
			//	parall_mass2[j] = parall_mass2[i];
			//	j++;
			//}

			for (i = 6543453; i < 6543553; i++)
			{
				printf("%f", parall_mass1[i]);
			}

		pulled_elems = (double*)malloc(count1 * sizeof(double));

		int elems_count = 0;
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				if (parall_mass2[i*m + j] = 1)
				{
					pulled_elems[elems_count] = parall_mass1[i*m + j];
					elems_count++;
				}
			}
		}

		printf("%d %f %f %f", elems_count, pulled_elems[3], pulled_elems[5], pulled_elems[8]);

		printf("Elements quantity: %d\n", count1);

		double time = (finish_p.LowPart - start_p.LowPart) * 1000.0f / frequency.LowPart;
		printf("Parallel execution time for %d threads = %f\n", threads, time);

		os << time << ";";

		free(parall_mass1);
		free(parall_mass2);
	}

	int count2 = 0;
	cons_mass1 = (double*)malloc(n * m * sizeof(double));
	ReadArray(cons_mass1, n*m, "C:\\Users\\nniki\\source\\repos\\Univ\\5000.bin");
	cons_mass2 = (char*)calloc(n * m, sizeof(char));

	 //-----------
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if (cons_mass1[i*m + j] >= min && cons_mass1[i*m + j] <= max)
			{
				cons_mass2[i*m + j] = (char)cons_mass1[i*m + j];
				count2++;
			}
			else
			{
				cons_mass2[i*m + j] = RAND_MAX + 1;
			}
		}
	}

	QueryPerformanceCounter(&finish_p);
	j = 0;
	for (i = 0; i < n*m; i++)
	{
		if (cons_mass2[i] != RAND_MAX + 1)
		{
			cons_mass2[j] = cons_mass2[i];
			j++;
		}
	}
	cons_mass2 = (char*)realloc(cons_mass2, count2 * sizeof(char));
	printf("Elements quantity: %d\n", count2);

	double time = (finish_p.LowPart - start_p.LowPart) * 1000.0f / frequency.LowPart;
	printf("Consistent execution time = %f\n", time);
	free(cons_mass1);
	free(cons_mass2);
	 //------------

	os << "\n";
	os.close();
}