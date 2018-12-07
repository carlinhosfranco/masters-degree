# include <stdlib.h>
# include <stdio.h>

# include <omp.h>

int main ( int argc, char *argv[] );
int prime_default ( int n );
int prime_static ( int n );
int prime_dynamic ( int n );

/******************************************************************************/
int main ( int argc, char *argv[] )
/******************************************************************************/
{
  int n;
  int n_factor;
  int n_hi;
  int n_lo;
  int primes;
  double time1;
  double time2;
  double time3;

  printf ( "\n" );
  printf ( "  Versão Sequencial\n");
  printf ( "  Contagem dos números primos de 5 até 500.000\n" );
  printf ( "\n" );

  n_lo = 5;
  n_hi = 500000;
  n_factor = 10;

  printf ( "\n" );
  printf ( "                           Sequencial        \n" );
  printf ( "         N     Pi(N)          Tempo        \n" );
  printf ( "\n" );

  n = n_lo;

  while ( n <= n_hi )
  {
    time1 = omp_get_wtime ( );
    primes = prime_default ( n );
    time1 = omp_get_wtime ( ) - time1;

    printf ( "  %8d  %8d  %12f \n", n, primes, time1);

    n = n * n_factor;
  }
/*
  Terminate.
*/
  printf ( "\n" );
  printf ( "\n" );
  printf ( "  FIM :D\n" );

  return 0;
}
/******************************************************************************/
int prime_default ( int n )
/******************************************************************************/
{
  int i;
  int j;
  int prime;
  int total = 0;

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
