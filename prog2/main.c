/* ----------------------------------------------------------- */
/* NAME : Veronica Yurek                     User ID: vryurek  */
/* DUE DATE : 10/19/2018                                       */
/* PROGRAM ASSIGNMENT #2                                       */
/* FILE NAME : main.c                                          */
/* PROGRAM PURPOSE :                                           */
/*      This program recieves an array of size n and forks,    */
/*			calling merge.c to implement mergesort concurrently    */
/*			It also creates a shared output array and shared       */
/*			temporary array.                                       */
/* ----------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/ipc.h>

/* ----------------------------------------------------------- */
/* FUNCTION  main :                                            */
/*     the purpose of this function                            */
/* PARAMETER USAGE :                                           */
/*    void                                                     */
/* FUNCTIONS CALLED :                                          */
/*    scanf() - reads input from the console or file           */
/*    sprintf() - writes output to a buffer                    */
/*    ftok() - returns a shared memeory key                    */
/*    write() - writes from a buffer to the console            */
/*    shmget() - returns a shared memory id using a key        */
/*    shmat() - attaches shared memory                         */
/*    fork() - creates an independent child process            */
/*    execvp() - launches another file with command args       */
/*    wait() - joins processes (waits for one+ to finish)      */
/*    shmdt() - detach shared memory                           */
/*    shmctl() - removes shared memory segment                 */
/*    a list of functions that are called by this one          */
/* ----------------------------------------------------------- */
int main(void) {
	int n, i;				/*size, counter*/
	scanf("%d", &n);	/*scan in size*/

	int a[n];	/*temp local array*/

	char buf[300];	/*buffer for sprintf and write*/

	int *shm, *zArr;	/*shared output array, shared temp array*/
	int shmid, zID;	/*shared output array id, shared temp array id*/
	key_t key;	/*shared output array key*/

	/*temps for argv*/
	char prog[8];
	char tempLeft[10], tempRight[10], tempTemp[10], tempShare[10], tempSize[10];


	pid_t child;

	/*create shared output memory array*/
	key = ftok("./", 'x');
	sprintf(buf, "*** MAIN: Output shared memory key = %d\n", key);
	write(1, buf, strlen(buf));
	shmid = shmget(key, n * sizeof(int), IPC_CREAT | 0666);
	sprintf(buf, "*** MAIN: Output shared memory created\n");
	write(1, buf, strlen(buf));
	shm = (int *) shmat(shmid, NULL, 0);
	sprintf(buf, "*** MAIN: Output shared memory is"
			" attached and ready to use\n\n");
	write(1, buf, strlen(buf));

	/*create shared temporary memory array*/
	key_t zKey = ftok("./", 'z');
	sprintf(buf, "*** MAIN: Temp shared memory key = %d\n", zKey);
	write(1, buf, strlen(buf));
	zID = shmget(zKey, n * sizeof(int), IPC_CREAT | 0666);
	sprintf(buf, "*** MAIN: Temp shared memory created\n\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "*** MAIN: Input array for mergesort has %d elements:\n", n);
	for (i = 0; i < n; i = i + 1) {
		scanf("%d", &a[i]);
		shm[i] = a[i];
		sprintf(buf + strlen(buf), "   %d", shm[i]);
	}
	sprintf(buf + strlen(buf), "\n\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "*** MAIN: About to spawn the merge sort process\n");
	write(1, buf, strlen(buf));

	/*create child process*/
	child = fork();
	if (child < 0) {
		exit(-1);
	}
	else if (child == 0) {
		sprintf(prog, "./merge");
		sprintf(tempLeft, "%d", 0);
		sprintf(tempRight, "%d", n - 1);
		sprintf(tempShare, "%d", shmid);
		sprintf(tempTemp, "%d", zID);
		sprintf(tempSize, "%d", n);

		char *argv2[] = { prog, tempLeft, tempRight, tempShare,
				tempTemp, tempSize, '\0' };
		/*run merge.c*/
		execvp(prog, argv2);
		sprintf(buf, "*** MAIN: execvp error\n");
		write(1, buf, strlen(buf));
	}
	else {
		wait(&child);	/*wait for merge.c to finish*/

		/*print sorted and merged array*/
		sprintf(buf, "*** MAIN: Merged array:\n   ", n);
		for (i = 0; i < n; i = i + 1) {
			sprintf(buf + strlen(buf), "   %d", shm[i]);
		}
		sprintf(buf + strlen(buf), "\n\n");
		write(1, buf, strlen(buf));

		/*delete shared memory segments*/
		shmdt(shm);
		sprintf(buf, "*** MAIN: Output shared memory successfully detached\n");
		write(1, buf, strlen(buf));
		shmctl(shmid, IPC_RMID, NULL);
		sprintf(buf, "*** MAIN: Output shared memory successfully removed\n\n");
		write(1, buf, strlen(buf));

		shmctl(zID, IPC_RMID, NULL);
		sprintf(buf, "*** MAIN: Temp shared memory successfully removed\n\n");
		write(1, buf, strlen(buf));


	}

	/*exit*/
	sprintf(buf, "*** MAIN: exits\n");
	write(1, buf, strlen(buf));
	return 0;
}
