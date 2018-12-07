#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

#define vectorSize 800000

void bs(int n, int * vetor);
void merge(int a[], int m, int b[], int n, int sorted[]);
int *interleaving(int vetor[], int tam);


int main(int argc, char** argv){
	int myRank;
	int procN;
	int i, j, dest;
	int TAG;
	int *vectorN;
	int sizeVectorFolhas;
	int *vectorAUX;
	int halfVector;
	int DELTA;
	
	double startTime, endTime;

	MPI_Status status;
	MPI_Status masterStatus;

	TAG=1;
	halfVector = vectorSize / 2;
        srand(time(NULL));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &procN);

	vectorN = (int *) malloc(sizeof(int) * vectorSize);

	DELTA = vectorSize / ((procN + 1) / 2);
	printf("DELTA %d\n", DELTA);
	
	

	if (myRank == 0)
	{		
		sizeVectorFolhas = vectorSize;	
		
		for(i=0; i<vectorSize; i++)
		{
			vectorN[i] = vectorSize -i;
		}
		
	}

	else
	{ 	
		MPI_Recv(vectorN, vectorSize, MPI_INT, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &masterStatus);
		MPI_Get_count(&masterStatus,MPI_INT,&sizeVectorFolhas);
	}
	
	if(sizeVectorFolhas <= DELTA)
	{	
		bs(sizeVectorFolhas, vectorN);	
	}
	else
	{

		halfVector = sizeVectorFolhas / 2;		
		
		MPI_Send(&vectorN[0], halfVector, MPI_INT, myRank * 2 +1, TAG, MPI_COMM_WORLD); 
		MPI_Send(&vectorN[halfVector], halfVector, MPI_INT, myRank * 2 +2, TAG, MPI_COMM_WORLD); 
		
		MPI_Recv(&vectorN[0], halfVector, MPI_INT, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(&vectorN[halfVector], halfVector, MPI_INT, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
		
		vectorN = interleaving(vectorN, vectorSize);

	}
	if(myRank != 0)
	{

		MPI_Send(vectorN, sizeVectorFolhas, MPI_INT, masterStatus.MPI_SOURCE, TAG, MPI_COMM_WORLD); 
	}

	MPI_Finalize();
	free(vectorN);
	
	printf("=>FIM: %d\n", myRank);


	return 0;
}


int *interleaving(int vetor[], int tam){
	int *vectorAUXiliar;
	int i1, i2, i_aux;

	vectorAUXiliar = (int *)malloc(sizeof(int) * tam);

	i1 = 0;
	i2 = tam / 2;

	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
		    || (i2 == tam))
			vectorAUXiliar[i_aux] = vetor[i1++];
		else
			vectorAUXiliar[i_aux] = vetor[i2++];
	}
	
	free(vetor);

	return vectorAUXiliar;
}

void bs(int n, int * vetor){
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou ){
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++){
            if (vetor[d] > vetor[d+1]){
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
             }
	}
        c++;
    }
}