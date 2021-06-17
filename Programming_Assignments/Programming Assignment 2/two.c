#include "two.h"

  // function genArray:
  // accepts:
  // a pointer to an integer array,
  // a number of elements, n,
  // a process rank


void GenerateArray(int theArray[], int n, int theRank)
{
  int i = 0;

  // seed the random number generator:
  // Use the rank to create the illusion
  // of randomness by giving each process
  // a different seed and therefore
  // different random numbers:

  srand (theRank);

    // populate the array with random
    // numbers of single-digit integers
    // to keep things relatively small:

  for (i = 0; i < n; i++)
  {
    // use mod 10 to limit numbers
    // to single digits:

    theArray[i] = rand() % 10;

  } // end for loop

} // end function GenerateArray

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
    //

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

  // maxTime function

int maxTime (int timeArray[], int *p)
{
  int i = 0;
  int max = 0;

  for (i = 0; i < *p; i++)
   {
     //printf ("timeArray[%d] = %d\n", i, timeArray[i]);
     if (timeArray[i] > max)
     {
       max = timeArray[i];
       
     } // end store max

   } // end find max

   return max;

} // end maxTime function

  // function MyReduce

int MyReduce (int *rank, int *p, int n)
{

      // declare variables:

    MPI_Status status;
    int i = 0, j = 0, sum = 0, runTime = 0;
    int t = log2(*p); // number of time steps to accomplish reduce
    struct timeval t1, t2;

    //printf ("rank: %d - p: %d\n", (*rank), (*p));

      // start here:
      // we will accomplish all of this in discrete time steps
      // of lg(p) (t)
      // we need to determine the partner process to send/receive
      // to/from by using the time step to flip the binary value
      // in position t of the binary number of our process
      // i.e. at t = 0, 000 will partner with 001,
      // at t = 1, 001 will partner with 011, etc.

    gettimeofday(&t1,NULL);

    for (j = 0; j < t; j++)
    {

          // if rank % 2^j == (2^j) - 1:
          // if rank > myPartner, receive from myPartner
          // else, send to myPartner

        if ((*rank) % ((int)(pow(2,j))) == ((int)(pow(2,j)) - 1))
        {
          int myArray[n/(*p)];
          int message;
          int myPartner = findPartner (*rank, j, t);

            // only generate the array on the first pass:

          if (j == 0)
          {
            GenerateArray (myArray, n/(*p), *rank);

              // *TODO: change to an operation that
              // is passed in by user
              // sum all of this processes numbers:
              // on the first pass only:

            for (i = 0; i < n/(*p); i++)
            {
              //printf ("%d: myArray[%d]: %d\n", *rank, i, myArray[i]);
              sum += myArray[i];
            } // sum the array

            //printf ("rank: %d - the Sum: %d\n", *rank, sum);

          } // end if this is the first pass, generate the array

            // If our rank is greater than our partners, we are
            // receiving, otherwise, we are sending:

          if (*rank > myPartner)
          {
            //printf ("receive rank = %d - partner rank = %d - j = %d\n", (*rank), myPartner, j);
            MPI_Recv (&message, 1, MPI_INT, myPartner, j, MPI_COMM_WORLD, &status);
            //printf ("process %d received message %d from process %d\n", (*rank), message, myPartner);

              // Add the sum we received to our sum:

            sum += message;
          } // end if rank > myPartner, receive from myPartner
          else
          {
            //printf ("send rank = %d - partner rank = %d - j = %d\n", (*rank), myPartner, j);
            //printf ("sum: %d\n", sum);
            MPI_Send (&sum, 1, MPI_INT, myPartner, j, MPI_COMM_WORLD);
          } // end else, send to myPartner

        } // end if we pass the mod eval

    } // end for loop to run for t time steps

    // At this point, the last process should contain our answer
    // now, we need to distribute it back out to the other processes:

    for (j = 0; j < t; j++)
    {
      int message;
      int myPartner;
      //printf ("*** Distribute portion ***\n");

      // This communicates in the reverse manner of the first part
      // so, use t - j to find our partner

      if ((*rank) % ((int)(pow(2,(t-j-1)))) == ((int)(pow(2,(t-j-1))) - 1) )
      {
        myPartner = findPartner (*rank, (t - j - 1), t);

        if ((*rank) > myPartner)
        {
          //printf ("Send_2 rank = %d - partner rank = %d - j2 = %d\n", (*rank), myPartner, j);
          MPI_Send (&sum, 1, MPI_INT, myPartner, j + t, MPI_COMM_WORLD);
        } // end if we are greater, we are sending
        else
        {
          //printf ("Receive_2 rank = %d - partner rank = %d - j2 = %d\n", (*rank), myPartner, j);
          MPI_Recv (&message, 1, MPI_INT, myPartner, j + t, MPI_COMM_WORLD, &status);
          sum = message;
          //printf ("rank: %d - sum: %d\n", *rank, sum);

        } // end we are receiving

      } // end only evaluate relevant processes to current time step

    } // end distribute the solution to all processes

    gettimeofday(&t2,NULL);

    runTime = (t2.tv_usec - t1.tv_usec);

    return runTime;

} // end function MyReduce

  // function NaiveReduce

int NaiveReduce (int *rank, int *p, int n)
{
  // n is the size of the full array,
  // rather than send a portion of
  // one array to each process,
  // use the number of processes
  // to determine the size of the
  // sub-array that each process will
  // have and generate a unique array
  // of random numbers of that size:
  // *p is the number of processes,
  // divide n by *p to get the sub-array size,
  // in each process, sum its array and
  // send the answer to (rank + 1)
  // for each process,
  // receive answer from (rank - 1)
  // and add it to the sum of the sub-array
  // for this process

    // declare variables:

  MPI_Status status;
  int i = 0, sum = 0, t = 0, runTime = 0;
  struct timeval t1, t2;

    // Naive happens in two discrete portions.
    // part 1 generates the array, calculates the function,
    // and sends the value to the next process until
    // we finally end at the last process
    // then, part 2 distributes the answer to the
    // other processes:

  gettimeofday(&t1,NULL);

  for (t = 0; t < 2; t++)
  {
        int message;

        if (t == 0)
        {
          int myArray[n/(*p)];
          GenerateArray (myArray, n/(*p), *rank);

            // sum all of this processes numbers:

          for (i = 0; i < n/(*p); i++)
          {
            //printf ("n: %d - p: %d rank: %d - myArray[%d]: %d\n", n, *p, *rank, i, myArray[i]);
            sum += myArray[i];
          } // end sum the array

          if (*rank != 0)
          {
            //printf ("receive: rank: %d from %d\n", *rank, *rank - 1);
            MPI_Recv (&message, 1, MPI_INT, (*rank) - 1, t, MPI_COMM_WORLD, &status);

              // Add the sum we received to our sum:

            sum += message;
            //printf ("rank %d received from rank %d sum += message: %d\n", *rank, *rank - 1, sum);

          } // process zero doesn't receive from anything

          if (*rank != ((*p) - 1))
          {
            //printf ("send: rank = %d to: %d - sum: %d\n", (*rank), (*rank) + 1, sum);
            MPI_Send (&sum, 1, MPI_INT, (*rank) + 1, t, MPI_COMM_WORLD);
          } // the last process doesn't send to anyone

        } // end first part
        else // distribute answer:
        {
          if (*rank != (*p - 1))
          {
            //printf ("receive2: rank: %d - from: %d\n", *rank, *rank + 1);
            MPI_Recv (&message, 1, MPI_INT, (*rank) + 1, t, MPI_COMM_WORLD, &status);

              // Distribute the sum we received to our sum:

            sum = message;
            //printf ("Rank: %d, sum = %d\n", *rank, sum);

          } // the last process doesn't receive from anything

          if (*rank != 0)
          {
            //printf ("send2: rank = %d to %d - sum: %d\n", (*rank), (*rank) - 1, sum);
            MPI_Send (&sum, 1, MPI_INT, (*rank) - 1, t, MPI_COMM_WORLD);
          } // process 0 doesn't send to anyone

        } // end second part

  } // end make two passes

  gettimeofday(&t2,NULL);

  runTime = (t2.tv_usec - t1.tv_usec);

  return runTime;

} // end function NaiveReduce

  // function MPILibraryReduce

int MPILibraryReduce (int *rank, int *p, int n)
{

    // declare variables:

  MPI_Status status;
  int i = 0, sum = 0, runTime = 0;
  int global_sum = 0;
  struct timeval t1, t2;

  int myArray[n];

  //printf ("rank: %d - p: %d\n", (*rank), (*p));

  gettimeofday (&t1, NULL);

  GenerateArray (myArray, n, *rank);

  for (i = 0; i < n/(*p); i++)
  {
    sum += myArray[i];
  }


  MPI_Allreduce (&sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  //printf ("global_sum: %d\n", global_sum);

  gettimeofday (&t2, NULL);

  runTime = (t2.tv_usec - t1.tv_usec);

  return runTime;

} // end function MPILibraryReduce
