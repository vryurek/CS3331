// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/16/2018
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//			landlord comes and checks his apartment frequently to ensure his
//			fantastic social room is being used properly, and breaks up the party
//			if there are too many students in the room.
//			Rules:
//			Any number of students can be in this room at the same time.
//			The landlord enters this room freely.
//			If there are more than n students in the room, where n is an input,
//			the landlord breaks up the party and all students must leave.
//			If there are no students or there are no more than n students in the
//			room, the landlord leaves and comes back later.
//			While the landlord is in the room, no students may enter and no
//			students may leave. The landlord does not leave the room until all
//			students have left if the lanlord breaks up the party. If this is the
//			landlord's last check, all students must leave no matter what. Then,
//			the landlord leaves. Note that only the landlord knows when the last
//			check will occur. There is only one landlord!
// -----------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include "thread.h"		//header file for thread

using namespace std;

int main(int argc, char *argv[]) {
	int n, m, k, i;
	char buffer[300];
	if (argc == 4) {
		m = atoi(argv[1]);
		n = atoi(argv[2]);
		k = atoi(argv[3]);

		//assign defaults if needed
		if (m == 0) {
			m = 5;
		}
		if (n == 0) {
			n = 5;
		}
		if (k == 0) {
			k = 10;
		}
	}
	else {	//wrong inputs
		cout << "Incorrect number of arguments. Please rerun." << endl;
		return -1;
	}

	//begin landlord thread
	Landlord *landlord = new Landlord(n, m, k);
	landlord->Begin();

	//begin student threads
	Student *students[k];
	for (i = 0; i < k; i++) {
		students[i] = new Student(i + 1);
		students[i]->Begin();
	}

	//join student threads
	for (i = 0; i < k; i++) {
		students[i]->Join();
	}

	//join landlord thread
	landlord->Join();

	return 0;
}
