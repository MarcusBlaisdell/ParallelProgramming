#include "two.c"

int main (int argc, char * argv[])
{
  int rank, p;
  int n = atoi (argv[1]);

  int naiveRunTime = 0;
  int myRunTime    = 0;
  int mpiRunTime   = 0;
  int naiveMax = 0;
  

    // Initialize the MPI process and get the values
    // for rank and number of processes:

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &p);


    // Test using NaiveReduce:

  //naiveRunTime = NaiveReduce (&rank, &p, n);
  //printf ("%d %d\n", n, naiveRunTime);

    // Test using MyReduce:

  //myRunTime = MyReduce (&rank, &p, n);
  //printf ("%d %d\n", n, myRunTime);

    // Test using MPILibraryReduce:

  mpiRunTime = MPILibraryReduce (&rank, &p, n);
  printf ("%d %d\n", n, mpiRunTime);

  MPI_Finalize ();

}
