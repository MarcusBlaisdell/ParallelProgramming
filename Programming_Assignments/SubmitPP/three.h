#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

  // function findPartner will find the partner process to a given process:
int findPartner (int theProcess, int thePos, int total);

  // function to find the max  value in an array:
int maxTime (int timeArray[], int *p);

  // p-element parallel prefix function:
int p_parallelPrefix (int A[], int *rank, int *p, int flag, int *l2);

  // n-element parallel prefix function:
int n_parallelPrefix (int A[], int *rank, int *p, int n);

  // parallel random number generator function:
int p_RNG (int seed, int A, int B, int P, int n, int *rank, int *p);

  // serial_matrix function:
int serial_matrix (int seed, int A, int B, int P, int n, int *rank, int *p, int * M_off);

  // Operator function:
void Operator (int One[], int Two[], int * Product, int P);
