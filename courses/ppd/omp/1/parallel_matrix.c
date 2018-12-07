 /*
Algoritmo de multiplicação de matrizes.

*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

main()
{
	int i,j,r,k,tamMatriz, numThreads, manyRepeat=0, chuckSize, resto;
	int ** matriz_1;
	int ** matriz_2;
	int ** matriz_3;
	double endTime, startTime, totalTime=0;
	
	srand( time(NULL));	

	printf("\nDigite o tamanho da matriz: ");
	scanf("%d", &tamMatriz);
	
	printf("\nDigite a quantidade de threads: ");
	scanf("%d", &numThreads);

	printf("Digite quantas repetições: ");
	scanf("%d", &manyRepeat);
	
	chuckSize = tamMatriz/numThreads;

	printf("ChuckSize = %d\n", chuckSize);
	
	resto = chuckSize%numThreads;
	printf("resto: %d\n", resto);

	if ( resto != 0 )

		resto = tamMatriz-((tamMatriz/numThreads)*numThreads);
		chuckSize = chuckSize+resto;
	printf("ChuckSize = %d\n", chuckSize);

	for(r=1;r<=manyRepeat;r++)
	{
		
		matriz_1 = malloc( tamMatriz * sizeof (int *) );
		matriz_2 = malloc( tamMatriz * sizeof (int *) );
		matriz_3 = malloc( tamMatriz * sizeof (int *) );


		//Alocando as matrizes
		for (i=0; i<tamMatriz; i++)
		{
			matriz_1[i] = malloc( tamMatriz * sizeof (int) );
			matriz_2[i] = malloc( tamMatriz * sizeof (int) );
			matriz_3[i] = malloc( tamMatriz * sizeof (int) );
		}
	
	//Calculando a matriz
		printf("\nCalculando a matriz---\n");
	
		startTime = omp_get_wtime();
		#pragma omp parallel num_threads(numThreads) shared(matriz_1,matriz_2,matriz_3) firstprivate(tamMatriz) private(i,j,k)	
		{
//		#pragma omp for schedule (dynamic) 
		#pragma omp for schedule (dynamic,chuckSize) nowait
		for(i=0; i<tamMatriz; i++)
		{
			for(j=0; j<tamMatriz; j++)
			{
				matriz_1[i][j] = rand() %999;
				matriz_2[i][j] = rand() %999;
				matriz_3[i][j] = 0;

				for(k=0; k<tamMatriz; k++)
				{
					matriz_3[i][j] += matriz_1[i][k] * matriz_2[k][j];
				}
			}		
		}
		}
		endTime = omp_get_wtime();
		printf("Tempo gasto: %f\n", endTime-startTime);
		totalTime = totalTime+(endTime-startTime);
		
		free(matriz_1);
		free(matriz_2);
		free(matriz_3);
	}
	
	printf("\n\n--> Tempo Médio para %d execuções em %d threads: %f\n\n",manyRepeat,numThreads,totalTime/manyRepeat);
}
