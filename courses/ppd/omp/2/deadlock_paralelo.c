#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

#define N 50000000
#define PI 3.1415926535
#define DELTA .01415926535

int main (int argc, char *argv[])
{
		int nThreads, tid, i;
		float *a, *b;
		double endTime, startTime;

		a = (float*) malloc( N * sizeof (float) );
		b = (float*) malloc( N * sizeof (float) );

		omp_set_nested(1);

		omp_lock_t locka, lockb;
		omp_init_lock(&locka);
		omp_init_lock(&lockb);

		startTime = omp_get_wtime();
		#pragma omp parallel shared(a, b, nThreads, locka, lockb) private(tid)  num_threads(nThreads)
		{
				tid = omp_get_thread_num();
				#pragma omp master
				{
						nThreads = omp_get_num_threads();
						printf("Number of threads = %d\n", nThreads);
				}
				printf("Thread %d starting...\n", tid);
		  #pragma omp barrier
				#pragma omp sections nowait
				{
						#pragma omp section
						{
								printf("Thread %d initializing a[]\n",tid);
								omp_set_lock(&locka);
								omp_set_lock(&lockb);

								#pragma omp parallel for private(i) num_threads(nThreads)
								for (i=0; i<N; i++)
								{
										//a[i] = ((i*DELTA)*(i*DELTA))*((i*DELTA)*(i*DELTA));
										a[i] = i * DELTA;
								}
								printf("Thread %d adding a[] to b[]\n",tid);

								#pragma omp parallel for num_threads(nThreads) private (i)
								for (i=0; i<N; i++)
								{
										b[i] += a[i];
								}

								omp_unset_lock(&lockb);
								omp_unset_lock(&locka);
						}

						#pragma omp section
						{
								printf("Thread %d initializing b[]\n",tid);
								omp_set_lock(&lockb);
								omp_set_lock(&locka);
								#pragma omp parallel for private(i)  num_threads(nThreads)
								for (i=0; i<N; i++)
								{
										//b[i] = ((i*PI)*(i*PI))*((i*PI)*(i*PI));
										b[i] = i * PI;
								}
								printf("Thread %d adding b[] to a[]\n",tid);
								#pragma omp parallel for private(i)  num_threads(nThreads)
								for (i=0; i<N; i++)
								{
										a[i] += b[i];
								}

								omp_unset_lock(&locka);
								omp_unset_lock(&lockb);

						}
				}
		}
		free (a);
		free (b);
		endTime = omp_get_wtime();
		printf("Tempo Paralelo com %d: %f\n", nThreads, endTime-startTime);
}