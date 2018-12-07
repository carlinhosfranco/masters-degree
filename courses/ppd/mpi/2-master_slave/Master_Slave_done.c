#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

double tempo(){
struct timeval tv;
gettimeofday(&tv,0);
return tv.tv_sec + tv.tv_usec/1e6;
}

void bs(int vectorSize, int *vectorSort)
{
	int i, j, aux;
  	for(i = 0; i < vectorSize - 1; i++)
  	{
    	for(j = i+1; j < vectorSize ; j++)
    	{
      		if(vectorSort[i] > vectorSort[j])
      		{
        		aux = vectorSort[i];
        		vectorSort[i] = vectorSort[j];
        		vectorSort[j] = aux;
        	}
    	}
  	}
}

int main(int argc, char **argv)
{
	//-----------------MPI
	int nWorkers,
		procN,
		myRank,
		dst=1,
		MASTER = 0,
		TAG,
		rankSource,
		workIndex;

		MPI_Status status;

	double endTime, startTime;
	//-----------------MPI		


	//-----------------Bubble Sort
	int **vectorSort, *vector;
	int i=0,
		j=0,
		vectorSize = 10000,
		qntVectors = 1000,
		tagKill = qntVectors;
	
	FILE *vectorIn;
	FILE *vectorOut;

	srand (time (NULL));

	//-----------------Bubble Sort


	MPI_Init (&argc , &argv);


	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &procN);

	nWorkers = procN-1;

	if ( myRank == MASTER)
	{
	
		vectorSort = (int **) malloc( qntVectors * sizeof (int *) );
		vector = (int *) malloc( vectorSize * sizeof (int));

		for (i=0; i<qntVectors; i++)
		{
			vectorSort[i] = malloc( vectorSize * sizeof (int) );
		}
	
		for (j=0; j<qntVectors; j++)
		{
			for (i=0; i < vectorSize; i++)
			{
				vectorSort[j][i] = vectorSize - i;
			}
		}
		

		//vectorIn = fopen("vetor_gerado.txt","w");
/*		for (j = 0; j < qntVectors; j++)
		{
			for (i = 0; i < vectorSize; i++)
			{
				//fprintf(vectorIn, "V[%d]P[%d][Gerado]-> %d\n",j,i, vectorSort[j][i]);
				printf("V[%d]P[%d][Gerado]-> %d\n",j,i, vectorSort[j][i]);
			}
		}*/
		//fclose(vectorIn);
		

		//Enviando vetores para os escravos
		
		startTime = tempo();
		dst=1;
		workIndex = 0;

		for (workIndex = 0; workIndex < nWorkers; workIndex++)
		{
			TAG = workIndex;

			MPI_Send(vectorSort[workIndex], vectorSize, MPI_INT, dst, TAG, MPI_COMM_WORLD);
			
			dst++;
												
		}

		for (j = workIndex; j < qntVectors; j++)
		{
			MPI_Recv(vector, vectorSize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			
			TAG = status.MPI_TAG;
			rankSource = status.MPI_SOURCE; //Último RANK Recebido
			
			for (i=0; i < vectorSize; i++)
				vectorSort[TAG][i] = vector[i]; //Armazena o vetor recebido

			TAG = j;
			
			MPI_Send(vectorSort[j], vectorSize, MPI_INT, rankSource, TAG, MPI_COMM_WORLD);
			
		}

		for (i = 0; i < workIndex; i++)
		{
			MPI_Recv(vector, vectorSize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			
			TAG = status.MPI_TAG;

			for (i=0; i < vectorSize; i++)
				vectorSort[TAG][i] = vector[i];

		}

		for (dst=1; dst < procN; dst++)
		{
			printf("Destino: %d\n", dst);
			MPI_Send (&tagKill, 1, MPI_INT, dst, tagKill, MPI_COMM_WORLD);
		}

		endTime = tempo();

		printf("Tempo de Execução: %f\n", endTime-startTime);

		//vectorOut = fopen("vetor_ordenado.txt","w");
		/*for (j=990; j <qntVectors; j++)
		{
			for (i=0; i < vectorSize; i++)
			{
				//fprintf(vectorOut, "V[%d]P[%d][Ordenado]-> %d\n", j,i, vectorSort[j][i]);
				printf("V[%d]P[%d][Ordenado]-> %d\n",j,i, vectorSort[j][i] );
			}
		}
		
		//fclose(vectorOut);*/

		printf("free vectors\n");

		for (i=0; i<qntVectors; i++)
			free(vectorSort[i]);

		free(vectorSort);
		free(vector);

		printf("master sleep 5\n");
		sleep(5);

		printf("master done\n");

	}
/*
		Processo Escravo
*/
	else
	{
		int *vectorSort;
		
		vectorSort = (int *) malloc( vectorSize * sizeof (int) );

		while(1)
		{
			
			MPI_Recv(vectorSort, vectorSize, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			TAG = status.MPI_TAG;
			
			int source = status.MPI_SOURCE;

			if (TAG != tagKill)
			{
								
				bs(vectorSize, vectorSort);
				MPI_Send(vectorSort, vectorSize, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
				
			}
			else
			{
				free(vectorSort);
				break;
			}
									
		}
		printf("slave %d done\n", myRank);

	}
	
	MPI_Finalize();
	return 0;
}