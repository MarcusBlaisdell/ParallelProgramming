#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct graph
{
  int nodeID;
  int * outEdge = NULL;
  int edgeCount = 0;
  int pageRank = 0;
} Graph;

typedef struct mystruct
{
  int a;
  char b;
} MyStruct;

extern int myGlobalInt = 42;
//extern Graph * theGraph;

int flipCoin (float D, int i);

int * manageArray (void);

Graph * nextArray (int * theCount);

void printOne(Graph * theGraph);

void testPointer (int * coin);
