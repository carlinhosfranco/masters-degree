README

Passos para executar o algoritmo de multiplicação de matrizes.

	Sequencial:
	1 - No terminal digite:
        gcc sequencial.c -fopenmp -o sequencial  //NOTA: É utilizado a flag -fopenmp para utilizar a função de coleta do tempo de execução.

	Paralelo:
	1- No terminal digite:
	   gcc parallel_matrix.c -fopenmp -o parallel_matrix


Passos para executar o algoritmo de calculo de números primos

	Sequencial
	1 - No terminal digite:
		gcc prime_seq.c -fopenmp -o prime_seq

	Paralelo
	1 - No terminal digite:
		gcc prime_parallel.c -fopenmp -o prime_parallel

	2 - É necessário utilizar a variável de ambiente para setar a quantidade de threads. Para isso, digite no terminal:
		export OMP_NUM_THREADS=[NUMERO DE THREADS (2,4,6,8)]
