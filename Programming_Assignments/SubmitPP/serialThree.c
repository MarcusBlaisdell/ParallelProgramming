#include "serialThree.h"


  // serial baseline:
  // serial_baseline function

int serial_baseline (int seed, int A, int B, int P, int n)
{
    // initialize:

  int x[n];
  int i = 0;

  x[0] = seed;

    // loop and create random numbers;

  //for (i = 1; i < (n - 1); i++)
  for (i = 1; i < n; i++)
  {
    x[i] = ((A * (x[i - 1]) + B) % P);
  } // end for loop

  /*
  printf ("x: ");
  for (i = 0; i < n; i++)
  {
    printf ("%d : ", x[i]);
  } // end for loop
  printf ("\n");
  */

  FILE *writeFile;
  writeFile = fopen ("serial_baselineoutput.txt", "w");

  for (i = 0; i < n; i++)
  {
    fprintf (writeFile, "%d\n", x[i]);
  } // end write sequence to file

  fclose (writeFile);

  return 1;

} // end serial_baseline function


  // serial_matrix function:

int serial_matrix (int seed, int A, int B, int P, int n)
{
  int i = 0, j = 0;

    // create a matrix, X, of size n:

  int X[n];

    // the provided seed value is our zeroeth element:

  X[0] = seed;

    // create our matrix M:

  int M[2][2];

    // set the matrix to: [A 0]
    //                    [B 1]:

  M[0][0] = A;
  M[0][1] = 0;
  M[1][0] = B;
  M[1][1] = 1;

    // create variable Mnext, initialize it to M:

  int Mnext[2][2];

    // copy M to Mnext:

  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 2; j++)
    {
      Mnext[i][j] = M[i][j];
    } // end inner loop
  } // end outer loop

  //for (i = 1; i < n - 1; i++)
  for (i = 1; i < n; i++)
  {
    X[i] = (Mnext[0][0] * X[0] + Mnext[1][0]) % P;
    Mnext[0][0] = (Mnext[0][0] * M[0][0]) % P;
    Mnext[1][0] = ((Mnext[1][0] * M[0][0]) + M[1][0]) % P;

  } // end for loop

  /*
  printf ("X: ");
  for (i = 0; i < n; i++)
  {
    printf ("%d : ", X[i]);
  } // end for loop
  printf ("\n");
  */

  FILE *writeFile;
  writeFile = fopen ("serial_matrixoutput.txt", "w");

  for (i = 0; i < n; i++)
  {
    fprintf (writeFile, "%d\n", X[i]);
  } // end write sequence to file

  fclose (writeFile);

  return 1;

} // end serial_matrix function
