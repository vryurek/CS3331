// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/07/2018
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//		This file contains the class definitions and other
//		definitions needed for thread.cpp
//
// -----------------------------------------------------------

#ifndef THREAD_H
#define THREAD_H

#include "ThreadClass.h"

//-----------------------------------------------------------------------
//definition for boat thread
//-----------------------------------------------------------------------
class BoatThread : public Thread {
public:
	BoatThread(int numCross);
private:
	int numCross;
	void ThreadFunc();
};

//-----------------------------------------------------------------------
//definition for cannibal thread
//-----------------------------------------------------------------------
class CannibalThread : public Thread {
public:
	CannibalThread(int id, int num);
private:
	int id;
	int num;
	void ThreadFunc();
};

//-----------------------------------------------------------------------
//definition for missionary thread
//-----------------------------------------------------------------------
class MissionaryThread : public Thread {
public:
	MissionaryThread(int id, int num);
private:
	int id;
	int num;
	void ThreadFunc();
};

#endif
