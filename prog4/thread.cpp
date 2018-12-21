// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/16/2018
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//
//
// -----------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include "thread.h"

using namespace std;

// -----------------------------------------------------------
// FUNCTION  Landlord
//     Constructor that initializes variables and prints
//		thread creation message for landlord.
// PARAMETER USAGE :
//		limit:	max number of students allowed
//		numChecks:	number of checks landlord makes
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
Landlord::Landlord(int limit, int numChecks, int k) : limit(limit), numChecks(numChecks), k(k) {

	ThreadName << "Landlord";
	char buffer[300];
	sprintf(buffer, "Landlord starts\n");
	write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// Landlord class body:
//
// -----------------------------------------------------------
void Landlord::ThreadFunc() {
	Thread::ThreadFunc();
	char buffer[300];
	for (i = 1; i <= numChecks; i++) {	// m is an input value
		Delay();				// take some rest
		CheckRoom(i, limit, numChecks, k);		// check the social room
		Delay();				// take some rest
	}

	sprintf(buffer, "After checking the room %d times, the landlord retires and is on vacation!\n", numChecks);
	write(1, buffer, strlen(buffer));
	Exit();
}

// -----------------------------------------------------------
// FUNCTION  Student
//     Constructor that initializes variables and prints
//		thread creation message for student threads.
// PARAMETER USAGE :
//		num: counter for number of students
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
Student::Student(int num) : num(num) {
	ThreadName << "Student " << num;
	char buffer[300];
	sprintf(buffer, "      Student %d starts\n", num);
	write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// Student class body:
//
// -----------------------------------------------------------
void Student::ThreadFunc() {
	Thread::ThreadFunc();
	char buffer[300];

	while (1) {
		Delay();			// study for a while
		if (GoToParty(num)) {	//if true, landlord is retired and student exits
			Exit();
		}		// go to the party
		Delay();
					// well, everyone needs some sleep
	}
}
