/* ----------------------------------------------------------- */
/* NAME : Veronica Yurek                     User ID: vryurek  */
/* DUE DATE : 09/29/2018                                       */
/* PROGRAM ASSIGNMENT #1                                       */
/* FILE NAME : prog1.cs                                        */
/* PROGRAM PURPOSE :                                           */
/*    A couple of lines describing your program briefly        */
/* ----------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
void main(int argc, char **argv)
{
     int i, n = atoi(argv[1]);

     for (i = 0; i < n; i++)
          if (fork() <= 0)
               break;
     printf("Process %ld with parent %ld\n", getpid(), getppid());
     sleep(1);
}
