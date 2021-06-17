/**********************************************************
 *
 *  Marcus Blaisdell
 *  CptS 411
 *  Programming Assignment 1
 *  9/17/2019
 *
 *  This code leverages from the send_recv_test.c sample
 *  code provided by Professor Kalyanaraman
 *
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *
/*********************************************************/


#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>

/*
 * runBlocking function:
 * This function accepts as arguments pointers to the rank and p (processor) values
 * assigned when MPI was initialized.
 *
*/

void runBlocking (int *rank, int *p)
{
      // Declare variables:

    int dest = 0; // Define rank 0 as the send-to or receive rank
    int x = 0;
    int i = 1;
    int tSend = 0, tRecv = 0;
    int mSize = 2097152; // message size is 2MB

       // For some reason this only works if I use the numeric value
       // If I use the variable mSize + 1, it crashes
    //char receivedMessage[mSize + 1];
    char receivedMessage[2097153];

    MPI_Status status;

    struct timeval t1, t2;

      // write time and message sizes to files for both send and receive:

    FILE *sendFile;
    FILE *receiveFile;

    sendFile = fopen ("Bsend.csv", "a");
    receiveFile = fopen ("Breceive.csv", "a");

      // Only run if the number of processes is two or more:

    assert (*p >= 2);

        // Use a while loop to continously double the size of the message
        // starting at 1 byte and increasing to 1MB (or 2MB):

        // To force the process to use the network, use non-consecutive
        // processes, I have chosen 0 and 4 as was suggested in class:

      i = 1;

      while (i < (mSize + 1))
      {
          // Create a string of "a's" of mSize + 1 to get the termination character:
        char message[i + 1];
        for (x = 0; x < i; x++)
        {
          message[x] = 'a';
        } // end create a string of "a's"

          // close off the string:

        message[x] = '\0';
        //printf ("i: %d\n", i);

          // Use rank 4 as the send process:

        if (*rank == 4)
        {
          gettimeofday (&t1, NULL);

            // send a message of size i by offsetting the message start point to
            // be i bytes from the end:

            // By experimentation, the MPI_Send count must be 1/4 the size of the
            // message, or in other words, a count of 1 allows for 4 bytes of
            // information to be sent so count must be message size divided by 4:

          MPI_Send (message, (mSize / 4), MPI_INT, dest, 0, MPI_COMM_WORLD);

          gettimeofday (&t2, NULL);

          tSend = (t2.tv_usec - t1.tv_usec);

            // write the message size and transmit time to file:

          fprintf (sendFile, "%d, %d\n", i, tSend);

        } // end if

          // Use rank 0 as the receive process:

        else if (*rank == 0)
        {
          gettimeofday (&t1, NULL);

              // Allow the message to receive up to the full message size
              // to get the full message regardless of the size sent:

          MPI_Recv (&receivedMessage, (mSize + 1), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

          gettimeofday (&t2, NULL);

          tRecv = (t2.tv_usec - t1.tv_usec);

          fprintf (receiveFile, "%d, %d\n", i, tRecv);

        } // end else if

          // double the message size for the next iteration:

        i = i * 2;

      } // end while loop

    fclose (sendFile);
    fclose (receiveFile);

} // end runBlocking Function

/*
 * runNonBlocking function:
 * This function accepts as arguments pointers to the rank and p (processor) values
 * assigned when MPI was initialized.
 *
*/

void runNonBlocking (int *rank, int *p)
{
      // Declare variables:

    int dest = 0; // Define rank 0 as the send-to or receive rank
    int x = 0;
    int i = 1;
    int tSend = 0, tRecv = 0;
    int mSize = 2097152; // message size is 2MB

       // For some reason this only works if I use the numeric value
       // If I use the variable mSize + 1, it crashes
    //char receivedMessage[mSize + 1];
    char receivedMessage[2097153];

    MPI_Request Srequest, Rrequest;

    MPI_Status status;

    struct timeval t1, t2;

      // write time and message sizes to files for both send and receive:

    FILE *sendFile;
    FILE *receiveFile;

    sendFile = fopen ("NBsend.csv", "a");
    receiveFile = fopen ("NBreceive.csv", "a");

      // Only run if the number of processes is two or more:

    assert (*p >= 2);

        // Use a while loop to continously double the size of the message
        // starting at 1 byte and increasing to 1MB (or 2MB):

        // To force the process to use the network, use non-consecutive
        // processes, I have chosen 0 and 4 as was suggested in class:

      i = 1;

      while (i < (mSize + 1))
      {

        char message[i + 1];
          // Create a string of "a's" of mSize + 1 to get the termination character:

        for (x = 0; x < i; x++)
        {
          message[x] = 'a';
        } // end create a string of "a's"

          // close off the string:

        message[x] = '\0';

          // Use rank 4 as the send process:

        if (*rank == 4)
        {
          gettimeofday (&t1, NULL);

            // send a message of size i by offsetting the message start point to
            // be i bytes from the end:

            // By experimentation, the MPI_Send count must be 1/4 the size of the
            // message, or in other words, a count of 1 allows for 4 bytes of
            // information to be sent so count must be message size divided by 4:

            // I experimented with using MPI_Send but my transmission times were
            // all curiously small, by using MPI_Isend, the values were more
            // closely matched with the receive times:

          MPI_Isend (message, (mSize / 4), MPI_INT, dest, 0, MPI_COMM_WORLD, &Srequest);
          MPI_Wait (&Srequest, &status);

          gettimeofday (&t2, NULL);

          tSend = (t2.tv_usec - t1.tv_usec);

            // write the message size and transmit time to file:

          fprintf (sendFile, "%d, %d\n", i, tSend);

        } // end if

          // Use rank 0 as the receive process:

        else if (*rank == 0)
        {
          gettimeofday (&t1, NULL);

              // Allow the message to receive up to the full message size
              // to get the full message regardless of the size sent:

          MPI_Irecv (&receivedMessage, (mSize + 1), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Rrequest);
          MPI_Wait (&Rrequest, &status);

          gettimeofday (&t2, NULL);

          tRecv = (t2.tv_usec - t1.tv_usec);

          fprintf (receiveFile, "%d, %d\n", i, tRecv);

        } // end else if

          // double the message size for the next iteration:

        i = i * 2;

      } // end while loop

    fclose (sendFile);
    fclose (receiveFile);

} // end runNonBlocking Function

/*
 * Main function:
 * I am initializing MPI in the main and passing the rank and processor information
 * to each function as pointers.
 *
*/

int main (int argc, char *argv[])
{

  int rank, p;

    // Initialize the MPI process and get the values
    // for rank and number of processes:

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &p);

    // Check command line argument to see which function
    // we will run:

  if (*argv[1] == '0')
  {
    runNonBlocking (&rank, &p);
  } // end run NonBlocking

  else if (*argv[1] == '1')
  {
    runBlocking (&rank, &p);
  } // end run Blocking

  MPI_Finalize ();

}
