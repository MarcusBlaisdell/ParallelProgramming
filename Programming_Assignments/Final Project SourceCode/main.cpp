/*************************************
* Marcus Blaisdell
* Cpt_S 411
*
* Final Project:
*    Parallel PageRank Estimator
*
* main.cpp
*
*************************************/

#include "final.cpp"

// main function:
//
// accepts three command line arguments:
// 1. value of variable K:
//    The length of the walk, K,
//    is provided by the user
//
// 2. value of variable D:
//    The value of the Damping ratio, D,
//    is provided by the user
//
// 3. The input graph file:
//    The file that contains the graph
//    to be used will be specified by
//    the user

int main (int argc, char * argv[])
{
  int i = 0;          // my favorite iterator
  int j = 0;          // my favorite second iterator
  int K = 0;          // the size of the random walk
  int coin[1000];     // array to be used for probability distribution
  int coinToss = 0;   // the result of a coin toss
  int d_u = 0;        // the size of the outgoing edges array of a node
  int nextNode = 0;   // the next Node to move to, chosen randomly
  int n = 0;          // the total number of nodes in a graph
  float D = 0.0;      // The damping value
  int NodesCount = 0; // total number of nodes in a graph (N)
  char * filename;    // the text file to be used to build/load the graph
  Graph * theGraph;   // the array that will hold/be our graph
  double startTime;
  double endTime;
  int theSize = 0;
  int p = 1;
  int rank = 0;

    // if there are less than 4 command line arguments,
    // inform the user of how we expect our input:

  if (argc < 6)
  {
    printf ("\n\n*** Insufficient arguments ***\n\n");
    printf ("Usage: final <K> <D> <filename>");
    printf ("Where <K> is the length of the walk\n");
    printf ("<D> is the \"Damping factor\" and is a value between 0 and 1 (0 <= D <= 1)\n");
    printf ("and <filename> is the name of the text file that contains the graph \n");
    printf ("we are evaluating.\n\n\n");

    return 0;

  } // end if there are less than 4 command line args
  else if (argc == 6)
  {
    K = atoi (argv[1]);
    D = atof (argv[2]);
    filename = argv[3];
    theSize = atoi(argv[4]);
    p = atoi(argv[5]);

  } // end if there are 4 args, assume we have the correct args

    // set number of threads:

  omp_set_num_threads(p);

    // create the probability distibution for the coin:
  buildProbabilityArray (D, coin);

    // load the graph:

  theGraph = loadGraph (filename, &NodesCount);

    // Now that we have the graph, perform the walk:

    // create a new Graph object and set it equal to the starting node:

    Graph * curNode;

    startTime = omp_get_wtime();

    // for each node in theGraph, take the given number
    // of steps using the following rules:
    //
    // 1. Toss a coin with probability D of landing heads,
    //    and probability 1-D of landing tails:

  //for (j = 0; j < NodesCount; j++)
  #pragma omp parallel shared (theGraph, coin) private (curNode, coinToss, nextNode, i, d_u)
  #pragma omp for schedule (dynamic)
  for (j = 0; j < theSize; j++)
  {
    if (theGraph[j].edgeCount > 0)
    {
      curNode = &theGraph[j];
      for (i = 0; i < K; i++)
      {
          // flip the coin, use the
          // value of i to change the
          // random seed:

        coinToss = flipCoin (coin, i);

        if (coinToss == 0)
        {
          // choose from d(u) at random:
          d_u = curNode->edgeCount;
          if (d_u != 0)
          {
            nextNode = choose_d_u (d_u, i);
          } // end choose next node if there are nodes to choose from
          else
          {
            nextNode = choose_d_u (NodesCount, i);
          } // end else, choose a node at random

        } // end if coin is tails
        else
        {
          nextNode = choose_d_u (NodesCount, i);

        } // end else, coin is heads

          // move to the next node:

        curNode = &theGraph[nextNode];
        curNode->pageRank++;
        //cout << "curNode->nodeID: " << curNode->nodeID << " : " << curNode->pageRank << endl;

      } // end perform the walk

    } // end only start at nodes that have links


  } // end loop through all nodes:

  endTime = omp_get_wtime();

  printTop5 (theGraph, NodesCount);

  cout << "Total time: " << endTime - startTime << endl;

  /*
    // quick data verification, test random points and verify they are correct:
    // This is using the web-Google_sorted.txt file:
  cout << "theGraph[0].nodeID: " << theGraph[0].nodeID << endl;
  cout << "theGraph[0].outEdge[0]: " << theGraph[0].outEdge[0] << endl;
  cout << "Should be: 11342" << endl;

  cout << "theGraph[420073].nodeID: " << theGraph[420073].nodeID << endl;
  cout << "theGraph[420073].outEdge[5]: " << theGraph[420073].outEdge[5] << endl;
  cout << "Should be: 766808" << endl;
  cout << "theGraph[916427].nodeID: " << theGraph[916427].nodeID << endl;
  cout << "theGraph[916427].outEdge[3]: " << theGraph[916427].outEdge[3] << endl;
  cout << "Should be: 394429" << endl;
  */

} // end main
