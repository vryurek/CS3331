// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/16/2018
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread-support.cpp
// PROGRAM PURPOSE :
//				This file contains the implementations of the
//			CheckRoom and GoToParty functions.
//
// -----------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <cstring>
#include "thread.h"

using namespace std;

static int students = 0;	//count of students currently in the party
static string landlord = "not_here";	//state of the landlord
static bool isRetired = false;	//whether the landlord is retired

static Semaphore Mutex("Mutex", 1);	//for protecting the number of students
static Semaphore Clear("Clear", 0);	//notifying landlord when room is clear
static Semaphore Bed("Bed", 0);	//notifying landlord when students have ended

// -----------------------------------------------------------
// FUNCTION  CheckRoom
//     simulates the landlord checking the room and forcing students to leave
//			if there are too many or if the landlord is retiring.
// PARAMETER USAGE :
//		i:	the ith time the landlord has checked the in_room
//		limit: the max number of students allowed to GoToParty
//		numChecks:	the total number of checks he makes
//		k:	The total number of students
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
//		Wait():	semaphore waits if < 0
//		Signal(): semaphore resumes a waiting process (depending)
// -----------------------------------------------------------
void CheckRoom(int i, int limit, int numChecks, int k) {
	char buffer[300];
	int j;	//counter
	Mutex.Wait();	//lock down number of students
	landlord = "in_room";

	sprintf(buffer, "The landlord checks the room the %d time\n", i);
	write(1, buffer, strlen(buffer));

	//if retiring and no more students in room
	if (students == 0 && i == numChecks) {
		sprintf(buffer, "The landlord finds the room has no students and leaves so he can retire.\n");
		write(1, buffer, strlen(buffer));

		isRetired = true;
	}
	else if (i == numChecks) {	//if retiring and students exist
		sprintf(buffer, "The landlord finds %d students in the room and breaks up the party so he can retire\n", students);
		write(1, buffer, strlen(buffer));
		sprintf(buffer, "The landlord finishes checking and forces everyone to leave\n");
		write(1, buffer, strlen(buffer));

		Mutex.Signal();	//let students leave
		Clear.Wait();		//wait for all students to leave

		sprintf(buffer, "The landlord leaves the room and the room is empty\n");
		write(1, buffer, strlen(buffer));

		isRetired = true;
	}
	else if (students > limit) {	//if more students than allowed
		sprintf(buffer, "The landlord finds %d students in the room and breaks up the party\n", students);
		write(1, buffer, strlen(buffer));
		sprintf(buffer, "The landlord finishes checking and forces everyone to leave\n");
		write(1, buffer, strlen(buffer));

		Mutex.Signal();	//let students leave
		Clear.Wait();		//wait for students to leave

		sprintf(buffer, "The landlord leaves the room and the room is empty\n");
		write(1, buffer, strlen(buffer));
	}
	else if (students == 0) {	//if no students in the room
		sprintf(buffer, "The landlord finds the room has no students and leaves.\n");
		write(1, buffer, strlen(buffer));
	}
	else {	//otherwise ok
		sprintf(buffer, "The landlord finds there are %d students in the room (condition being good) and leaves.\n", students);
		write(1, buffer, strlen(buffer));
	}

		landlord = "not_here";	//no longer in room

		Mutex.Signal();	//release lock

		if (isRetired) {	//if retired, wait for termination
			for (j = 0; j < k; j++) {
				Bed.Wait();
			}
		}


}

// -----------------------------------------------------------
// FUNCTION  GoToParty
//     simulates the students entering an leaving the party
// PARAMETER USAGE :
//		num:	the number of the student
//RETURN:
//		true if the landlord is retired, otherwise false
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
//		Wait():	semaphore waits if < 0
//		Signal(): semaphore resumes a waiting process (depending)
// -----------------------------------------------------------
bool GoToParty(int num) {
	char buffer[300];
	if (isRetired) {	//if retired, signal Bed and return true
		sprintf(buffer, "      Student %d terminates\n", num);
		write(1, buffer, strlen(buffer));
		Bed.Signal();
		return true;
	}
	sprintf(buffer, "      Student %d waits to enter the room\n", num);
	write(1, buffer, strlen(buffer));
	Mutex.Wait();	//wait to enter
	while (landlord == "in_room") {	//if in room, keep waiting
		Mutex.Signal();
		Mutex.Wait();
	}

	if (isRetired) {	//if retired, signal Bed and return true
		Mutex.Signal();
		sprintf(buffer, "      Student %d terminates\n", num);
		write(1, buffer, strlen(buffer));
		Bed.Signal();
		return true;
	}
	students += 1;	//enter
	sprintf(buffer, "      Student %d enters the room (%d students in the room)\n", num, students);
	write(1, buffer, strlen(buffer));
	Mutex.Signal();

	//party time

	sprintf(buffer, "      Student %d waits to leave the room\n", num);
	write(1, buffer, strlen(buffer));
	Mutex.Wait();
	students -= 1;	//leave room

	sprintf(buffer, "      Student %d leaves the room (%d students in the room)\n", num, students);
	write(1, buffer, strlen(buffer));

	//if last to leave when landlord is in room, signal Clear
	if (students == 0 && (landlord == "in_room")) {
		Clear.Signal();
	}
	else {
		Mutex.Signal();
	}

	if (isRetired) {	//if retired, signal Bed and return true
		sprintf(buffer, "      Student %d terminates\n", num);
		write(1, buffer, strlen(buffer));
		Bed.Signal();
		return true;
	}

	return false;	//otherwise, landlord is not retired so keep looping
}
