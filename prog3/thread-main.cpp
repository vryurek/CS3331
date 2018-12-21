// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/02/2018
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//		This program computes the prefix sum of a sequence of
//		n integers through the following steps:
//		1. Reads the number of integers n from stdin
//		2. Initialize the 0th row of a 2D array B of k + 1 rows
//			(k=log2(n)) and n columns from input array x[n]
//		3. Iterate k times and create n threads on each
//			iteration to compute a prefix sum
//		4. After k iterations, prefix sum is on B[k][*]
//
// -----------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <cmath>
#include "thread.h"		//header file for thread

using namespace std;

int B[500][500];		//global array B

// -----------------------------------------------------------
// FUNCTION  main
//     Main function of the program. Creates n * k threads
//		to compute the prefix sum of an input sequence.
// PARAMETER USAGE :
//		argc:		The number of command line args + prog name
//		*argv[]:	The list of command line args + prog name
// FUNCTION CALLED :
//		Begin():	starts a created thread
//		Join():		joins threads together with the parent
//		PrefixSum:	creates an instance(thread) of class PrefixSum
// -----------------------------------------------------------
int main(int argc, char *argv[]) {

	int i, j, k, xSize;	//counters, k value (log2(n)), and size of input array

	cin >> xSize;		//read input array size

	k = log2(xSize);	//compute k

	int *x = new int[xSize];

	PrefixSum *threadList[k + 1][xSize];	//array to hold list of threads

	cout << "Concurrent Prefix Sum Computation" << endl;
	cout << "Number of input data = " << xSize << endl << "Input array: " << endl;

	for (i = 0; i < xSize; i++) {	//read in input array
		cin >> x[i];
		cout << "    " << x[i];
	}

	cout << endl << endl << endl;

	for (j = 0; j < xSize; j++) {	//intialize 0th row of B
		B[0][j] = x[j];
	}

	//iterate k times
	for (i = 1; i <= k; i++) {
		cout << "Run " << i << ":" << endl;
		for (j = 0; j < xSize; j++) {			//create and begin n threads
			threadList[i][j] = new PrefixSum(i, j);
			threadList[i][j]->Begin();
		}

		for (j = 0; j < xSize; j++) {			//join the threads
			threadList[i][j]->Join();
		}

		cout << "Result after run " << i << ":" << endl;

		for (j = 0; j < xSize; j++) {			//display result after ith run
			cout << "    " << B[i][j];
		}

		cout << endl << endl << endl;
	}

	cout << "Final Result after run " << k << ":" << endl;		//display final result
	for (i = 0; i < xSize; i++) {
		cout << "    " << B[k][i];
	}

	cout << endl;
	return 0;

}
