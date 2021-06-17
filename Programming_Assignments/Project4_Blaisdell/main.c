/*********************************
* Programming Assignment #4
* Pi estimator
*
* main.c
*
* Marcus Blaisdell
*
*/

#include "four.c"

int main (int argc, char * argv[])
{
  int radius = 1;
  long int n = 0;
  int i = 0, j = 0;
  float pi = 0.0;
  int t = 0, p = 0;
  int rank = 0;
  int success = 0;
  double time = 0;

  if (argc < 4)
  {
    printf ("Usage: main <number of iterations> <number of threads> <number of processes>\n");
    return 0;

  } // end print error
  else if (argc == 4)
  {
    n = atoi (argv[1]);
    t = atoi (argv[2]);
    p = atoi (argv[3]);

  } // end get command line args

  // n is the total number of iterations
  // since I am dividing work among the threads,
  // the total iterations per thread is n / p
  // to complete n total iterations:

  //n = (n / t) / p;

  // hold n/p constant:
  // so, if we have n iterations and p processes,
  // if n = 1B, p = 1, our constant is 1B
  // if p = 2, then n / p = 1/2(n),
  // so we need to double n, so just multiply n by p
  // and we will still divide the number of threads so we have:

  n = (n * p) / t;

  omp_set_num_threads (p);

	#pragma omp parallel
	{
		assert (p == omp_get_num_threads());

		rank = omp_get_thread_num();

	}  // end of my omp parallel region

  time = omp_get_wtime();

  success += runIt (radius, n);

  pi = 4 * (float)success / ((float)(n * p));

  testPi (pi);
  time = omp_get_wtime() - time;
  printf ("time: %f\n", time);
  printf ("pi: %f\n", pi);

}
