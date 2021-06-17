#include "three.h"

// function Operator:
// Accepts two, (2x2) matrices
// and the P value
// and returns the mod P product
// as a 2x2 matrix:

void Operator (int One[], int Two[], int * Product, int P)
{

  Product[0] = (((One[0] * Two[0]) + (One[1] * Two[2])) % P);
  Product[1] = (((One[0] * Two[1]) + (One[1] * Two[3])) % P);
  Product[2] = (((One[2] * Two[0]) + (One[3] * Two[2])) % P);
  Product[3] = (((One[2] * Two[1]) + (One[3] * Two[3])) % P);


} // end Operator function


  // function p_parallelPrefix:
  // accepts:
  // an array of numbers to be summed,
  // the ranks of all processes
  // the number of available processes
  // outputs the cumulative sums of the
  // input array
  // assumes n = p and that p divides n
  // to allow for "normal" use and use
  // with n_parallelPrefix, we use the
  // flag variable,
  // if 0, initialize local to its value from A,
  // if 1, initialize local to 0

int p_parallelPrefix (int A[], int *rank, int *p, int flag, int *l2)
{
  int i = 0;

    // in the parallel Prefix, n = p:

  int n = *p;

    // t will be used for the for loop:

  int t = 0;

    // let lgp = lg of (p - 1):

  int lgp = ceil(log2(*p));

    // initialization:
    // create and set our local and global variables:

  int l = 0;
  int g = 0;

  if (flag == 0)
  {
    l = A[*rank];
    g = l;
  } // end if flag == 0
  else if (flag == 1)
  {
    l = 0;
    g = A[*rank];
  } // end if flag == 1

  int g_remote = 0;
  int partner = 0;

  MPI_Status status;

    // Now, perform the operations:

    // loop lg(p-1) times:

  for (t = 0; t < lgp; t++)
  {
    partner = findPartner (*rank, t, lgp);
      // send g to partner:
    MPI_Send (&g, 1, MPI_INT, partner, t, MPI_COMM_WORLD);
      // receive g_remote from partner
    MPI_Recv (&g_remote, 1, MPI_INT, partner, t, MPI_COMM_WORLD, &status);


    if (partner < *rank)
    {
      l = l + g_remote;
    } // end if partner less than our Rank

    g = g + g_remote;

  } // end for loop

  *l2 = l;

  return 1;

} // end function p_parallelPrefix


// function pm_parallelPrefix:
// This is a modification of the p_parallelPrefix
// for matrices
// Matrices are repsented as 4-element arrays as follows:
// [0 1]
// [2 3]
// is represented as: A{0, 1, 2, 3}
//
// accepts:
// a Matrix that is the start point,
// the ranks of all processes
// the number of available processes
// outputs the cumulative sums of the
// input array
// assumes n = p and that p divides n
// to allow for "normal" use and use
// with n_parallelPrefix, we use the
// flag variable,
// if 0, initialize local to its value from A,
// if 1, initialize local to 0
// Matrix A is M_local

int pm_parallelPrefix (int *A, int *rank, int *p, int flag, int *M_off, int P)
{
  int i = 0;

    // in the p-element parallel Prefix, n = p:

  int n = *p;

    // t will be used for the for loop:

  int t = 0;

    // let lgp = lg of (p - 1):

  int lgp = ceil(log2(*p));

    // initialization:
    // create and set our local and global variables:
    // Things get a bit tricky here:
    // I can't pass a struct through MPI_Send/MPI_Recv
    // so, I will convert the matrices to 4-element
    // int arrays for processing, and convert back
    // after they are computed:
    // so, the 2x2 matrix:
    // [a  c]
    // [b  d]
    // will be: [a  c  b  d]:
    // [0,0] is [0]
    // [0,1] is [1]
    // [1,0] is [2]
    // [1,1] is [3]

  int l[4];
  int g[4];
  int g_remote[4];
  int product[4];

  if (flag == 0)
  {
    l[0] = g[0] = A[0];
    l[1] = g[1] = A[1];
    l[2] = g[2] = A[2];
    l[3] = g[3] = A[3];

  } // end if flag == 0
  else if (flag == 1)
  {
    // initialize l to the identity matrix:

    l[0] = l[3] = 1;
    l[1] = l[2] = 0;

    g[0] = A[0];
    g[1] = A[1];
    g[2] = A[2];
    g[3] = A[3];
  } // end if flag == 1

  g_remote[0] = g_remote[1] = g_remote[2] = g_remote[3] = 0;

  int partner = 0;

  MPI_Status status;

    // Now, perform the operations:


    // loop lg(p-1) times:

  for (t = 0; t < lgp; t++)
  {
    partner = findPartner (*rank, t, lgp);
      // send g to partner:
    MPI_Send (&g, 4, MPI_INT, partner, t, MPI_COMM_WORLD);
      // receive g_remote from partner
    MPI_Recv (&g_remote, 4, MPI_INT, partner, t, MPI_COMM_WORLD, &status);

    if (partner < *rank)
    {

      Operator (l, g_remote, product, P);
      l[0] = product[0];
      l[1] = product[1];
      l[2] = product[2];
      l[3] = product[3];

    } // end if partner less than our Rank

    Operator (g, g_remote, product, P);
    g[0] = product[0];
    g[1] = product[1];
    g[2] = product[2];
    g[3] = product[3];

  } // end for loop

    // set the offset to the local value:

  M_off[0] = l[0];
  M_off[1] = l[1];
  M_off[2] = l[2];
  M_off[3] = l[3];

} // end function pm_parallelPrefix

  // n_parallelPrefix:
  // n-element parallel Prefix

int n_parallelPrefix (int A[], int *rank, int *p, int n)
{
    // s is the local sum:

  int s  = 0;
  int i  = 0, j = 0;
  int x[*p];
  int y  = 0;
  int l2 = 0;
  int l3[n];
  MPI_Status status;

    // the sub-array size is n/p,
    // this is the size of the array for each process:

  int subn = n / *p;

    // use subn to create an array of local variables:

  int l[subn];

    // load our local l with the values
    //  that correspond to this process:
    // the start point of the sub-array for each process
    // is the process (rank * n) / p,
    // or subn * rank for this case
    // (since n/p is pre-calculated)

  for (i = 0; i < subn; i++)
  {
    l[i] = A[((*rank) * subn) + i];

  } // end for loop

    // calculate the sum:

  for (i = 0; i < subn; i++)
  {
    s = s + l[i];
  } // end get local sum

    // Now we need to populate our x array:
    // we know the sum at our current position:

  x[*rank] = s;

    // now we need to send our sum
    // to all the other processes:

  for (i = 0; i < *p; i++)
  {
    if (i != *rank)
    {
      MPI_Send (&s, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    } // end only send to other ranks

  } // end send to all processes

    // Now we need to receive the sums
    // from all the other processes

  for (i = 0; i < *p; i++)
  {
    if (i != *rank)
    {
      MPI_Recv (&y, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
      x[i] = y;
    } // end only send to other ranks

  } // end receive from all processes

    // x array should now contain all local sums
    // from all processes

      // uncomment the next block to display output to the slurm file:
    /*
    printf ("%d - x: ", *rank);
    for (i = 0; i < *p; i++)
    {
      printf ("%d - ", x[i]);
    }  // end for loop to print the array
    printf ("\n");
    */

      // call p_parallelPrefix with x array:
      // use flag 1 so it initializes the
      // local variables to 0:

    p_parallelPrefix (x, rank, p, 1, &l2);

    printf ("n_parallel l2: %d\n", l2);

      // compute the local sums with offset:
      // first, add the offset to the first
      // value:

    l[0] += l2;

      // and compute sums for the remaining values:

    for (i = 1; i < subn; i++)
    {
      l[i] += l[i - 1];

    } // end add offset to local array

      // Now, send our local array to every process
      // to build the full array, in order:

    int y2[subn];

    for (i = 0; i < *p; i++)
    {
      if (i != *rank)
      {
        MPI_Send (&l, subn, MPI_INT, i, 4, MPI_COMM_WORLD);

        MPI_Recv (&y2, subn, MPI_INT, i, 4, MPI_COMM_WORLD, &status);

        for (j = 0; j < subn; j++)
        {
          l3[(i * subn) + j] = y2[j];

        } // end put the variables in order

      } // end send

    } // end send to other processes

      // receive from other processes:
      // and assemble the local values in order:

    for (i = 0; i < subn; i++)
    {
      l3[((*rank) * subn) + i] = l[i];

    } // end populate our values


      // print the full local array from the last process:

    if (*rank == (*p - 1))
    {
      printf ("*rank: %d - l = ", *rank);
      for (i = 0; i < n; i++)
      {
        printf ("%d : ", l3[i]);
      } // end print l3
      printf ("\n");
    } // end print the local array


} // end function n_parallelPrefix


  // serial_matrix function:

int serial_matrix (int seed, int A, int B, int P, int n, int *rank, int *p, int * M_off)
{
  int i = 0, j = 0, k = 0;
  int subn = (n/(*p));

    // create an array to hold the entire sequence:

  int RNG_sequence[n];

    // use MPI to build the full sequence on all processes:

  MPI_Status status;

    // receive local arrays:

  int Y[subn];

    // create a matrix, X, of size subn:

  int X[subn];

    // create our matrix M:

  int M[4];
  int product[4];

    // set the matrix to: [A 0]
    //                    [B 1]:

  M[0] = A;
  M[1] = 0;
  M[2] = B;
  M[3] = 1;

    // create variable Mnext, initialize it to M_off:

  int Mnext[4];
  Mnext[0] = M_off[0];
  Mnext[1] = M_off[1];
  Mnext[2] = M_off[2];
  Mnext[3] = M_off[3];

    // General form: X[i] = (cA) + B) % P
    // [seed  1] * [A  0]
    //             [B  1]
    // (((seed * A) + (1 * B)) % P)

    // Mnext[0] = ((Mnext[0] * Mnext[0]) % P)
    // Mnext[1] = 0 = Mnext[1] i.e. no need to update
    // Mnext[2] = (((Mnext[2] * Mnext[0]) + Mnext[2]) % P)
    // Mnext[3] = 1 = Mnext[3] i.e. no need to update

  for (i = 0; i < subn; i++)
  {
    X[i] = (((seed * Mnext[0]) + (Mnext[2])) % P);
    Operator (Mnext, M, product, P);
    Mnext[0] = product[0];
    Mnext[1] = product[1];
    Mnext[2] = product[2];
    Mnext[3] = product[3];

  } // end for loop

    // uncomment the following block to print output to slurm file:
  /*
  printf ("%d - X: ", *rank);
  for (i = 0; i < (n/(*p)); i++)
  {
    printf ("%d : ", X[i]);
  } // end for loop

  printf ("\n");
  */

  // add our numbers to the master sequence:

  j = (*rank) * subn;

  for (k = 0; k < subn; k++)
  {
    RNG_sequence[j + k] = X[k];

  } // end add sequence to collective

    // now we need to send our local sequence
    // to all the other processes:

  for (i = 0; i < *p; i++)
  {
    if (i != *rank)
    {
      MPI_Send (&X, subn, MPI_INT, i, 2, MPI_COMM_WORLD);
    } // end only send to other ranks

  } // end send to all processes

    // Now we need to receive the sequences
    // from all the other processes

  for (i = 0; i < *p; i++)
  {
    if (i != *rank)
    {
      MPI_Recv (&Y, subn, MPI_INT, i, 2, MPI_COMM_WORLD, &status);

      j = (i) * subn;

      for (k = 0; k < subn; k++)
      {
        RNG_sequence[j + k] = Y[k];

      } // end add sequence to collective

    } // end only send to other ranks

  } // end receive from all processes

    // RNG_sequence array should now contain the full sequence
    // from all processes

      // uncomment the following block to write the full sequence
      // to the slurm file:
    /*
    if (*rank == 0)
    {
      printf ("%d - Sequence: ", *rank);
      for (i = 0; i < n; i++)
      {
        printf ("%d : ", RNG_sequence[i]);

      } // end for loop

      printf ("\n");

    } // only print the full sequence from one process
    */

    // uncomment the next block to write the output to a file:
    /*
    if (*rank == 0)
    {
      FILE  *writeFile;
      writeFile = fopen("paralleloutput.txt", "w");

      for (i = 0; i< n; i++)
      {
        fprintf (writeFile, "%d\n", RNG_sequence[i]);

      } // end write output to file

      fclose (writeFile);

    } // end write output to file
    */

  return 1;

} // end serial_matrix function


  // function p_RNG
  // function to generate random numbers
  // in parallel using the Linear
  // congruential form:
  // accepts:
  //         initial seed value,
  //         constants: A, B, P
  //         size of data, n

int p_RNG (int seed, int A, int B, int P, int n, int *rank, int *p)
{
  int i = 0;
  int subn = (n/(*p));

  // Step 1: load input parameters {X[0], A, B, P, n}
  // on all processes
  // By calling this function and passing the parameters
  // as arguments, this is accomplished

  // Step 2: At each process P_j:
  //      a) Initialize matrix M and M_0:
  //      b) Initialize: Array X_local of size n/p
  //         with each element of a 2x2 matrix:
  //         - init: X_local = [M  M  ...  M]^((n/p)-1)

  // a):

  // create our matrix M:

  int M[4];
  int product[4];

    // set the matrix to: [A 0]
    //                    [B 1]:

  M[0] = A;
  M[1] = 0;
  M[2] = B;
  M[3] = 1;

  // create our matrix M_0:

  int M_0[4];

    // set the matrix to: [1 0]
    //                    [0 1]:

  M_0[0] = 1;
  M_0[1] = 0;
  M_0[2] = 0;
  M_0[3] = 1;

  // b):

  // create X_local of size n/p:
  // This will need to be an array of structs
  // that hold our 2x2 matrices:

  int *X_local[subn];

  // Initialize X_local to contain all M's:

  for (i = 0; i < (n/(*p)); i++)
  {
    X_local[i] = M;
  } // end initialize X_local

  // Step 3: At each process Pj:
  // M_local = M_0;

  int M_local[4];
  M_local[0] = M_0[0];
  M_local[1] = M_0[1];
  M_local[2] = M_0[2];
  M_local[3] = M_0[3];

  for (i = 0; i < ((n/(*p))); i++)
  {
    Operator(M_local, X_local[i], product, P);
    M_local[0] = product[0];
    M_local[1] = product[1];
    M_local[2] = product[2];
    M_local[3] = product[3];

  } // end for loop

  // Step 4:
  // Run P_element_PP with each process
  // providing its corresponding M_local
  // as input, and (X) as the matrix
  // multiplication operator
  // output will be a 2x2 matrix, M_off,
  // which represents the prefix matrix product:
  // M_off at process P_j will be equal to:
  // M j(n/*p)

  int M_off[4];

    // pm_parallelPrefix will calculate the offsets:

  pm_parallelPrefix (M_local, rank, p, 1, M_off, P);

  // Step 5:
  // At every process Pj:
  // - call serial_matrix (n/p), with
  //   two modifications:
  //   - Initialize its M_next <- M_off
  //   - Run its for loop from 0 to ((n/p) - 1)
  // Output X from every process
  //printf ("M_off.M_Prime[0][0]: %d\n", M_off.M_Prime[0][0]);

  serial_matrix (seed, A, B, P, n, rank, p, M_off);

} // end p_RNG function

  // Function findPartner,
  // This function finds a partner process
  // to a given process at a given time step:
  // input: a process number,
  // a time step / position number,
  // a total number of time steps
  // returns a partner process as an int

int findPartner (int theProcess, int thePos, int theTotal)
{
    // initilialize to -1 so we
    // can detect if it fails:

  int thePartner = -1;
  int theBit = 0;
  int loopCount = theTotal;
  int exponent = theTotal - 1;
  int theEvalVal = 0;
  int i = 0;

  // First, convert to a binary array:
  // the number of elements in the array is
  // theTotal number of time steps

  int myBinary[theTotal];

    // to convert to binary,

    // start at MSB and work our way down

  for (i = 0; i < loopCount; i++)
  {
      // get the decimal value of the
      // position in the binary expression:

    theEvalVal = pow(2, exponent);

    if (theProcess >= theEvalVal)
    {
      myBinary[(loopCount - 1) - i] = 1;

      // decrement our number by this value:

      theProcess -= theEvalVal;

    } // end if
    else
    {
      myBinary[(loopCount - 1) - i] = 0;
    } // end set to 0

      // move the the next MSB:

    exponent--;

  } // end for loop

    // Now, flip the bit in the given position:
    // add 1 to whatever we have, mod the sum by 2,
    // and we will have the flipped binary value,
    // (0 + 1) % 2 = 1,
    // (1 + 1) % 2 = 0

  myBinary[thePos] = (myBinary[thePos] + 1) % 2;

  // Now, convert from binary to decimal and return
  // this as our partner process:

  thePartner = 0;

  for (i = 0; i < loopCount; i ++)
  {
    if (myBinary[i] == 1)
    {
      // add 2^i to our running sum:

      thePartner += pow(2, i);
    } // end if current position is a 1

  } // end for loop, convert binary to decimal

  return thePartner;

} // end function findPartner
