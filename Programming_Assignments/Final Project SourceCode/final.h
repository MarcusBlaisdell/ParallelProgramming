/*************************************
* Marcus Blaisdell
* Cpt_S 411
*
* Final Project:
*    Parallel PageRank Estimator
*
* final.h
*
*************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <time.h>

using namespace std;

// use a struct to track all node information:
typedef struct graph
{
  int nodeID;
  int * outEdge = NULL;
  int edgeCount = 0;
  int pageRank = 0;
} Graph;

Graph * loadGraph (char * filename, int * NodesCount);

int getNodeCount (char * line);

void buildProbabilityArray (float D, int * coin);

int flipCoin (int * coin, int i);

int choose_d_u (int d_u, int i);

void printTop5 (Graph * theGraph, int NodesCount);

// EOF
