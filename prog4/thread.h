// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/16/2018
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//		This file contains the class definitions and other
//		definitions needed for thread.cpp, thread-main.cpp,
//		and thread-support.cpp.
//
// -----------------------------------------------------------

#ifndef THREAD_H
#define THREAD_H

#include "ThreadClass.h"

//-----------------------------------------------------------------------
//definition for landlord thread
//-----------------------------------------------------------------------
class Landlord : public Thread {
	public:
		Landlord(int limit, int numChecks, int k);		//constructor
	private:
		int limit;	//max number of students allowed in the room
		int numChecks;	//number of checks the landlord makes
		int k;	//total number of students
		int i;	//counter
		void ThreadFunc();
};

//-----------------------------------------------------------------------
//definition for student threads
//-----------------------------------------------------------------------
class Student : public Thread{
	public:
		Student(int num);	//constructor
	private:
		int num;	//the student number
		void ThreadFunc();
};

//function definitions
void CheckRoom(int i, int n, int m, int k);	//landlord checks the room
bool GoToParty(int num);	//student goes to the party

#endif
