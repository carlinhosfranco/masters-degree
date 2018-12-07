# include <stdlib.h>
# include <stdio.h>
# include <omp.h>

int chuckSize = 200;

int main ( int argc, char *argv[] );
int prime_default ( int n );
int prime_static ( int n );
int prime_dynamic ( int n );
int prime_guided  (int n);

/******************************************************************************/
int main ( int argc, char *argv[] )
/******************************************************************************/

{
  int n;
  int n_factor;
  int n_hi, th;
  int n_lo;
  int primes;
  double time1;
  double time2;
  double time3;
  double time4;

  printf ( "\n" );
  printf ( "  Versão Paralela\n" );
  printf ( "  Contagem dos números primos de 5 até 500.000\n" );
  printf ( "  Escalonamento utilizados: default, static, dynamic e guided\n" );
  printf ( "\n" );
  printf ( "  Número de processadores dispovíveis = %d\n", omp_get_num_procs ( )  );
  printf ( "  Número de threds utilizadas =              %d\n", omp_get_max_threads ( )  );


  n_lo = 5;
  n_hi = 500000;
  n_factor = 10;

  printf ( "\n" );
  printf ( "                           Default        Static       Dynamic       Guided\n" );
  printf ( "         N     Pi(N)         Tempo         Tempo         Tempo        Tempo\n" );
  printf ( "\n" );

  n = n_lo;

  while ( n <= n_hi )
  {
    time1 = omp_get_wtime ( );
    primes = prime_default ( n );
    time1 = omp_get_wtime ( ) - time1;

    time2 = omp_get_wtime ( );
    primes = prime_static ( n );
    time2 = omp_get_wtime ( ) - time2;

    time3 = omp_get_wtime ( );
    primes = prime_dynamic ( n );
    time3 = omp_get_wtime ( ) - time3;

	time4 = omp_get_wtime ( );
    primes = prime_guided ( n );
    time4 = omp_get_wtime ( ) - time4;

    printf ( "  %8d  %8d  %12f  %12f  %12f  %12f\n", n, primes, time1, time2, time3, time4 );

    n = n * n_factor;
  }
/*
  FIM
*/
  printf ( "\n" );
  printf ( "\n" );
  printf ( "  FIM :D\n" );

  return 0;
}

int prime_default ( int n )

{
  int i;
  int j;
  int prime;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, prime )

# pragma omp for reduction ( + : total )
  for ( i = 2; i <= n; i++ )
  {
    prime = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        prime = 0;
        break;
      }
    }
    total = total + prime;
  }

  return total;
}

int prime_static ( int n )

{
  int i;
  int j;
  int prime;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, prime )

# pragma omp for reduction ( + : total ) schedule ( static, chuckSize )
  for ( i = 2; i <= n; i++ )
  {
    prime = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        prime = 0;
        break;
      }
    }
    total = total + prime;
  }

  return total;
}


int prime_dynamic ( int n )

{
  int i;
  int j;
  int prime;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, prime )

# pragma omp for reduction ( + : total ) schedule ( dynamic, chuckSize )
  for ( i = 2; i <= n; i++ )
  {
    prime = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        prime = 0;
        break;
      }
    }
    total = total + prime;
  }

  return total;
}

int prime_guided ( int n )

{
  int i;
  int j;
  int prime;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, prime )

# pragma omp for reduction ( + : total ) schedule ( guided, chuckSize )
  for ( i = 2; i <= n; i++ )
  {
    prime = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        prime = 0;
        break;
      }
    }
    total = total + prime;
  }

  return total;
}
