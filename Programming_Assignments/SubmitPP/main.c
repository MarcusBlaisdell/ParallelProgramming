/*****************************
*   Programming Assignment 3
*   Cpt_S 411
*   Marcus Blaisdell
*   Tyler Scheffler
*
*   This program is built off
*   of base code from
*   Marcus' Programming Project 2
*
******************************/

#include "three.c"

int main (int argc, char * argv[])
{
  int rank, p;
  int seed, A, B, P, n;
  int l2 = 0;
  struct timeval t1, t2;

  if (argc < 6)
  {
    printf ("*** Error ***\n");
    printf ("You entered %d arguments\n", argc);
    printf ("usage is \"main <seed> <A> <B> <P> <n>\"\n");
    printf ("Where <seed> is the seed value\n");
    printf ("      <A> & <B> are constants\n");
    printf ("  and <P> is the modulus value\n");
    printf ("*  Note: <A> and <B> should be prime numbers\n");
    printf ("   I recommend A = 107, B = 139\n");
    printf ("** Note: <P> should be a very large prime number\n");
    printf ("         I would suggest 10007\n");
    printf ("n is the number of random numbers you require\n");
    printf ("\n");

    return 0;
  }
  else if (argc == 6)
  {
    seed = atoi (argv[1]);
    A    = atoi (argv[2]);
    B    = atoi (argv[3]);
    P    = atoi (argv[4]);
    n    = atoi (argv[5]);
  } // end get arguments from command line

    // Initialize the MPI process and get the values
    // for rank and number of processes:

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &p);


    // test parallelPrefix:

    // If testing n_parallelPrefix / p_parallelPrefix,
    // uncomment the next four lines:

  //int Aa[16] = {2, 4, 2, 1, 3, 1, 4, 5, 1, 2, 3, 4, 2, 6, 5, 7};
  //n = sizeof(Aa) / sizeof(Aa[0]); // temporarily override n:

  //p_parallelPrefix (Aa, &rank, &p, 1, &l2);
  //n_parallelPrefix (Aa, &rank, &p, n);

    // test parallel random number generator:

  p_RNG (seed, A, B, P, n, &rank, &p);

  gettimeofday (&t1, NULL);
  MPI_Finalize ();
  gettimeofday (&t2, NULL);

  printf ("%d, ", (t2.tv_usec - t1.tv_usec));

}
