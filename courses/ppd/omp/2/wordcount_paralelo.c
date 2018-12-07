#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#define wordSize 30

struct palavras
{
	char palavra[wordSize];
	long unsigned int qtd;
};

double tempo()
{
				struct timeval tv;
				gettimeofday(&tv,0);
				return tv.tv_sec + tv.tv_usec/1e6;
}

int eh_delimitador(char c, char *delimitadores)
{
	int tamanho = strlen(delimitadores);
	int i = 0;

	for( i = 0; i < tamanho; i++)
	{
		if( c == delimitadores[i] || c == '\n' )
			return 1;
	}
	return 0;
}
//verifica o total de palavras
long unsigned int quantidade_palavras(char *texto, long unsigned int tamanho_texto, char *delimitadores)
{
	long unsigned int i = 0, qtd_palavras = 0;

	while( i < tamanho_texto )
	{
		if ((texto[i] >= 0x41) && (texto[i] <= 0x5A))
		{
				texto[i] |= 0x20;
		}
		if( eh_delimitador(texto[i], delimitadores) )
		{
			qtd_palavras += 1;
			while( eh_delimitador(texto[i], delimitadores) )
				i += 1;
		}
		else
		{
			i += 1;
		}
	}
	return qtd_palavras;
}

//cria uma lista de palavras
void adiciona_palavras(struct palavras *palavras, long unsigned int qtd_palavras ,char *texto, long unsigned int tamanho_texto, char *delimitadores)
{
	long unsigned int i = 0, j = 0, aux = 0;
	char palavra[wordSize];

	palavra[0] = '\0';

	while( i < tamanho_texto )
	{
		if ( eh_delimitador(texto[i], delimitadores) )
		{
			while( eh_delimitador(texto[i], delimitadores) && i < tamanho_texto)
				i += 1;

			if (strlen(palavra) > 0)
			{
				for(j = 0; j < qtd_palavras; j++)
				{
					if (palavras[j].qtd == -1 || !strcmp(palavras[j].palavra, palavra))
					{
						palavras[j].qtd = 0;
						strcpy(palavras[j].palavra, palavra);
						//printf("%lu  %s\n", j, palavra);
						break;
					}
				}
			}
			palavra[0] = '\0';
			aux = 0;
		}
		else
		{
			palavra[aux] = texto[i];
			aux += 1;
			palavra[aux] = '\0';
			i += 1;
		}
	}
}

main(int argc, char **argv)
{
	char *texto;
	struct palavras *palavras;
	char *delimitadores = ",. ";
	long unsigned int tamanho_texto = 0;
	long unsigned int i = 0;
	long unsigned int qtd_palavras = 0;
	long unsigned int qtd_palavras_distintas = 0;
	float endTime, startTime;
	int numero_threads = 1;
	double tempo_inicio, tempo_fim, media = 0;

omp_set_nested(1);

 if (argc < 3)
	{
		printf("\nErro: use %s <FILENAME> <NUM_THREADS>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else
		numero_threads = atoi( argv[2] );


	FILE *arquivo = fopen(argv[1], "rb");

	if (arquivo == NULL)
	{
		printf("\n\tErro ao ler %s!!\n", argv[1]);
		return 1;
	}

	//Faz a leitura do texto

	fseek(arquivo, 0, SEEK_END);
	tamanho_texto = ftell(arquivo);
	fseek(arquivo, 0, SEEK_SET);

	//Aloca em memória
	texto = (char *) malloc(tamanho_texto + 1);
	fread(texto, sizeof(char), tamanho_texto, arquivo);


	startTime = omp_get_wtime();
	qtd_palavras = quantidade_palavras(texto, tamanho_texto, delimitadores);


	palavras = (struct palavras *) malloc( qtd_palavras*sizeof(struct palavras) );
	for(i = 0; i < qtd_palavras; i++)
	{
		palavras[i].qtd = -1;
		palavras[i].palavra[0] = '\0';
	}



	adiciona_palavras(palavras, qtd_palavras, texto, tamanho_texto, delimitadores);

	for( i = qtd_palavras; i > 0 ; i--)
	{
		if( palavras[i-1].qtd != -1 )
		{
			qtd_palavras_distintas = i;
			break;
		}
	}
		omp_set_num_threads(numero_threads);

		//zera contagem palavras
		for(i = 0; i < qtd_palavras_distintas; i++)
		{
			palavras[i].qtd = 0;
		}


		#pragma omp parallel for private(i)
		for(i = 0; i < qtd_palavras_distintas; i++)
		{
			int tamanho_palavra;
			char *p_aux;

			tamanho_palavra = strlen( palavras[i].palavra );

			p_aux = texto;
		 
			while ( p_aux < texto + tamanho_texto && p_aux != NULL)
			{
				p_aux = strstr(p_aux, palavras[i].palavra);
				if (p_aux != NULL)
				{
					if ( ( eh_delimitador(*(p_aux-1), delimitadores) || p_aux == texto )
						 && eh_delimitador(*(p_aux+tamanho_palavra), delimitadores) )
					{
						palavras[i].qtd += 1;
						p_aux += tamanho_palavra;
					}
						else
						p_aux += 1;

				}
			}
		}

	endTime = omp_get_wtime();

	printf("\n\nTotal de palavras encontradas: %lu", qtd_palavras);
	printf("\nPalavras distintas encontradas: %lu", qtd_palavras_distintas);
	printf("\nNumero de threads: %d", numero_threads);
	printf("\nTempo médio: %f\n\n", endTime-startTime);

	free(texto);
	free(palavras);
	free(arquivo);
}
