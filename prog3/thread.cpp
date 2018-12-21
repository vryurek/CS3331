// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/02/2018
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//		This file contains the code that each thread computes.
//		Each thread calculates a prefix sum pair.
//	
// -----------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include "thread.h"

using namespace std;

// ----------------------------------------------------------- 
// FUNCTION  PrefixSum                          
//     Constructor that initializes variables and prints
//		thread creation message.
// PARAMETER USAGE :                                           
//		i:	counter for k iterations
//		j:	counter for j number of threads	
// FUNCTION CALLED :                                           
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
// ----------------------------------------------------------- 
PrefixSum::PrefixSum (int i, int j) : i(i), j(j) {
	char buffer[300];
	sprintf(buffer, "      Thread %d Created\n", j);
	write(1, buffer, strlen(buffer));
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc                          
//     contains thread implementation
// PARAMETER USAGE :                                           	
// FUNCTION CALLED :                                           
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		log2():		computes the binary log of an integer
//		pow():		computes a power
//		Exit():		terminates a thread
// ----------------------------------------------------------- 
void PrefixSum::ThreadFunc() {
	Thread::ThreadFunc();
	char buffer[300];
	int power = j - pow(2, i - 1);

	if (power < 0) {		//if less than 0, simply copy
		sprintf(buffer, "      Thread %d copies x[%d]\n", j, j);
		write(1, buffer, strlen(buffer));
		B[i][j] = B[i - 1][j];
	}
	else {					//otherwise compute
		sprintf(buffer, "      Thread %d computes x[%d] + x[%d-2^(%d-1)]\n", j, j, j, i);
		write(1, buffer, strlen(buffer));
		B[i][j] = B[i - 1][j] + B[i - 1][power];
	}

	sprintf(buffer, "      Thread %d exits\n", j);
	write(1, buffer, strlen(buffer));
	Exit();				//terminate thread
}