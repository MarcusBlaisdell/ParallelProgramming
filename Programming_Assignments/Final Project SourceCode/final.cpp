/*************************************
* Marcus Blaisdell
* Cpt_S 411
*
* Final Project:
*    Parallel PageRank Estimator
*
* final.cpp
*
*************************************/

#include "final.h"

  // function buildProbabilityArray
  // Accepts a probability value,
  // returns an array of size 1000 of
  // evenly distributed heads and tails
  // based on the given probability
  // distribution from the user.
  // This array will be used for the
  // coin toss by allowing us to choose
  // a random number between 0 and 999
  // to select a coin from the array.
  // Since the distribution of the
  // heads and tails in the array is
  // in the proportion of the given
  // probability, selecting one position
  // at random is the effective result of
  // flipping a coin that has that probability.
  // Since the probability is the same
  // for every iteration, this array
  // should only be built once and then
  // referenced for each coin toss

void buildProbabilityArray (float D, int * coin)
{
  int Heads = D * 1000;
  int Tails = 1000 - Heads;
  int modVal = 0;
  int j = 0;

  if (Heads > Tails)
  {
    modVal = Heads / Tails;
  } // end if Heads > Tails
  else
  {
    modVal = Tails / Heads;
  } // end else, Heads < Tails

  /*
  cout << "D: " << D << endl;
  cout << "Heads: " << Heads << endl;
  cout << "Tails: " << Tails << endl;
  cout << "modVal: " << modVal << endl;
  */

  // first, populate an array of size 1000
  // with a rough approximation of our
  // probability value distribution of
  // 1's and 0's:

  for (j = 0; j < 1000; j++)
  {
    if ((j % modVal) == 0)
    {
      if (Heads > Tails)
      {
        coin[j] = 0;
      } // end if Head > Tails, populate with Tails
      else
      {
        coin[j] = 1;
      } // end else, populate with Heads

    } // end if
    else
    {
      if (Heads > Tails)
      {
        coin[j] = 1;
      } // end if Head < Tails, populate with Heads
      else
      {
        coin[j] = 0;
      } // end else, populate with Tails

    } // end else, heads

  } // end for loop

  /*
  for (j = 0; j < 1000; j++)
  {
    cout << coin[j];
  }
  cout << endl;
  */

} // end function buildProbabilityArray

  // function flipCoin
  // accepts a probability, D
  // and uses this to flip a coin
  // and return the result
  // 0 is tails, 1 is heads
  // To use D to create a probability,
  // I first multiply D by 1000 to obtain
  // my Heads count.
  // The Tails count is then 1000 - Heads Count
  // I then create a single array of
  // (count of heads) 1's, and (count of tails) 0's
  // interlaced in the array

  int flipCoin (int * coin, int i)
  {
    int randomValue = 0;
    
    // select a random value between 0 and 1000:

    srand(time(NULL) + i);

    randomValue = rand () % 1000;

    return coin[randomValue];

  } // end function flipCoin

  // function getNodeCount:
  // accepts a string
  // and reads the value for Nodes
  // and returns it as an int
  // Assumes that the string it receives
  // is in the form:
  // # Nodes: xxxxxx Edges: yyyyy

int getNodeCount (string line)
{
  int Nodes = 0;
  int i = 9;

    // read each digit in the number
    // and build the full value:

  while (line[i] != ' ')
  {
    // we will successively build the full value by
    // multiplying the current value by ten,
    // and adding the next digit:

    Nodes *= 10;
    Nodes += line[i++] - '0';

  } // end for loop

  return Nodes;

} // end function getNodeCount

  // function loadGraph:
  // accepts a pointer to a filename,
  // and a pointer to an int to store the total
  // number of nodes and
  // creates a local copy of a Graph Array and
  // loads the data from the text file into the array
  // and returns the array to the caller

Graph * loadGraph (char * filename, int * NodesCount)
{
  string line;
  string theNodeString;
  string outEdgeString;
  int Nodes = 0, startNode = 0;
  int theNodeInt = 0;
  int outEdgeInt = 0;
  int currentNode = 0;
  int i = 0, x = 0;
  Graph * theGraph;

  ifstream inFile;

    // open the file for reading:

  inFile.open (filename);

    // read the file in to our array:
    // some of the nodes are missing
    // missing nodes will still be represented in the Array
    // but will have a 0 edge count
    // Note: a node can be present and have no outgoing edges,
    // this will be represented as an edge count of 0 and
    // notes that the node does exist but has no links

  while (getline (inFile, line) )
  {
      // remove the line break:
      // ********************************************************
      // (Use this for all files except the facebook file)
      // (For some reason, the facebook file is formatted)
      // (differently and removing the last char removes actual)
      // (data and not a line break)
      // ********************************************************
    line.pop_back ();

      // reset position to 0:
    i = 0;

      // if we have comments,
      // read through them looking
      // for a Node count:

    if (line[0] == '#')
    {

      while (line[0] == '#')
      {
          // !!!This could be improved **FIX**
        if (line[2] == 'N' && line[3] == 'o' && line[4] == 'd' && line[5] == 'e' && line[6] == 's' && line[7] == ':')
        {
            // if we have found a line that contains the string 'Nodes: ',
            // parse it into a node count:

          Nodes = getNodeCount (line);
          *NodesCount = Nodes;
          //cout << "Nodes: " << Nodes << endl;

        } // end get number of nodes

        getline (inFile, line);
        // ********************************************************
        // (Use this for all files except the facebook file)
        // (For some reason, the facebook file is formatted)
        // (differently and removing the last char removes actual)
        // (data and not a line break)
        // ********************************************************
        line.pop_back ();

      } // end look through commented lines

        // Now that we know how many nodes we have,
        // make our graph the right size:

      theGraph = (Graph*) malloc (Nodes * sizeof (Graph));

    } // end if we have commented lines

      // parse the input:
      // everything up to the first space/tab is the node id:

    while (line[i] != '\t' and line[i] != ' ')
    {
      theNodeString += line[i];
      i++;

    } // end concatenate all numerics into node id

      // the outgoing edge is everything to the end of the line:

    while (line[i])
    {
      outEdgeString += line[i];
      i++;

    } // end get the id of the outgoing edge

      // convert the nodeID from string to int:

    theNodeInt = stoi (theNodeString);

      // There are two possible conditions here:
      //
      // 1: the node we are currently pointing to
      //    is the node we need to store the
      //    out edge to
      //       If we are currently pointing to
      //       the node we are storing values for,
      //       then just add the outgoing edge
      //       onto the current node out edge array,
      //
      // 2: the node id we just read is past the
      //    current node we are pointing to
      //       In this case, increment the
      //       node we are pointing to
      //       and check if we are now at the
      //       right node, if yes, add the
      //       out edge onto this node,
      //       if not, then we have empty nodes
      //       between the last node we had edges for
      //       and this current node, and we need to
      //       note that there are nodes that do not
      //       have outgoing edges by storing a 0
      //       for the edge count at those nodes:

    outEdgeInt = stoi(outEdgeString);

    if (theNodeInt == currentNode)
    {
      // If the nodeID has not been updated to match
      // the currentNode, update it:

      if (theGraph[currentNode].nodeID != currentNode)
      {
        theGraph[currentNode].nodeID = currentNode;
      } // end update nodeID

      // I had to add a counter to keep track of the number of edges
      // since there isn't an easy way to get an array size in C
      // so, whenever we add an edge to a node, increment this counter
      // and since we're using dynamic memory, we need to increase the
      // allocation using realloc and then add the new edge:

      theGraph[currentNode].edgeCount++;
      theGraph[currentNode].outEdge = (int *) realloc (theGraph[currentNode].outEdge, theGraph[currentNode].edgeCount * sizeof(int));
      theGraph[currentNode].outEdge[theGraph[currentNode].edgeCount - 1] = outEdgeInt;

    } // end push on edge
    else
    {
      // if the current node doesn't match the node we just read,
      // then we have just encountered our next node and we need
      // to test if there are empty nodes between the last node
      // we added edges to and this node

      // move to the next node and evaluate:
      theGraph[currentNode].nodeID = currentNode;
      currentNode++;

      // If the new node we just moved to
      // does not match the node we have an out edge for,
      // then we are pointing to a node that does not have
      // an out edge and we need to note this by storing a
      // 0 at that nodes edge count and then progress
      // to the next node:

      while (currentNode != theNodeInt)
      {
          // If there are no edges to add to a node,
          // We will leave the edge count at default 0
          // and skip over doing anything with this node:
        theGraph[currentNode].nodeID = currentNode;
        currentNode++;

      } // end increment currentNode to match the actual node

        // we should now have moved to the next valid node
        // for which we have an outgoing edge
        // and we still have our outEdge from our last read
        // so we need to add that to this current node:

      theGraph[currentNode].edgeCount++;
      theGraph[currentNode].outEdge = (int *) realloc (theGraph[currentNode].outEdge, theGraph[currentNode].edgeCount * sizeof(int));
      theGraph[currentNode].outEdge[theGraph[currentNode].edgeCount - 1] = outEdgeInt;

    } // end move to correct node
//cout << "theNodeString: " << theNodeString << endl;
//cout << "outEdgeString: " << outEdgeString << endl;

    theNodeString.clear ();
    outEdgeString.clear ();

  } // end read nodes from file

    // return the populated graph:

  return theGraph;

} // end function loadGraph

  // function choose_d_u:
  // accepts an int value that is the
  // number of nodes to choose from
  // with equal probability,
  // chooses a number from 0 to this value
  // randomly and returns it:

int choose_d_u (int d_u, int i)
{
  int randomValue = 0;

  srand(time(NULL) + i);

  randomValue = rand () % d_u;

  return randomValue;

} // end function choose_d_u

  // printTop5 function
  // accepts a pointer to a Graph array,
  // finds the top 5 ranked nodes and prints them

void printTop5 (Graph * theGraph, int NodesCount)
{
  // store the nodeID's and page ranks of the top 5 nodes
  int theTop[5][2] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
  int i = 0;

  while (i < NodesCount)
  {
    /*
    cout << "theGraph[" << i << "]: " << theGraph[i].pageRank << endl;
    cout << "theTop[0][1]" << theTop[0][1] << endl;
    cout << "theTop[1][1]" << theTop[1][1] << endl;
    cout << "theTop[2][1]" << theTop[2][1] << endl;
    cout << "theTop[3][1]" << theTop[3][1] << endl;
    cout << "theTop[4][1]" << theTop[4][1] << endl;
    */

    if (theGraph[i].pageRank > theTop[0][1])
    {
      //cout << theGraph[i].pageRank << " > " << theTop[0][1] << endl;
      theTop[4][0] = theTop[3][0];
      theTop[4][1] = theTop[3][1];
      theTop[3][0] = theTop[2][0];
      theTop[3][1] = theTop[2][1];
      theTop[2][0] = theTop[1][0];
      theTop[2][1] = theTop[1][1];
      theTop[1][0] = theTop[0][0];
      theTop[1][1] = theTop[0][1];
      theTop[0][0] = theGraph[i].nodeID;
      theTop[0][1] = theGraph[i].pageRank;
      i++;
      continue;
    } // end if greater than top 1
    else if (theGraph[i].pageRank > theTop[1][1])
    {
      //cout << theGraph[i].pageRank << " > " << theTop[1][1] << endl;
      theTop[4][0] = theTop[3][0];
      theTop[4][1] = theTop[3][1];
      theTop[3][0] = theTop[2][0];
      theTop[3][1] = theTop[2][1];
      theTop[2][0] = theTop[1][0];
      theTop[2][1] = theTop[1][1];
      theTop[1][0] = theGraph[i].nodeID;
      theTop[1][1] = theGraph[i].pageRank;
      i++;
      continue;
    } // end if greater than top 2
    else if (theGraph[i].pageRank > theTop[2][1])
    {
      //cout << theGraph[i].pageRank << " > " << theTop[2][1] << endl;
      theTop[4][0] = theTop[3][0];
      theTop[4][1] = theTop[3][1];
      theTop[3][0] = theTop[2][0];
      theTop[3][1] = theTop[2][1];
      theTop[2][0] = theGraph[i].nodeID;
      theTop[2][1] = theGraph[i].pageRank;
      i++;
      continue;
    } // end if greater than top 3
    else if (theGraph[i].pageRank > theTop[3][1])
    {
      //cout << theGraph[i].pageRank << " > " << theTop[3][1] << endl;
      theTop[4][0] = theTop[3][0];
      theTop[4][1] = theTop[3][1];
      theTop[3][0] = theGraph[i].nodeID;
      theTop[3][1] = theGraph[i].pageRank;
      i++;
      continue;
    } // end if greater than top 4
    else if (theGraph[i].pageRank > theTop[4][1])
    {
      //cout << theGraph[i].pageRank << " > " << theTop[4][1] << endl;
      theTop[4][0] = theGraph[i].nodeID;
      theTop[4][1] = theGraph[i].pageRank;
    } // end if greater than top 5

    i++;

  } // end iterate through the graph

  for (i = 0; i < 5; i++)
  {
    cout << "rank " << i + 1 << ": " << theTop[i][0] << " : " << theTop[i][1] << endl;
  } // end print top 5 node id's and page ranks

} // end printTop5 function
