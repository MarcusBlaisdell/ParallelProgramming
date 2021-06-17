#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>


int MyReduce (int *rank, int *p, int n);

int NaiveReduce (int *rank, int *p, int n);

int MPILibraryReduce (int *rank, int *p, int n);

  // function GenerateArray will generate an array of n integers:
void GenerateArray(int theArray[], int n, int theRank);
  // function findPartner will find the partner process to a given process:
int findPartner (int theProcess, int thePos, int total);
  // function to find the max  value in an array:
int maxTime (int timeArray[], int *p);
