#include <stdlib.h> 
#include <stdio.h>
#include "mpi.h"
#include <string.h>

void imprime(int *nVector, int n);
void bubble_sort(int *list, int n);

//----------Controle dos nVectores
int *nVector,
	sizeVector, 
	partVector=0,
	startVector,
	endVector,
	i=0,
	stop,
	valueNeighbor,
	change;

	FILE *vectorIN;
	FILE *vectorOUT;

double startTime, endTime;
//----------Controle dos nVectores

//----------MPI
int	procN,
	myRank,
	TAG=0;

int main(int argc, char *argv[])
{
	
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &procN);

	MPI_Status status;

	sizeVector = 800000;
	
	partVector = sizeVector / procN;
	change = (partVector * 0.20);
	

	int sizeAditional = partVector + change;
	int contador = 0;
	
	nVector = (int*) malloc(sizeAditional * sizeof(int));
	int statusVector[procN];
	
	startVector = sizeVector - (myRank * partVector);
	
	srand (time (NULL));

	//inicializa o vetor
	for (i=0; i<partVector; i++)
	{
		nVector[i] = startVector - i;
	}
	//fim


/*	vectorIN = fopen("vetor_gerado.txt","w");
	for (i=0; i<partVector; i++)
	{
		printf("[Rank %d gerado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
		fprintf(vectorIN, "[Rank %d gerado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
	}
	fclose(vectorIN); */

/*	char aux[256]; 
	sprintf (aux,"%d", myRank);
	strcat (aux, "_.txt");
	vectorIN = fopen(aux,"w");

	for (i=0; i<partVector; i++)
	{
		//printf("[Rank %d gerado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
		fprintf(vectorIN, "[Rank %d gerado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
	}
	fclose(vectorIN);*/



	if (myRank == 0)
	{
		startTime = MPI_Wtime();
	}

	stop = 0;
	while(stop != 1)
	{
		for (i=0; i<procN; i++)
			statusVector[i]=0;	

		bubble_sort(&nVector[0], partVector);

		if (myRank>0)
		{
//			printf("Meu ID: %d vou eviar %d para %d\n", myRank,nVector[0], myRank-1 );
			MPI_Send(&nVector[0], change, MPI_INT, myRank-1, TAG, MPI_COMM_WORLD );
		}
			
			
		if (myRank != procN-1)
		{
			MPI_Recv(&nVector[partVector], change, MPI_INT, myRank+1, TAG, MPI_COMM_WORLD, &status);
				
//				printf("[%d] > [%d]\n", nVector[partVector-1], nVector[partVector]);
				if (nVector[partVector-1]>nVector[partVector])
				{
					statusVector[myRank] = 1;
				}				
		}

		for (i=0; i < procN; i++)
			MPI_Bcast(&statusVector[i], 1, MPI_INT, i,MPI_COMM_WORLD);		
		
		stop = 1;
		for (i = 0; i < procN; i++)
		{
			if (statusVector[i] == 1)
			{
				stop = 0;				
			}

		}

		if (stop == 0)
		{
			bubble_sort(&nVector[partVector-change], change*2);
	
			if (myRank != procN-1)
				MPI_Send(&nVector[partVector], change, MPI_INT, myRank+1, TAG, MPI_COMM_WORLD);
			if (myRank>0)
				MPI_Recv(&nVector[0], change, MPI_INT, myRank-1, TAG, MPI_COMM_WORLD, &status);
		}
		contador++;
		
	}		

/*	vectorOUT = fopen("vetor_ordenado.txt","w");
	for (i = 0; i < partVector; i++)
	{
			fprintf(vectorOUT, "[Rank %d ordenado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
			printf("[ordenado]Posicao[%d] Valor[%d]\n", i, nVector[i] );
	}
	fclose(vectorOUT);*/
   
	if (myRank == 0)
	{
		endTime = MPI_Wtime();

		printf("Tempo de execução: %f\n", endTime-startTime);
		printf("Interações: %d\n", contador);
		printf("Numero de processos: %d\n", procN);

	}

/*	char aux2[256]; 
	sprintf (aux2,"%d", myRank);
	strcat (aux2, ".txt");
	vectorOUT = fopen(aux2,"w");
	for (i=0; i<partVector; i++)
	{
		//printf("[Rank %d gerado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
		fprintf(vectorOUT, "[Rank %d ordenado]Posicao[%d] Valor[%d]\n",myRank, i, nVector[i] );
	}
	fclose(vectorOUT);*/


	MPI_Finalize();
	free(nVector);
	return 0;
}


void imprime(int *nVector, int partVector){
    int i;
    for(i=0;i<partVector;i++){
        printf("[%d]\t",nVector[i]);
        
    }
    printf("\n");
}

void bubble_sort(int *list, int n)
{
  long c, d, t;

  for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (list[d] > list[d+1])
      {
        t         = list[d];
        list[d]   = list[d+1];
        list[d+1] = t;
      }
    }
  }
}
