#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

#define N 50000000
#define PI 3.1415926535
#define DELTA .01415926535

double tempo(){
struct timeval tv;
gettimeofday(&tv,0);
return tv.tv_sec + tv.tv_usec/1e6;
}

int main (int argc, char *argv[])
{
        int nThreads, tid, i;
        float *a, *b;
        double endTime, startTime;

        a = (float*) malloc( N * sizeof (float) );
        b = (float*) malloc( N * sizeof (float) );

        printf("Thread %d starting...\n", tid);

        printf("Thread %d initializing a[]\n", tid);

        startTime = tempo();

        for (i=0; i<N; i++)
        {
                //a[i] = ((i*DELTA)*(i*DELTA))*((i*DELTA)*(i*DELTA));
                a[i] = i * DELTA;
 		}

        printf("Thread %d adding a[] to b[]\n", tid);

        for (i=0; i<N; i++)
        {
                b[i] += a[i];
        }

        printf("Thread %d initializing b[]\n", tid);

                for (i=0; i<N; i++);
                {
//                      b[i] = ((i*PI)*(i*PI))*((i*PI)*(i*PI));
                        b[i] = i * PI;
                }

        printf("Thread %d adding b[] to a[]\n",tid);

                for (i=0; i<N; i++)
                {
                        a[i] += b[i];
                }
        endTime = tempo();

        printf("Tempo sequencial: %f\n", endTime-startTime);

}
