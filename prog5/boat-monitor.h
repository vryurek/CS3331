// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/07/2018
// PROGRAM ASSIGNMENT #5
// FILE NAME : boat-monitor.h
// PROGRAM PURPOSE :
//		This file contains the class definitions and other
//		definitions needed for boat-monitor.cpp
//
// -----------------------------------------------------------
#ifndef BOAT_MON_H
#define BOAT_MON_H

#include "ThreadClass.h"

//-----------------------------------------------------------------------
//definition for Hoare Monitor
//-----------------------------------------------------------------------
class BoatMonitor : public Monitor {
public:
	BoatMonitor();

	//monitor procedures
	void CannibalArrives(int cid);
	void MissionaryArrives(int mid);
	void BoatReady();
	void BoatDone(int totalCross);

	//helper get methods
	int getNumCrosses();
	bool getValid();
	bool getLoaded();
	char* getPassList();

private:
	//conditional variables
	Condition *misWaiting;
	Condition *canWaiting;
	Condition *getOffBoat;

	//helpers, including counter vars, flags, and arrays to keep track
	//of passengers
	int numMisWait;
	int numCanWait;
	int totalCross;
	int curNumCross;
	int numPass;
	int *passIds;
	int *passTypes;
	bool isValid;
	bool loaded;
	bool canSail;
};

#endif
