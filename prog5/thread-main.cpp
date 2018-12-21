// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/07/2018
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//		A particular river crossing is shared by both cannibals and missionaries.
//		A single boat is used to cross the river, but it only seats three people,
//		and must always carry a full load. In order to guarantee the safety of
//		the missionaries, you cannot put one missionary and two cannibals in the
//		same boat (because the cannibals would gang up and eat the missionary),
//		but all other combinations are acceptable. This program implements a
//		Hoare style monitor to institute this policy.
// -----------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include "thread.h"		//header file for thread

using namespace std;

// -----------------------------------------------------------
// FUNCTION  main
//     starts the boat thread, cannibal threads, and missionary threads,
//			then waits for them to terminate before exiting the program.
// COMMAND ARGS:
//		c:	the number of cannibal threads
//		m:	the number of missionary threads
//		b:	the number of boat crossings to be made
// FUNCTIONS CALLED :
//		Begin():	begins a thread
//		Join():	joins a terminated thread to the caller
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
int main(int argc, char *argv[]) {
	int b, m, c, i;
	char buffer[300];
	if (argc == 4) {
		c = atoi(argv[1]);
		m = atoi(argv[2]);
		b = atoi(argv[3]);

		//assign defaults if needed
		if (c == 0) {
			c = 8;
		}
		if (m == 0) {
			m = 8;
		}
		if (b == 0) {
			b = 5;
		}
	}
	else {	//wrong inputs
		cout << "Incorrect number of arguments. Please rerun." << endl;
		return -1;
	}

	//begin boat thread
	BoatThread *boat = new BoatThread(b);
	boat->Begin();

	//begin cannibal threads
	CannibalThread *cannibals[c];
	for (i = 0; i < c; i++) {
		cannibals[i] = new CannibalThread(i, b);
		cannibals[i]->Begin();
	}

	MissionaryThread *missionaries[m];
	for (i = 0; i < m; i++) {
		missionaries[i] = new MissionaryThread(i, b);
		missionaries[i]->Begin();
	}



	//join cannibal threads
	for (i = 0; i < c; i++) {
		cannibals[i]->Join();
	}

	//join missionary threads
	for (i = 0; i < m; i++) {
		missionaries[i]->Join();
	}

	//join boat thread
	boat->Join();

	return 0;
}
