#include "test2.h"

int flipCoin (float D, int i)
{
  int Heads = D * 1000;
  int Tails = 1000 - Heads;
  int modVal = Heads / Tails;
  int j = 0;
  int coin[1000];
  int randomValue = 0;

  // first, populate an array of size 1000
  // with a rough approximation of our
  // probability value distribution of
  // 1's and 0's:

  for (j = 0; j < 1000; j++)
  {
    //cout << "modVal: " << modVal << " - j % modVal: " << j % modVal << endl;
    if (j % modVal == 0)
    {
      coin[j] = 0;
    } // end if
    else
    {
      coin[j] = 1;
    } // end else, heads

  } // end for loop
  /*
  for (j = 0; j < 1000; j++)
  {
    cout << coin[j];
  }
  cout << endl;
  */

  // Now, select a random value between 0 and 1000:

  srand(time(NULL) + i);
  //cout << "time: " << time(NULL) << endl;

  randomValue = rand () % 1000;
  //cout << "randomValue: " << randomValue << endl;

  return coin[randomValue];

} // end function flipCoin

// manage dynamic array

int * manageArray (void)
{
  int i = 0;
  int * myArray = (int *) malloc (100 * sizeof(int));

    // fill the array

  for (i = 0; i < 100; i++)
  {
    myArray[i] = i;
  } // end fill array

  for (i = 0; i < 100; i++)
  {
    cout << "in function: " << i << ": " << myArray[i] << endl;
  } // end fill array

  return myArray;

} // end function manageArray

// function nextArray:

Graph * nextArray (int * theCount)
{
  int i = 0;
  ifstream inFile;
  string line;
  char a;
  Graph * theGraph;
  theGraph = (Graph *) malloc (sizeof(Graph) * 3);

  cout << "in test2c, the global variable = " << myGlobalInt << endl;

  inFile.open("testText.txt");
  while (getline (inFile, line))
  {
    a = line[0];
    *theCount = i;

    theGraph[i].nodeID = i;
    theGraph[i].edgeCount++;
    theGraph[i].outEdge = (int *) realloc (theGraph[i].outEdge, theGraph[i].edgeCount * sizeof(int));
    theGraph[i].outEdge[theGraph[i].edgeCount - 1] = a;
    theGraph[i].pageRank = 1;

    //cout << "_Two->a = " << Two[i].nodeID << " - _Two->b = " << Two[i].outEdge[0] << endl;
    i++;
  }

  inFile.close ();

  return theGraph;
} // end function nextArray

  // function printOne,
  // test using pointers for graph array:

void printOne(Graph * theGraph)
{
  cout << "theGraph[0].nodeID: " << theGraph[0].nodeID << endl;

} // end printOne function

  // testPointer function:
  // test passing an array by reference
void testPointer (int * coin)
{
  int i = 0;

  for (i = 0; i < 10; i++)
  {
    coin[i] = 10 - i;
  } // end for loop to fill array

} // end testPointer function
