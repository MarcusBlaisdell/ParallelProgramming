/*********************************
* Programming Assignment #4
* Pi estimator
*
* four.c
*
* Marcus Blaisdell
*
*/

#include "four.h"


  // getPoint function:
  // determine if a point is inside the circle,
  // return 1 if yes, 0 if no:

int getPoint (int radius, int x, int y)
{
  float dist;

  dist = calcDist (radius, x, y);

  if (dist <= radius)
  {
    return 1;
  } // end if point is within the circle
  else
  {
    return 0;
  } // end print not in message

} // end getPoint function

  // calcDist function:
  // calculates the distance
  // of a point from the center of our circle

float calcDist (int radius, int x, int y)
{
  float dist;

  dist = sqrt(pow((x - radius), 2) + (pow((y - radius), 2)));

  return dist;

} // end calcDist function

  // runIt function:
  // run the pi estimator:
  // accepts a radius and number of iterations
  // generates random points and tests if they
  // are within the circle,
  // returns the number of points in the circle
  // as number of sucesses:

float runIt (int radius, int iterations)
{
  int i = 0;
  int success = 0;
  float pi = 0;
  float x = 0, y = 0;

  #pragma omp parallel for schedule(static) shared(success)
  for(i = 0; i < iterations; i++)
  {
    #pragma omp critical
    {
      int rank = omp_get_thread_num();
      int seed = rank;
      srand(seed);

      for (i = 0; i < iterations; i++)
      {
        x = (float)((rand_r(&seed)) % 200000) / 100000;
        y = (float)((rand_r(&seed)) % 200000) / 100000;

        success += getPoint (radius, x, y);

      } // end for loop

    } // end critical portion

  } // end parallel portion

  return success;

} // end runIt function

  // testPi function:
  // reads pi values up to 2000 places
  // after the decimal and compares the
  // prediction to the actual value,
  // precision is counted as the number
  // of matching digits after the decimal point
  // if the first value before the decimal point
  // matches

int testPi (float pi)
{
  int i = 0;
  int precision = 0;

    // create a char array of size 2002,
    // this will let us have 2000 digits
    // after the decimal point:

  char pichar[2002];

    // create a single char to read from
    // the pi text file one byte at a time
  char c;

    // convert our float to a string:

  gcvt (pi, 2002, pichar);

  FILE *inFile;

  inFile = fopen("pi2000.txt", "r");

  c = fgetc(inFile);

  while (c == pichar[i])
  {
    precision++;

    // only count precision as what comes after decimal point
    if (c == '.')
    {
      precision = 0;
    } // end only count precision as what comes after decimal point

    i++;

    c = fgetc(inFile);

  } // end get precision

  fclose (inFile);

  printf ("Precision: %d\n", precision);

} // end testPi function
