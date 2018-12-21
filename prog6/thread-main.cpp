// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/14/2018
// PROGRAM ASSIGNMENT #6
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//		Given input matrices A and B, this 
//		program calculates the matrix C = A * B using
//		synchronous message passing.
// -----------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include "thread.h"		//header file for thread

using namespace std;

SynOneToOneChannel* rChannels[100][100];	//right messages
SynOneToOneChannel* dChannels[100][100];	//down messages
int matrixA[100][100];
int matrixB[100][100];
int matrixC[100][100];

// -----------------------------------------------------------
// FUNCTION  getMatrix
//		scans in a matrix from an input file
// COMMAND ARGS:
//		rows:	the number of rows in the matrix
//		cols:	the number of columns
//		type:	whether it is matrix A or B
// FUNCTIONS CALLED :
//		scanf():	reads in input
// -----------------------------------------------------------
void getMatrix(int rows, int cols, int type) {
	int m, n;
	for (m = 0; m < rows; m++) {
		for (n = 0; n < cols; n++) {
			if (type == 0) {	//if matrix A
				scanf("%d", &matrixA[m][n]);
			}
			else {	//otherwise matrix B
				scanf("%d", &matrixB[m][n]);
			}
		}
	}
}

// -----------------------------------------------------------
// FUNCTION  main
//	main function of the program
// COMMAND ARGS:
//		input file
// FUNCTIONS CALLED :
//		Begin():	begins a thread
//		Join():	joins a terminated thread to the caller
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
//		getMatrix():	scans in a matrix
// -----------------------------------------------------------
int main(int argc, char *argv[]) {
	char buffer[300];
	int i, j;
	int aRows, aCols, bRows, bCols, cRows, cCols;

	//get matrix A
	scanf("%d", &aRows);
	scanf("%d", &aCols);
	getMatrix(aRows, aCols, 0);

	//get matrix B
	scanf("%d", &bRows);
	scanf("%d", &bCols);
	getMatrix(bRows, bCols, 1);

	if (aCols != bRows) {	//can't compute
		printf("ERROR: The matrices cannot be multiplied. Exiting now.\n");
		exit(1);
	}

	//get C's dimensions
	cRows = aRows;
	cCols = bCols;

	//create channels
	for (i = 0; i <= cRows; i++) {
		for (j = 0; j <= cCols; j++) {
			rChannels[i][j] = new SynOneToOneChannel("rChannel", 0, 0);
			dChannels[i][j] = new SynOneToOneChannel("dChannel", 0, 0);
		}
	}

	//start row processor threads
	RowProcessor* rows[cRows];
	for (i = 0; i < cRows; i++) {
		rows[i] = new RowProcessor(i + 1, aCols);
		rows[i]->Begin();
	}

	//start column processor threads
	ColProcessor* cols[cCols];
	for (i = 0; i < cCols; i++) {
		cols[i] = new ColProcessor(i + 1, bRows);
		cols[i]->Begin();
	}

	//start inner (P) processor threads
	InnerProcessor* inners[cRows][cCols];
	for (i = 0; i < cRows; i++) {
		for (j = 0; j < cCols; j++) {
			inners[i][j] = new InnerProcessor(i + 1, j + 1, cRows, cCols);
			inners[i][j]->Begin();
		}
	}

	//wait for rows to join
	for (i = 0; i < cRows; i++) {
		rows[i]->Join();
	}

	//wait for cols to join
	for (i = 0; i < cCols; i++) {
		cols[i]->Join();
	}

	//wait for inners to join
	for (i = 0; i < cRows; i++) {
		for (j = 0; j < cCols; j++) {
			inners[i][j]->Join();
		}
	}

	//print results
	sprintf(buffer, "*** From Main ***\n");
	write(1, buffer, strlen(buffer));

	//print matrix A
	sprintf(buffer, "Matrix A: %2d rows and %2d columns\n", aRows, aCols);
	write(1, buffer, strlen(buffer));
	for (i = 0; i < aRows; i++)
	{
		for (j = 0; j < aCols; j++)
		{
			sprintf(buffer, "%4d ", matrixA[i][j]);
			write(1, buffer, strlen(buffer));
		}
		sprintf(buffer, "\n");
		write(1, buffer, strlen(buffer));
	}

	//print matrix B
	sprintf(buffer, "Matrix B: %2d rows and %2d columns\n", bRows, bCols);
	write(1, buffer, strlen(buffer));
	for (i = 0; i < bRows; i++)
	{
		for (j = 0; j < bCols; j++)
		{
			sprintf(buffer, "%4d ", matrixB[i][j]);
			write(1, buffer, strlen(buffer));
		}
		sprintf(buffer, "\n");
		write(1, buffer, strlen(buffer));
	}

	//print matrix C
	sprintf(buffer, "Matrix C = A * B: %2d rows and %2d columns\n", cRows, cCols);
	write(1, buffer, strlen(buffer));
	for (i = 0; i < cRows; i++)
	{
		for (j = 0; j < cCols; j++)
		{
			sprintf(buffer, "%4d ", matrixC[i][j]);
			write(1, buffer, strlen(buffer));
		}
		sprintf(buffer, "\n");
		write(1, buffer, strlen(buffer));
	}

	return 0;	//exit
}
