/* ----------------------------------------------------------- */
/* NAME : Veronica Yurek                     User ID: vryurek  */
/* DUE DATE : 10/15/2018                                       */
/* PROGRAM ASSIGNMENT #1                                       */
/* FILE NAME : merge.c                                         */
/* PROGRAM PURPOSE :                                           */
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
/*    Recursively forks in half and uses a binary merge method */
/*    to merge the processes together in a shared array        */
/* PARAMETER USAGE :                                           */
/*    val: the comparison value                                */
/*    left: the left index of the array                        */
/*    right: the rightmost index of the array                  */
/*    *temp: the shared output array                           */
/* FUNCTIONS CALLED :                                          */
/*    bSearch() - recursively searches for a value k to        */
/*            position in a value in the shared temp array     */
/* ----------------------------------------------------------- */
int bSearch(int val, int left, int right, int *temp) {
	int mid = (right + left) / 2;	/*middle index*/
	if (right <= left) {	/*base case*/
		if (val < temp[left]) {
			return left;
		} else {
			return left + 1;
		}
	}
	if (val < temp[mid]) {	/*otherwise, recurse*/
		return bSearch(val, left, mid, temp);
	} else {
		return bSearch(val, mid + 1, right, temp);
	}
}

/* ----------------------------------------------------------- */
/* FUNCTION  main :                                            */
/*    Recursively forks in half and uses a binary merge method */
/*    to merge the processes together in a shared array        */
/* PARAMETER USAGE :                                           */
/*    prgname, left index, right index, shared output array id*./
/*    shared temp array id, total initial array size           */
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
/*    bSearch() - recursively searches for a value k to        */
/*            position in a value in the shared temp array     */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	/*retrieve command args*/
	int left = atoi(argv[1]);
	int right = atoi(argv[2]);
	int shmID = atoi(argv[3]);
	int zID = atoi(argv[4]);
	int fullSize = atoi(argv[5]);
	int i;
	char buf[300];	/*buffer for output*/

	int n = (right - left + 1);	/*size of current array*/
	int *shmArr, *zArr;	/*shared output array, shared temp array*/

	char prog[8];

	/*Attach output memory*/
	shmArr = (int *) shmat(shmID, NULL, 0);
	sprintf(buf, "   ### M-PROC(%d) Output shared memory is attached and ready"
			"\n\n", getpid());
	write(1, buf, strlen(buf));

	/*Attach temp memory*/
	zArr = (int *) shmat(zID, NULL, 0);
	sprintf(buf, "   ### M-PROC(%d) temp shared memory is attached and ready "
			"to use for temporary storage\n\n", getpid());
	write(1, buf, strlen(buf));

	/*if the first array*/
	if ((right - left + 1) == fullSize) {
		sprintf(buf, "   ### M-PROC(%d): entering with a[%d..%d]\n   ",
							getpid(), left, right);
		for (i = left; i <= right; i = i + 1) {
			sprintf(buf + strlen(buf), "   %d", shmArr[i]);
		}
		sprintf(buf + strlen(buf), "\n\n");
		write(1, buf, strlen(buf));
	} else {	/*otherwise, just print normally*/
		sprintf(buf, "   ### M-PROC(%d) created by M-PROC(%d): "
					"entering with a[%d..%d]\n   ", getpid(), getppid(), left, right);

		for (i = left; i <= right; i = i + 1) {
			sprintf(buf + strlen(buf), "   %d", shmArr[i]);
		}
		sprintf(buf + strlen(buf), "\n\n");
		write(1, buf, strlen(buf));
	}

	int mid = (right + left) / 2;	/*current middle index*/
	int temp;

	/*if array size is two, do a compare and simple swap*/
	pid_t leftChild, rightChild;
	if ((right - left) == 1) {
		if (shmArr[right] < shmArr[left]) {
			temp = shmArr[right];
			shmArr[right] = shmArr[left];
			shmArr[left] = temp;
			sprintf(buf, "   ### M-PROC(%d) created by M-PROC(%d): entering with "
						"with a[%d..%d] -- sorted\n   ", getpid(), getppid(), left, right);

			for (i = left; i <= right; i = i + 1) {
				sprintf(buf + strlen(buf), "   %d", shmArr[i]);
			}
			sprintf(buf + strlen(buf), "\n\n", left, right);
			write(1, buf, strlen(buf));
		}

		exit(0); /*exit to wait*/
	}

		/*otherwise, fork and call execvp recursively (split in half)*/
		if ((leftChild = fork()) < 0) {	/*left fork*/
			sprintf(buf, "   ### MEREGE: fork error\n");
			write(1, buf, strlen(buf));
			exit(-1);
		}
		else if (leftChild == 0) {
			char tempLeft[10], tempRight[10];

			sprintf(prog, "./merge");
			sprintf(tempLeft, "%d", left);
			sprintf(tempRight, "%d", mid);



			char *argvLeft[] = { prog, tempLeft, tempRight, argv[3], argv[4], argv[5], '\0' };

			execvp(prog, argvLeft);
			sprintf(buf, "   ### MEREGE: excvp error\n");
			write(1, buf, strlen(buf));
			exit(-1);

		}


			if ((rightChild = fork()) < 0) {	/*right fork*/
				sprintf(buf, "   ### MEREGE: fork error\n");
				write(1, buf, strlen(buf));
				exit(-1);
			}
			else if (rightChild == 0) {

				char tempLeft[10];
				char tempRight[10];

				sprintf(prog, "./merge");
				sprintf(tempLeft, "%d", mid + 1);
				sprintf(tempRight, "%d", right);

				char *argvRight[] = { prog, tempLeft, tempRight, argv[3], argv[4], argv[5], '\0' };

				/*run merge.c with new args*/
				execvp(prog, argvRight);
				sprintf(buf, "   ### MERGE: execvp error\n");
				write(1, buf, strlen(buf));
				exit(-1);

			}

			/*wait for children to finish*/
			wait(NULL);
			wait(NULL);


	sprintf(buf,"   ### M-PROC:(%d) created by M-PROC(%d): both array section sorted. start merging\n\n",getpid(), getppid());
	write(1, buf, strlen(buf));
	int c;

	/*create a process for each element*/
	for (i = left; i <= right; i++) {
		if ((leftChild = fork()) < 0) {
			exit(-1);
		}
		else if (leftChild == 0) {
			int k, pos;		/*the binary search val, position in array*/
			sprintf(buf,"      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n\n",getpid(), getppid(), i, shmArr[i]);
			write(1, buf, strlen(buf));
			if (i < mid + 1) {	/*if in the lower half of array*/
				sprintf(buf, "Array: \n");
		    for (c = 0; c < fullSize; c++) {
		      sprintf(buf + strlen(buf), "%d\n", shmArr[c]);
		    }
		    sprintf(buf + strlen(buf), "\n");

				/*find k value*/
				k = bSearch(shmArr[i], mid + 1, right, shmArr);
				pos = k + i - left - mid - 1 + left;	/*offset for lower half*/
				zArr[pos] = shmArr[i];
				if (k == mid + 1) {
					sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n\n",getpid(), i, shmArr[i], k, shmArr[k],pos);
				} else if (k == right + 1) {
					sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d]\n\n",getpid(), i, shmArr[i], k-1, shmArr[k-1],pos);
				} else {
					sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n\n",getpid(), i, shmArr[i], k-1, shmArr[k-1], k,shmArr[k],pos);
				}
			} else {	/*if in upper half*/
				/*find k value */
				k = bSearch(shmArr[i], left, mid, shmArr);
				pos = i - mid - 1 + k;	/*offset for right half*/
				zArr[pos] = shmArr[i];
				if (k == left) {
					sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n\n",getpid(), i, shmArr[i], k, shmArr[k], pos);
				} else if (k == mid + 1) {
					sprintf(buf,"      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d]\n\n",getpid(), i, shmArr[i], k-1,shmArr[k-1],pos);
				} else {
					sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n\n",getpid(), i, shmArr[i], k-1,shmArr[k-1],k,shmArr[k], pos);
				}
			}
			write(1, buf, strlen(buf));
			sprintf(buf,"      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is terminated\n\n",getpid(), getppid(), i,shmArr[i]);
			write(1, buf, strlen(buf));
			shmdt((void *) zArr);	/*detach memory segment*/
			return 0;
		}

	}

	/*wait for children to complete*/
	for(i = left; i <= right; i++) {
		wait(&leftChild);
	}

/*assign temp array vals to output array*/
	for(i = 0; i <= right - left + 1; i++) {
		shmArr[i + left] = zArr[i + left];
	}

	if ((right - left + 1) == fullSize) {
		sprintf(buf,"   ### M-PROC(%d) created by M-PROC(%d): merge sort a[%d...%d] completed:\n   ",getpid(), getppid(),left,right);
	} else {
		sprintf(buf,"   ### M-PROC(%d): entering at a[%d...%d] completed:\n   ",getpid(),left,right);
	}
	for(i = left; i <= right; i++){
			sprintf(buf + strlen(buf), "   %d", shmArr[i]);
	}
	sprintf(buf + strlen(buf), "\n\n");
	write(1, buf, strlen(buf));

	/*detach shared memory*/
	shmdt((void *) shmArr);
	return 0;
}
