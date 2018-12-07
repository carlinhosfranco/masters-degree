#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <math.h>


#define SIZE 3072
#define nThreads 2
//#define SIZE 100
//#define nThreads 4
#define min(a,b) ((a)<(b)?(a):(b))

//Sequencial
int** floyd(int N, int **A) {
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i][j] = min(A[i][j], A[i][k] + A[k][j]);
			}
		}
	}
	return A;
}

//Inicializa as matrizes
void initMatrix(int firstRow, int numRows, int firstColumn, int numColumn, int ** A){

	for (int i = 0, ii = firstRow; i < numRows; i++, ii++){
		for (int j = 0, jj = firstColumn; j < numColumn; j++, jj++){
			A[i][j] = 100;
			if (((jj + 1) % (ii + 1)) == 0)
				A[i][j] = 1;
			if (((ii + 1) % (jj + 1)) == 0)
				A[i][j] = 1;
			if (ii == jj)
				A[i][j] = 0;
		}
	}
}

int** floyd2D(int numRows, int ** block, int * row, int myRank, double * commTime){
	int n = SIZE;
	double start_time = 0.0;
	
	for (int k = 0; k < n; k++){

		#pragma omp single
		{		

			if (myRank == 0)
				start_time = MPI_Wtime();
			
			int p = k / numRows;

			if (p == myRank){
				int kk = k % numRows;
				for (int i = 0; i < n; i++)
					row[i] = block[kk][i];
			}

			MPI_Bcast(row, n, MPI_INT, p, MPI_COMM_WORLD);
			*commTime += MPI_Wtime() - start_time;
		}

	#pragma omp for schedule(guided)
	for (int i = 0; i < numRows; i++){
		for (int j = 0; j < n; j++){
			block[i][j] = min(block[i][j], block[i][k] + row[j]);
		}
	}

	}
	return block;
}

int main(int argc, char** argv){
	double commTime;
	int n = SIZE;
	//MPI------------------------------------
	int myRank, nProc;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	//MPI------------------------------------

	int rowsPerProcesses = n / nProc;
	int firstRow = myRank * rowsPerProcesses;
	
	//OMP-------------
	omp_set_num_threads(nThreads);
	//OMP-------------
	
	//Alocação de memória-------------------
	int ** block = (int **) malloc(rowsPerProcesses * sizeof (int*));
	block[0] = (int *) malloc((rowsPerProcesses * n) * sizeof (int));
	for (int i = 1; i < rowsPerProcesses; i++)
		block[i] = block[i - 1] + n;
	
	int * row = (int *) malloc(n * sizeof (int));
	//Alocação de memória-------------------
	//Inicializa as Matrizes
	
	initMatrix(firstRow, rowsPerProcesses, 0, n, block);
	//---------------------
	floyd2D(rowsPerProcesses, block, row, myRank, &commTime);
	double totalTime = 0.0;
	double seconds = MPI_Wtime();
	int nIterations = 10;
	#pragma omp parallel shared(block, row, commTime)
	{
		int thread_id = omp_get_thread_num();
		floyd2D(rowsPerProcesses, block, row, myRank, &commTime);
		if (myRank == 0 && thread_id == 0){
			totalTime = MPI_Wtime();
			commTime = 0.0;
		}
	for (int i = 0; i < nIterations; i++){
		floyd2D(rowsPerProcesses, block, row, myRank, &commTime);
	}
	if (myRank == 0 && thread_id == 0){
		totalTime = MPI_Wtime() - seconds;
		printf("\n Matrix: %d \n Average Time: %1.6f \n Average Communication Time: %f\n", n,
		totalTime / nIterations; , commTime / nIterations;);
	}
	
	}
	
	free(row);
	free(block[0]);
	free(block);
	MPI_Finalize();
	return (EXIT_SUCCESS);
}