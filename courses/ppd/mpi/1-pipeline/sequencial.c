#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	//---------------MPI
	int i,
		j,
		myRank, 
		procN, 
		source, 
		tag=1, 
		dest, 
		nWorkers;
		

		FILE *filePrime;
		FILE *fileVector;
	//---------------MPI
	
	//---------------Siede de Arethosthenes
	int sizeVector = 100000, 	//tamanho do vetor a ser alocado
	*vectorN, 					// vetor que irá armazenar os dados
	primoCont = 0;
	//---------------Siede de Arethosthenes


	srand (time (NULL));
	
	vectorN = (int*) malloc( sizeVector * sizeof (int) );

	for (j = 1; j < 11; j++)
	{
	
			fileVector = fopen("vetor_gerado.txt","w");
		

			for (i=0; i<sizeVector; i++)
			{
				vectorN[i] = (rand() %100)+2;
				fprintf(fileVector, "%i\n", vectorN[i]);		
			}
				
			fclose(fileVector);


			for (i=0; i<sizeVector; i++)
				{				
					if ( vectorN[i] != 2 && (vectorN[i] % 2) == 0 )
					{
						vectorN[i] = 0;
					}
				}
			
			for (i=0; i<sizeVector; i++)
				{				
					if ( vectorN[i] != 3 && (vectorN[i] % 3) == 0 )
					{
						vectorN[i] = 0;
					}
				}

			for (i=0; i<sizeVector; i++)
				{				
					if ( vectorN[i] != 5 && (vectorN[i] % 5) == 0 )
					{
						vectorN[i] = 0;
					}
				}

			for (i=0; i<sizeVector; i++)
				{				
					if ( vectorN[i] != 7 && (vectorN[i] % 7) == 0 )
					{
						vectorN[i] = 0;
					}
				}

			filePrime = fopen("primos.txt","w");
				
				for (i=0; i<sizeVector; i++)
				{
					
					if (vectorN[i] != 0)
					{
						primoCont++;
						fprintf(filePrime, "%i\n", vectorN[i]);
							
					}
				}
			
			printf("%d\n", primoCont);
			fclose(filePrime);
			primoCont = 0;
			
	}
	free(vectorN);
}