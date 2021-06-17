/*********************************
* Programming Assignment #4
* Pi estimator
*
* four.h
*
* Marcus Blaisdell
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

// getPoint function
int getPoint (int radius, int x, int y);

// calcDist function:
float calcDist (int radius, int x, int y);

// runIt
float runIt (int radius, int iterations);

// testPi function:
int testPi (float pi);
