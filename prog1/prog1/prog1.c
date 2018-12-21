/* ----------------------------------------------------------- */
/* NAME : Veronica Yurek                     User ID: vryurek  */
/* DUE DATE : 09/29/2018                                       */
/* PROGRAM ASSIGNMENT #1                                       */
/* FILE NAME : prog1.c                                         */
/* PROGRAM PURPOSE :                                           */
/*   This program receives 7 integer (n, r, a, b, s, x, y)     */
/*   command line arguments and creates four child processes   */
/*   that complete the following functions concurrently:       */
/*     1) Computes the nth fibonacci term using recursion      */
/*     2) Computes Buffon's needle problem given r throws      */
/*     3) Estimates the area of an Ellipse with axises a and b */
/*        given s points                                       */
/*     4) Calculates the probability of the number of balls in */
/*        each bin in a simple pinball game given x bins and y */
/*        balls dropped.                                       */
/* ----------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

/*define PI because c89 does not allow PI from math.h*/
#define PI 3.14159265358979323846264338327950288

/* ----------------------------------------------------------- */
/* FUNCTION  fib   :                                           */
/*     Calculates the nth fibonacci term using recursion       */
/* PARAMETER USAGE :                                           */
/*    long n - the target nth term in the sequence             */
/* FUNCTION CALLED :                                           */
/*    fib(long n) - calculates the nth fibonacci term          */
/* ----------------------------------------------------------- */
long fib(long n) {
  if(n == 1)			/*base case*/
    return 1;
  else if(n == 2)		/*base case*/
    return 1;
  else
    return fib(n-1) + fib(n-2);	/*solve recursively*/
}

/* ----------------------------------------------------------- */
/* FUNCTION  printFib   :                                      */
/*     Prints the input and results of the fib function        */
/* PARAMETER USAGE :                                           */
/*    int n - the target nth term in the sequence              */
/* FUNCTIONS CALLED :                                          */
/*    fib(long) - calculates the nth fibonacci term            */
/*    sprintf(char *, const char *,..) - print output to bufer */
/*    write(int, const void*, size_t) - output from buffer     */
/* ----------------------------------------------------------- */
void printFib(int n) {
  char buf[300];	/*output buffer*/

  /*print start and input*/
  sprintf(buf, "   Fibonacci Process Started\n");
  write(1, buf, strlen(buf));

  sprintf(buf, "   Input Number %ld\n", n);
  write(1, buf, strlen(buf));

  /*call recursive function*/
  sprintf(buf, "   Fibonacci Number f(%d) is %ld\n", n, fib((long) n));
  write(1, buf, strlen(buf));

  sprintf(buf, "   Fibonacci Process Exits\n");
  write(1, buf, strlen(buf));
}

/* ----------------------------------------------------------- */
/* FUNCTION  needle   :                                        */
/*     finds the solution to Buffon's needle problem           */
/* PARAMETER USAGE :                                           */
/*    int r - number of times the needle is thrown             */
/*   FUNCTIONS CALLED :                                        */
/*    srand(unsigned int) - seed random number generator       */
/*    sprintf(char *, const char *,..) - print output to bufer */
/*    write(int, const void*, size_t) - output from buffer     */
/*    rand() - generate random integer                         */
/*    sin(double) - calulate the sine                          */
/* ----------------------------------------------------------- */
void needle(int r) {
  srand(time(NULL));	/*seed rand*/
  char buf[300];		/*output buffer*/

  /*print start and input*/
  sprintf(buf, "      Buffon's Needle Process Started\n");
	write(1, buf, strlen(buf));
  sprintf(buf, "      Input Number %d\n", r);
	write(1, buf, strlen(buf));

  int t = 0;	/*number of times it crosses dividing line*/
  int i;		/*counter*/
  float d, a , res;	/*random numbers, estimated probability*/

  /*loop r times and calculate the number of hits (t)*/
  for (i = 0; i < r; i = i + 1) {
    d = ((float) rand())/((float) RAND_MAX);
    a = ( ((float) rand()) / ((float) RAND_MAX) ) * 2 * PI;
    res = d + (float) sin((double) a);
    if (res < 0 || res > 1) {
      t = t + 1;
    }
  }

  /*print estimated probability and exit*/
  sprintf(buf, "      Estimated Probability is %.5f\n", ((float)t / (float)r));
  write(1, buf, strlen(buf));
  sprintf(buf, "      Buffon's Needle Process Exits\n");
  write(1, buf, strlen(buf));
}

/* ----------------------------------------------------------- */
/* FUNCTION  ellipse   :                                       */
/*     Estimates the area of an ellipse                        */
/* PARAMETER USAGE :                                           */
/*    int s - number of random points                          */
/*    int a - semi-major axis of the ellipse                   */
/*    int b - semi-minor axis of the ellipse                   */
/*   FUNCTIONS CALLED :                                        */
/*    sprintf(char *, const char *,..) - print output to bufer */
/*    write(int, const void*, size_t) - output from buffer     */
/*    rand() - generate random integer                         */
/*    srand(unsigned int) - seed random number generator       */
/* ----------------------------------------------------------- */
void ellipse(int s, int a, int b) {
  char buf[300];	/*output buffer*/

  /*print start*/
  sprintf(buf, "         Ellipse Area Process Started\n");
  write(1, buf, strlen(buf));

  int t = 0, i;	/*number of hits, counter*/
  float res, actualRes, x, y;	/*estimated area, actual area, random numbers*/

  /*print inputs*/
  sprintf(buf, "         Total Random Number Pairs %d\n", s);
  write(1, buf, strlen(buf));
  sprintf(buf, "         Semi-Major Axis Length %d\n", a);
  write(1, buf, strlen(buf));
  sprintf(buf, "         Semi-Minor Axis Length %d\n", b);
  write(1, buf, strlen(buf));

  srand(time(NULL));	/*seed*/

  /*loop s times and calculate number of hits*/
  for (i = 0; i < s; i = i + 1) {
	   x = (((float)rand()) / ((float)RAND_MAX)) * a;
	    y = (((float)rand()) / ((float)RAND_MAX)) * b;
	     if ((((x * x) / (a * a)) + ((y * y) / (b * b))) <= 1) {
		       t = t + 1;
        }
  }

  /*Print results*/
  sprintf(buf, "         Total Hits %d\n", t);
  write(1, buf, strlen(buf));

  res = ((((float)t / (float)s) * a * b) * 4);
  sprintf(buf, "         Estimated Area is %.5f\n", res);
  write(1, buf, strlen(buf));

  actualRes = PI * a * b;
  sprintf(buf, "         Actual Area is %.5f\n", actualRes);
  write(1, buf, strlen(buf));

  sprintf(buf, "         Ellipse Area Process Exits\n");
  write(1, buf, strlen(buf));
}

/* ----------------------------------------------------------- */
/* FUNCTION  pinball   :                                       */
/*     Estimates probability of a simple pinball game          */
/* PARAMETER USAGE :                                           */
/*    int x - the number of bins                               */
/*    int y - the number of ball drops                         */
/* FUNCTIONS CALLED :                                          */
/*    sprintf(char *, const char *,..) - print output to bufer */
/*    write(int, const void*, size_t) - output from buffer     */
/*    rand() - generate random integer                         */
/*    srand(unsigned int) - seed random number generator       */
/* ----------------------------------------------------------- */
void pinball(int x, int y) {
	srand(time(NULL));	/*seed*/
	char buf[1000];		/*output buffer*/

	/*print start and inputs*/
  sprintf(buf, "Simple Pinball Process Started\n");
  write(1, buf, strlen(buf));
  sprintf(buf, "Number of Bins %d\n", x);
  write(1, buf, strlen(buf));
  sprintf(buf, "Number of Ball Droppings %d\n", y);
	write(1, buf, strlen(buf));
  buf[0] = 0;	/*flush buffer*/

	/*counter, counter, number of a bin, bit (0 or 1),
  number of stars in histogram*/
	int i, j, k, t, bit, hist;
	float max;	/*max percent of balls dropped in a bin */
	int bin[x];	/*array of bins*/
	float percentBin[x];	/*array of the percent of balls in each bin*/

	/*initialize arrays*/
	for (k = 0; k < x; k = k + 1) {
		bin[k] = 0;
		percentBin[k] = 0;
	}

	/*determine which bin the balls drop into*/
	for (i = 0; i < y; i = i + 1) {	/*for each ball dropped*/
		t = 1;	/*start at bin 1*/
		for (j = 0; j < (x - 1); j = j + 1) {	/*x - 1 decisions*/
			bit = rand() % 2;
			if (bit == 1) {	/*if 1, go to next bin. Otherwise no change*/
				t = t + 1;
			}
		}
		bin[t - 1] = (bin[t - 1] + 1);	/*save bin number to array*/

    /*save percent to array*/
		percentBin[t - 1] = ((float)bin[t - 1] / (float)y) * 100;
	}

	/*find max percent*/
	max = percentBin[0];
	for (k = 0; k < x; k = k + 1) {
		percentBin[k] = ((float)bin[k] / (float)y) * 100;
		if (percentBin[k] > max)
			max = percentBin[k];
	}

	/*print histogram*/
	for (k = 0; k < x; k = k + 1) {
    hist = (percentBin[k] / max) * 50;	/*calculate number of stars*/
		sprintf(buf, "%3d - (%7d) - (%5.2f%%) | ", (k + 1), bin[k], percentBin[k]);
		for (j = 0; j < hist; j = j + 1) {	/*print stars to buffer*/
			sprintf(buf + strlen(buf), "*");
		}
		sprintf(buf + strlen(buf), "\n");
		write(1, buf, strlen(buf));
	}


	sprintf(buf, "Simple Pinball Process Exits\n");	/*end of process*/
	write(1, buf, strlen(buf));
}

/* ----------------------------------------------------------- */
/* FUNCTION  main   :                                          */
/*     Main function of the program                            */
/* PARAMETER USAGE :                                           */
/*    int argc - the nubmer of command line arguments + 1      */
/*    char *argv[] - array of command line arguments           */
/* FUNCTION CALLED :                                           */
/*    printFib - Prints the input and results of the fib       */
/*              function                                       */
/*    needle - finds the solution to Buffon's needle problem   */
/*    ellipse - Estimates the area of an ellipse               */
/*    pinball - Estimates probability of a simple pinball game */
/*    sprintf(char *, const char *,..) - print output to bufer */
/*    write(int, const void*, size_t) - output from buffer     */
/*    fork() - creates a clone process that runs independently */
/*    wait(&pid_t) - waits for the specified process to exit   */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
  char buf[500];	/*output buffer*/

  /*process start*/
  sprintf(buf, "Main Process Started\n");
  write(1, buf, strlen(buf));

	if (argc != 8) {	/*invalid arg check*/
		printf("Wrong number of arguments. Rerun and enter 7 integer arguments.");
		exit(1);
	}

	int i;	/*counter*/
	pid_t pidArr[4];	/*array of process id's (for child processes)*/

	/*get command line arguments*/
	int n = atoi(argv[1]);
  int r = atoi(argv[2]);
  int s = atoi(argv[5]);
  int a = atoi(argv[3]);
  int b = atoi(argv[4]);
  int x = atoi(argv[6]);
  int y = atoi(argv[7]);

  /*invalid arg check*/
  if (n <= 0 || r < 0 || s < 0 || a <= 0 || b <= 0 || x < 0 || y < 0) {
    sprintf(buf, "No negative arguments (or zero for n, a, or b)."
        " Rerun and enter 7 integer arguments\n");
    write(1, buf, strlen(buf));
  }

  /*print inputs*/
  sprintf(buf, "Fibonacci input            = %d\n", n);
  write(1, buf, strlen(buf));
  sprintf(buf, "Buffon's Neddle Iterations = %d\n", r);
  write(1, buf, strlen(buf));
  sprintf(buf, "Total Random Number Pairs  = %d\n", s);
  write(1, buf, strlen(buf));
  sprintf(buf, "Semi-Major Axis Length     = %d\n", a);
  write(1, buf, strlen(buf));
  sprintf(buf, "Semi-Minor Axis Length     = %d\n", b);
  write(1, buf, strlen(buf));
  sprintf(buf, "Number of bins             = %d\n", x);
  write(1, buf, strlen(buf));
  sprintf(buf, "Number of Ball Droppings   = %d\n", y);
  write(1, buf, strlen(buf));

  /*loop to create 4 child processes*/
  for (i = 0; i < 4; i = i + 1) {
	  if ((pidArr[i] = fork()) < 0) {	/*fail to fork*/
		  sprintf(buf, "Fork failed. Please rerun.\n");
		  write(1, buf, strlen(buf));
		  exit(1);
	  }
	  else if (pidArr[i] == 0) {	/*if process is a child*/
		  if (i == 0) {				/*first child process*/
			  printFib(n);
			  exit(0);
		  }
		  else if (i == 1) {		/*second child process*/
			  needle(r);
			  exit(0);
		  }
		  else if (i == 2) {		/*third child process*/
			  ellipse(s, a, b);
			  exit(0);
		  }
		  else if (i == 3) {		/*fourth child process*/
			  pinball(x, y);
			  exit(0);
		  }
	  }
	  else {						/*parent does this*/
		  if (i == 0) {	/*after first fork*/
			  sprintf(buf, "Fibonacci Process Created\n");
			  write(1, buf, strlen(buf));
		  }
		  else if (i == 1) {	/*after second fork */
			  sprintf(buf, "Buffon's Needle Process Created\n");
			  write(1, buf, strlen(buf));
		  }
		  else if (i == 2) {	/*after third fork*/
			  sprintf(buf, "Ellipse Area Process Created\n");
			  write(1, buf, strlen(buf));
		  }
		  else if (i == 3) {	/*after fourth fork*/
			  sprintf(buf, "Pinball Process Created\n");
			  write(1, buf, strlen(buf));
		  }
	  }
  }

  /*wait for children to finish processing*/
  sprintf(buf, "Main Process Waits\n");
  write(1, buf, strlen(buf));

  wait(&pidArr[0]);
  wait(&pidArr[1]);
  wait(&pidArr[2]);
  wait(&pidArr[3]);

  /*exit parent*/
  sprintf(buf, "Main Process Exits\n");
  write(1, buf, strlen(buf));

}
