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

#include "serialThree.c"

int main (int argc, char * argv[])
{
  int seed, A, B, P, n;

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
    //int n = atoi (argv[1]);
    seed = atoi (argv[1]);
    A    = atoi (argv[2]);
    B    = atoi (argv[3]);
    P    = atoi (argv[4]);
    n    = atoi (argv[5]);
  } // end get arguments from command line

  serial_baseline (seed, A, B, P, n);

  serial_matrix (seed, A, B, P, n);

  return 0;

}
