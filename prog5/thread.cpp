//-----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/07/2018
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//      This program contains the class definitions for the
//      boat, missionary, and cannibal threads.
// -----------------------------------------------------------
#include "boat-monitor.h"
#include "thread.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

static BoatMonitor *mon = new BoatMonitor();  //instance of boat monitor

// -----------------------------------------------------------
// BoatThread Constructor
//     Constructor that initializes variables and prints
//		thread creation message for the boat.
// PARAMETER USAGE :
//		numCross:	number of trips the boat makes
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
BoatThread::BoatThread(int numCross): numCross(numCross) {
  char buffer[100];
  sprintf(buffer, "***** BOAT thread starts\n");
  write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// Boat class body:
// Functions Called:
//    getNumCrosses():  the current number of getNumCrosses
//    getValid(): returns true if there is a valid combo on shore
//    BoatReady(): mon procedure, the boat is ready for boarding
//    getLoaded():  returns true if the boat is loaded
//    getPassList(): returns the list of passengers
//    BoatDone(): boat has crossed, releases passengers
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
void BoatThread::ThreadFunc() {
  Thread::ThreadFunc();
  char buffer[300];

  while((mon->getNumCrosses()) < numCross) {
    Delay();
    if (mon->getValid()) {  //if there is a valid combo on shore
      sprintf(buffer, "***** The boat is ready\n");
      write(1, buffer, strlen(buffer));
      mon->BoatReady();
    }
    Delay();
    Delay();
    if (mon->getLoaded()) { //if boat is full
      sprintf(buffer, "***** Boat load (%d): Passenger List (%s)\n",
                mon->getNumCrosses() + 1, mon->getPassList());
      write(1, buffer, strlen(buffer));

      Delay();  //crossing the river
      Delay();
      //done crossing
      sprintf(buffer, "***** Boat load (%d): Completed\n", mon->getNumCrosses() + 1);
      write(1, buffer, strlen(buffer));
      mon->BoatDone(numCross);  //release passengers for next ride
	    
    }
    Delay();
  }
  Exit();
}

// -----------------------------------------------------------
// CannibalThread Constructor
//     Constructor that initializes variables and prints
//		thread creation message for the cannibals.
// PARAMETER USAGE :
//		id:	number of the cannibal
//    num: the nubmer of crosses allowed
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
CannibalThread::CannibalThread(int id, int num): id(id), num(num) {
  char buffer[100];
  char space = ' ';
  if (id != 0) {
    sprintf(buffer, "%*cCannibal %d starts\n", id, space, id + 1);
  } else {
    sprintf(buffer, "Cannibal %d starts\n", id + 1);
  }
  write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// Cannibal class body:
// Functions Called:
//    getNumCrosses():  the current number of getNumCrosses
//    CannibalArrives(): cannibal arrives at shore to cross
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
void CannibalThread::ThreadFunc() {
  Thread::ThreadFunc();
  char buffer[300];
  char space = ' ';

  while(1) {
    if(mon->getNumCrosses() == num) { //if the last ride, exit thread
      Exit();
    }
    Delay();
    if (id != 0) {
      sprintf(buffer, "%*cCannibal %d arrives\n", id, space, id + 1);
    } else {
      sprintf(buffer, "Cannibal %d arrives\n", id + 1);
    }

    write(1, buffer, strlen(buffer));
    mon->CannibalArrives(id);
    Delay();
  }
}

// -----------------------------------------------------------
// MissionaryThread Constructor
//     Constructor that initializes variables and prints
//		thread creation message for the missionaries.
// PARAMETER USAGE :
//		id:	number of the cannibal
//    num: the nubmer of crosses allowed
// FUNCTION CALLED :
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
MissionaryThread::MissionaryThread(int id, int num): id(id), num(num) {
  char buffer[100];
  char space = ' ';
  if (id != 0) {
    sprintf(buffer, "%*cMissionary %d starts\n", id, space, id + 1);
  } else {
    sprintf(buffer, "Missionary %d starts\n", id + 1);
  }
  write(1, buffer, strlen(buffer));
}

// -----------------------------------------------------------
// Missionary class body:
// Functions Called:
//    getNumCrosses():  the current number of getNumCrosses
//    MissionaryArrives(): missionary arrives at shore to cross
//		sprintf():	prints output to a buffer
//		write():	writes a buffer to stdout
//		strlen():	returns the length of a char array
// -----------------------------------------------------------
void MissionaryThread::ThreadFunc() {
  Thread::ThreadFunc();
  char buffer[300];
  char space = ' ';

  while(1) {
    if(mon->getNumCrosses() == num) { //if the last ride, exit thread
      Exit();
    }
    Delay();
    if (id != 0) {
      sprintf(buffer, "%*cMissionary %d arrives\n", id, space, id + 1);
    } else {
      sprintf(buffer, "Missionary %d arrives\n", id + 1);
    }
    write(1, buffer, strlen(buffer));
    mon->MissionaryArrives(id);
    Delay();
  }
}
