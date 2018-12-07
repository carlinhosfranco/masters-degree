/*
Algoritmo de multiplicação de matrizes.

*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

main()
{
	int i,j,k,r,tamMatriz, manyRepeat;
	int ** matriz_1;
	int ** matriz_2;
	int ** matriz_3;
	double totalTime=0;
	clock_t startTime, endTime;

	srand( time(NULL));	

	printf("\nDigite o tamanho da matriz \n");
	scanf("%d", &tamMatriz);
	
	printf("Digite a quantidade de repetições: ");
	scanf("%d", &manyRepeat);

	for(r=0;r<manyRepeat;r++)
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
	
		//Preenchendo as matrizes
		for(i=0; i<tamMatriz; i++)
		{
			for(j=0;j<tamMatriz; j++)
			{
				matriz_1[i][j] = rand() %999;
				matriz_2[i][j] = rand() %999;	
				matriz_3[i][j] = 0;
			}
		}
	
		//Imprime as matrizes
/*
	for(i=0; i<tamMatriz; i++)
	{
		for(j=0; j<tamMatriz-1; j++)
		{
			printf(" %d |", matriz_1[i][j]);
		}
		printf(" %d \n", matriz_1[i][j]);
	} printf("\n");
*/
/*
	for(i=0; i<tamMatriz; i++ )
	{
		for(j=0; j<tamMatriz-1; j++)
		{
			printf(" %d |", matriz_2[i][j]);
		}	
		printf(" %d \n", matriz_2[i][j]);
	} printf("\n");
*/	
	//Calculando a matriz
	startTime = clock();
	printf("\nCalculando a matriz\n");
	
		for(i=0; i<tamMatriz; i++)
		{
			for(j=0; j<tamMatriz; j++)
			{
				for(k=0; k<tamMatriz; k++)
				{
					matriz_3[i][j] += matriz_1[i][k] * matriz_2[k][j];
				}
			}		
		}
	
	endTime = clock();	
	totalTime = totalTime+(endTime-startTime);
	free(matriz_1);
	free(matriz_2);
	free(matriz_3);	


	printf("\n--> Tempo Decorrido: %lf\n",((double)(endTime-startTime)/(CLOCKS_PER_SEC)));

	}

	printf("\n\n\nResultado Final");
	printf("\n--> Tempo  médio para %d execuções: %lf\n\n",manyRepeat, (((double)totalTime/manyRepeat)/(CLOCKS_PER_SEC)));


}
