#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>

using namespace std;

typedef struct graph
{
  int nodeID;
  vector <int> outEdge;
  int pageRank = 0;
} Graph;

void testLine (char line[])
{
  cout << "inside function: line: " << line << endl;

} // end testLine function

int testString (char * line)
{
  int Nodes = 0;
  string NodesChar;
  int i = 9;
  int len = strlen(line);
  //char line2[len];
  //strcpy (line2,line);

  /*
  printf ("testString:\n%s\n--\n", line2);
  printf ("line size:\n%d\n--\n", len);
  printf ("line2[len - 1]:\n%c\n--\n", line2[len - 1]);
  line2[len - 1] = '\0';
  printf ("testString2:\n%s\n--\n", line2);
  */

  while (line[i] != ' ')
  {
    printf ("line[%d]: %c\n", i, line[i]);
    //NodesChar += line[i++];
    Nodes *= 10;
    Nodes += line[i++] - '0';

  } // end for loop

  //printf ("NodesChar: %s\n", NodesChar);
  printf ("Nodes: %d\n", Nodes);
  //Nodes = atoi (NodesChar);

  return Nodes;

} // end testString function

void handleStruct (vector <Graph> *theGraph)
{
  int i = 0;

  for (i = 0; i < 3; i++)
  {
    Graph *newGraph = new Graph;

    newGraph->nodeID = i;

    newGraph->outEdge.push_back(i + 1);
    newGraph->outEdge.push_back(i + 2);
    newGraph->outEdge.push_back(i + 3);

    theGraph->push_back (*newGraph);

  } // end for loop



} // end handleStruct function

void handleVector (vector <vector <int> > * one)
{
  vector <int> two;

  /*
  one->push_back (0);
  one->push_back (1);
  one->push_back (2);
  */

  two.push_back (3);
  cout << "two.size(): " << two.size() << endl;
  two.push_back (4);
  cout << "two.size(): " << two.size() << endl;
  two.push_back (5);
  cout << "two.size(): " << two.size() << endl;

  one->push_back(two);

  two.clear ();
  two.push_back (6);
  two.push_back (7);
  two.push_back (8);

  one->push_back(two);

  two.clear ();

  two.push_back (-1);
  one->push_back(two);

  two.clear ();

  two.push_back (9);
  two.push_back (10);
  two.push_back (11);

  one->push_back(two);

} // end handleVector function

int readFromFile ()
{
  string line;
  string theString;
  int i = 0;

  theString.append("1");
  theString.append("2");
  theString.append("0");
  theString.append("\0");

  cout << "theString as string: " << theString << endl;
  cout << "theString as int: " << stoi(theString) << endl;


  ifstream inFile;
  inFile.open ("testIn.txt");

  getline (inFile, line);

  cout << "*** *** *** line: " << line << endl;
  line.pop_back();
  cout << "    *** *** *** line: " << line << endl;

  cout << "line is: " << typeid(line).name() << endl;
  cout << "i is: " << typeid(i).name() << endl;
  //testLine (line);
  //printf ("line: %s\n", line);

  /*
  while (line[i] != ' ')
  {
    theString += line[i++];
  } // end get full string

  theString += '\n';

  printf ("theString as string: %s\n", theString);
  printf ("thestring as int: %d\n", atoi (theString) );
  */

  inFile.close ();


  return 0;

}

int main (int argc, char * argv[])
{
  vector <vector <int> > one;
  vector <Graph> myGraph;
  int i = 0, j = 0;
  int Nodes;
  char * line = "# Nodes: 875713 Edges: 5105039x";

  handleVector (&one);

  printf ("vector size: %d\n", one.size() );

  for (i = 0; i < one.size (); i++)
  {
    for (j = 0; j < one[i].size(); j++)
    {
      printf ("vector[%d][%d] = %d\n", i, j, one[i][j]);

    } // end inner for


  } // end for loop to print all

  readFromFile ();

  handleStruct (&myGraph);

  cout << "myGraph.size(): " << myGraph.size() << endl;

  for (vector <Graph>:: iterator it = myGraph.begin(); it != myGraph.end(); ++it)
  {
    cout << "myGraph: " << it->nodeID << endl;
  } // end iterate through vector

  Nodes = testString (line);

  printf ("Nodes = %d\n", Nodes);

}
