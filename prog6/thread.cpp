//-----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/14/2018
// PROGRAM ASSIGNMENT #6
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//      This program contains the class definitions for the
//       threads.
// -----------------------------------------------------------
#include "thread.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

extern SynOneToOneChannel *rChannels[100][100];
extern SynOneToOneChannel *dChannels[100][100];
extern int matrixA[100][100];
extern int matrixB[100][100];
extern int matrixC[100][100];

// -----------------------------------------------------------
// RowProcessor Constructor
//
// PARAMETER USAGE :
//		id:	the id of the row thread
//		aCols:	num of columns in A
// FUNCTIONS CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
RowProcessor::RowProcessor(int id, int aCols) : id(id), aCols(aCols) {
	char buffer[100];
	sprintf(buffer, "Row thread r[%d] started\n", id);
	write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// RowProcessor Class Body
//
// PARAMETER USAGE :
//		
// FUNCTIONS CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
//		Send():		send a message through a channel
// -----------------------------------------------------------
void RowProcessor::ThreadFunc() {
	Thread::ThreadFunc();

	char buffer[300];
	int k;

	for (k = 0; k < aCols; k++) {	//send messages to innners
		number = matrixA[id - 1][k];
		rChannels[id][1]->Send(&number, sizeof(int));
		
		sprintf(buffer, "Row thread r[%d] sent %d to P[%d][%d]\n", 
			id, number, id, 1);
		write(1, buffer, strlen(buffer));

	}

	number = -1;	//EOD
	rChannels[id][1]->Send(&number, sizeof(int));
	sprintf(buffer, "Row thread r[%d] sent EOD to P[%d][%d] and terminated\n",
		id, id, 1);
	write(1, buffer, strlen(buffer));

	Exit();
}

// -----------------------------------------------------------
// ColProcessor Constructor
//
// PARAMETER USAGE :
//		id: the id of the col processor
//		bRows: num of rows in B
// FUNCTIONS CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
ColProcessor::ColProcessor(int id, int bRows) : id(id), bRows(bRows) {
	char buffer[100];
	sprintf(buffer, "   Column thread c[%d] started\n", id);
	write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// ColProcessor Class Body
//
// PARAMETER USAGE :
//		
// FUNCTIONS CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
//		Send():		send a message through a channel
// -----------------------------------------------------------
void ColProcessor::ThreadFunc() {
	Thread::ThreadFunc();

	char buffer[300];
	int k;

	for (k = 0; k < bRows; k++) {	//send messages to inners
		number = matrixB[k][id - 1];
		dChannels[1][id]->Send(&number, sizeof(int));

		sprintf(buffer, "   Column thread c[%d] sent %d to P[%d, %d]\n",
			id, number, 1, id);
		write(1, buffer, strlen(buffer));

	}

	number = -1;	//EOD
	dChannels[1][id]->Send(&number, sizeof(int));
	sprintf(buffer, "   Column thread c[%d] sent EOD to P[%d, %d] and terminated\n",
		id, 1, id);
	write(1, buffer, strlen(buffer));

	Exit();
}

// -----------------------------------------------------------
// InnerProcessor Constructor
//
// PARAMETER USAGE :
//		rid:	row id
//		cid:	column id
//		cRows:	num of rows in C
//		cCols:	num of cols in C
// FUNCTIONS CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
InnerProcessor::InnerProcessor(int rid, int cid, int cRows, int cCols) : 
	rid(rid), cid(cid), cRows(cRows), cCols(cCols) {
	
	number = 0;	//dot product starts at 0
	char buffer[100];
	sprintf(buffer, "      Thread P[%d, %d] started\n", rid, cid);
	write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// InnerProcessor Class Body
//
// PARAMETER USAGE :
//		
// FUNCTIONS CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
//		Send():		send a message through a channel
//		Receive():	receive a message through a channel
// -----------------------------------------------------------
void InnerProcessor::ThreadFunc() {
	Thread::ThreadFunc();
	char buffer[300];

	while (1) {	//infinite loop
		//receive messages
		dChannels[rid][cid]->Receive(&above, sizeof(int));
		rChannels[rid][cid]->Receive(&left, sizeof(int));

		//EOD
		if (above == -1 && left == -1) {
			if (rid < cRows) {	//pass along down if possible
				dChannels[rid + 1][cid]->Send(&above, sizeof(int));
			}
			if (cid < cCols) {	//pass to the right if possible
				rChannels[rid][cid + 1]->Send(&left, sizeof(int));
			}

			matrixC[rid - 1][cid - 1] = number;	//save to C
			sprintf(buffer, "       Thread P[%d, %d] received EOD, saved result "
				"%d and terminated\n", rid, cid, number);
			write(1, buffer, strlen(buffer));
			Exit();
		}

		sprintf(buffer, "       Thread P[%d, %d] received %d from above and"
			" %d from left\n", rid, cid, above, left);
		write(1, buffer, strlen(buffer));

		if (rid < cRows) {	//pass along down if possible
			dChannels[rid + 1][cid]->Send(&above, sizeof(int));
			sprintf(buffer, "       Thread P[%d, %d] sent %d down to P[%d, %d]\n", 
				rid, cid, above, rid + 1, cid);
		}
		if (cid < cCols) {	//pass to the right if possible
			rChannels[rid][cid + 1]->Send(&left, sizeof(int));
			sprintf(buffer, "       Thread P[%d, %d] sent %d right to P[%d, %d]\n",
				rid, cid, left, rid, cid + 1);
		}

		number += (above * left);	//add to dot product calculation
	}
}