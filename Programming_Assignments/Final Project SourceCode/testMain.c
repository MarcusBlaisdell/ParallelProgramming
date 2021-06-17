#include "test2.c"

//Graph * theGraph;

int main (int argc, char * argv[])
{
  int myCoin = 0;
  int i = 0;
  int theCount = 0;

  int * myArray;

  MyStruct One;
  Graph * theGraph;

  int coin[10];

  One.a = 1;
  One.b = 'c';

  //printf ("a = %d, b = %c\n", One.a, One.b);
  //cout << "a = " << One.a << ", b = " << One.b << endl;

    // flip the coin ten times:
  /*
  for (i = 0; i < 100; i++)
  {
    myCoin = flipCoin (0.867, i);
    //cout << "myCoin: " << myCoin << endl;
  } // end flip 10 times
  */

  /*
  myArray = manageArray ();

  for (i = 0; i < 100; i++)
  {
    cout << "out of function: " << i << ": " << myArray[i] << endl;

  } // end print the array
  */

  theGraph = nextArray (&theCount);
  cout << "theCount: " << theCount << endl;

  for (i = 0; i < 3; i++)
  {
    cout << "theGraph->nodeID = " << theGraph[i].nodeID << " - theGraph->outEdge[0] = " << theGraph[i].outEdge[0] << endl;
  }

  cout << "in Main, the global variable = " << myGlobalInt << endl;

  printOne (theGraph);

  int testThis[5][2];
  testThis[0][0] = 0;
  testThis[1][0] = 1;
  testThis[2][0] = 2;
  testThis[3][0] = 3;
  testThis[4][0] = 4;

  testThis[0][1] = 5;
  testThis[1][1] = 6;
  testThis[2][1] = 7;
  testThis[3][1] = 8;
  testThis[4][1] = 9;

  cout << "testThis[0][0] = " << testThis[0][0] << "testThis[0][1] = " << testThis[0][1] << endl;
  testThis[0][0] = testThis[1][0];
  testThis[0][1] = testThis[1][1];
  cout << "testThis[0][0] = " << testThis[0][0] << "testThis[0][1] = " << testThis[0][1] << endl;

  testPointer (coin);

  for (i = 0; i < 10; i++)
  {
    cout << "coin[" << i << "]: " << coin[i] << endl;
  }

}
