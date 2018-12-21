// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/07/2018
// PROGRAM ASSIGNMENT #5
// FILE NAME : boat-monitor.cpp
// PROGRAM PURPOSE :
//
//
// -----------------------------------------------------------
#include "boat-monitor.h"
#include "thread.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

// -----------------------------------------------------------
// BoatMonitorConstructor
//     Constructor that initializes variables for the monitor.
// -----------------------------------------------------------
BoatMonitor::BoatMonitor() : Monitor("boat-monitor", HOARE) {
    misWaiting = new Condition("mw"); //for missionaries waiting to cross
    canWaiting = new Condition("cw"); //for cannibals waiting to cross
    getOffBoat = new Condition("getoff"); //for passengers waiting to get off

	  srand(time(NULL)); //seed rand to randomly choose a safe passenger combo
    numMisWait = 0; //number of waiting missionaries
    numCanWait = 0; //number of waiting cannibals
    curNumCross = 0;  //the current cross number
    numPass = 0;  //indes for number of passengers
    passIds = new int[3]; //ids of the passengers
    passTypes = new int[3]; //types of the passengers

    int i;
    for (i = 0; i < 3; i++) { //initialize to -1
      passIds[i] = -1;
      passTypes[i] = -1;
    }
    isValid = false;  //flag for valid passenger combo
    loaded = false; //flag for boat loading
    canSail = true; //flag for whether the boat is done crossing for good
  }

  // -----------------------------------------------------------
  // FUNCTION CannibalArrives
  //     Mimics a cannibal arriving to shore, waiting to cross on the boat,
  //    and getting on/off
  // PARAMETER USAGE :
  //		cid:  the cannibal's id num
  // FUNCTION CALLED :
  //		sprintf():	prints output to a buffer
  //		write():	writes a buffer to stdout
  //		strlen():	returns the length of a char array
  // -----------------------------------------------------------
  void BoatMonitor::CannibalArrives(int cid) {
    MonitorBegin();
    if (!canSail) { //if boat is retired, exit monitor
      MonitorEnd();
      return;
    }
    numCanWait++; //increase wait count
    //check if shore has a valid combo
  if (!isValid && (numMisWait >= 3 || numCanWait >= 3 || (numMisWait >= 2 && numCanWait >= 1))) {
		isValid = true;
	}
    canWaiting->Wait(); //wait to board

    if (!canSail) { //if boat is retired, exit monitor
      MonitorEnd();
      return;
    }
    passIds[numPass] = cid + 1; //add to passenger array
    passTypes[numPass] = 0; //0 is cannibal
    numCanWait--; //decrease wait count
    //check if shore has a valid combo
    if (isValid && !(numMisWait >= 3 || numCanWait >= 3 || (numMisWait >=2 && numCanWait >= 1))) {
      isValid = false;
    }
    numPass++;  //increase passenger count

    if(numPass == 3) {  //if boat is full
      loaded = true;
    }

    getOffBoat->Wait(); // wait to get off boat
    numPass--;  //decrease passenger count
    passIds[numPass] = -1;
    passTypes[numPass] = -1;

    if(numPass == 0) {  //no longer loaded
      loaded = false;
    }
    MonitorEnd();
  }

  // -----------------------------------------------------------
  // FUNCTION MissionaryArrives
  //     Mimics a missionary arriving to shore, waiting to cross on the boat,
  //    and getting on/off
  // PARAMETER USAGE :
  //		mid:  the missionary's id num
  // FUNCTION CALLED :
  //		sprintf():	prints output to a buffer
  //		write():	writes a buffer to stdout
  //		strlen():	returns the length of a char array
  // -----------------------------------------------------------
  void BoatMonitor::MissionaryArrives(int mid) {
    MonitorBegin();
    if (!canSail) { //if boat is retired, exit monitor
      MonitorEnd();
      return;
    }
    numMisWait++; //increase wait count
    //check for valid shore combo
  if (!isValid && (numMisWait >= 3 || numCanWait >= 3 || (numMisWait >= 2 && numCanWait >= 1))) {
		isValid = true;
	}

    misWaiting->Wait(); //wait to board
    if (!canSail) { //if boat is retired, exit monitor
      MonitorEnd();
      return;
    }
    passIds[numPass] = mid + 1; //add to passenger list
    passTypes[numPass] = 1;
    numMisWait--;
    //check for valid shore combo
    if (isValid && !(numMisWait >= 3 || numCanWait >= 3 || (numMisWait >=2 && numCanWait >= 1))) {
      isValid = false;
    }
    numPass++;  //increase passenger count

    if(numPass == 3) {  //of boat is full
      loaded = true;
    }

    getOffBoat->Wait(); //wait to get off boat
    numPass--;
    passIds[numPass] = -1;
    passTypes[numPass] = -1;

    if(numPass == 0) {
      loaded = false;
    }
    MonitorEnd();
  }

  // -----------------------------------------------------------
  // FUNCTION BoatReady
  //     The boat is read for the next boatload; chooses a safe
  //    passenger load
  // PARAMETER USAGE :
  //
  // FUNCTIONS CALLED :
  //		sprintf():	prints output to a buffer
  //		write():	writes a buffer to stdout
  //		strlen():	returns the length of a char array
  //    Signal(): signals a conditional variable
  //    Wait(): blocks the caller and releases a waiting thread
  // -----------------------------------------------------------
  void BoatMonitor::BoatReady() {
    MonitorBegin();
      char buffer[300];
      int i, k;

    //array to randomize choosing of a safe passenger load
	  int array[] = { 0, 1, 2 };
	  for (i = 0; i < 3; i++) {
		  int r = rand() % 3;
		  int temp = array[i];
		  array[i] = array[r];
		  array[r] = temp;
	  }

    //loop three times to be sure to hit each possibility
	  for (k = 0; k < 3; k++) {
		  if (array[k] == 0) {  //case 1, three missionaries
			  if (numMisWait >= 3) { //check for on-shore validity
				  misWaiting->Signal();
				  misWaiting->Signal();
				  misWaiting->Signal();
				  sprintf(buffer, "MONITOR (%d): Three missionaries are selected (%d, %d, %d)\n",
					  curNumCross + 1, passIds[0], passIds[1], passIds[2]);
				  write(1, buffer, strlen(buffer));
				  break;  //break out of loop
			  }

		  }
		  else if (array[k] == 1) { //case 2, three cannibals
			  if (numCanWait >= 3) { //check for on-shore validity
				  canWaiting->Signal();
				  canWaiting->Signal();
				  canWaiting->Signal();
				  sprintf(buffer, "MONITOR (%d): Three cannibals are selected (%d, %d, %d)\n",
					  curNumCross + 1, passIds[0], passIds[1], passIds[2]);
				  write(1, buffer, strlen(buffer));
				  break;
			  }
		  } else {  //case 3, two missionaries one cannibal
			  if (numCanWait >= 1 && numMisWait >= 2) {  //check for on-shore validity
				  misWaiting->Signal();
				  misWaiting->Signal();
				  canWaiting->Signal();
				  int c, m1, m2;
				  if (passTypes[0] == 0) {  //determine which passenger is which type
					  c = 0;
					  m1 = 1;
					  m2 = 2;
				  }
				  else if (passTypes[1] == 0) {
					  c = 1;
					  m1 = 0;
					  m2 = 2;
				  }
				  else {
					  c = 2;
					  m1 = 0;
					  m2 = 1;
				  }

				  sprintf(buffer, "MONITOR (%d): Two missionaries (%d, %d) and one cannibal are selected (%d)\n",
					  curNumCross + 1, passIds[m1], passIds[m2], passIds[c]);
				  write(1, buffer, strlen(buffer));
				  break;
			  }
		  }
	  } //end for loop
    MonitorEnd();
  }

  // -----------------------------------------------------------
  // FUNCTION BoatDone
  //     The boat has crossed; releases the passengers
  // PARAMETER USAGE :
  //      totalCross: total number of crosses allowed
  // FUNCTIONS CALLED :
  //		sprintf():	prints output to a buffer
  //		write():	writes a buffer to stdout
  //		strlen():	returns the length of a char array
  //    Signal(): signals a conditional variable
  //    Wait(): blocks the caller and releases a waiting thread
  // -----------------------------------------------------------
  void BoatMonitor::BoatDone(int totalCross) {
    MonitorBegin();
    char buffer[300];
    getOffBoat->Signal(); //release the passengers
    getOffBoat->Signal();
    getOffBoat->Signal();
    int i;

    curNumCross++;  //increase number of complete crosses

    //if the last crossing
    if (curNumCross == totalCross) {

      canSail = false;
      for (i = 0; i < numCanWait; i++) {  //release all waiting so they can exit
        canWaiting->Signal();
      }
      for (i = 0; i < numMisWait; i++) {
        misWaiting->Signal();
      }
      sprintf(buffer, "MONITOR: %d crosses have been made.\n", curNumCross);
      write(1, buffer, strlen(buffer));
      sprintf(buffer, "MONITOR: This river cross is closed indefinitely for renovation.\n");
      write(1, buffer, strlen(buffer));

    }
    MonitorEnd();
  }

  // -----------------------------------------------------------
  // FUNCTION getNumCrosses
  //     returns the current crossing number
  // -----------------------------------------------------------
  int BoatMonitor::getNumCrosses() {
    return curNumCross;
  }

  // -----------------------------------------------------------
  // FUNCTION getLoaded
  //     returns whether the boat is loaded
  // -----------------------------------------------------------
  bool BoatMonitor::getLoaded() {
    return loaded;
  }

  // -----------------------------------------------------------
  // FUNCTION getValid
  //     returns whether there is a valid passenger combo on shore
  // -----------------------------------------------------------
  bool BoatMonitor::getValid() {
    return isValid;
  }

  // -----------------------------------------------------------
  // FUNCTION getPassList
  //     returns the formatted passenger list
  // -----------------------------------------------------------
  char* BoatMonitor::getPassList() {
    char buffer[100];
    int i;
    char *res = new char[100];
    for (i = 0; i < 2; i++) { //fisrt two passengers
      if (passTypes[i] == 0) {  //if a cannibal
        sprintf(buffer, "c%d, ", passIds[i]);
      } else {
        sprintf(buffer, "m%d, ", passIds[i]);
      }

      strcat(res, buffer);
    }

	if (passTypes[2] == 0) { //last passenger
		sprintf(buffer, "c%d", passIds[i]);
	}
	else {
		sprintf(buffer, "m%d", passIds[i]);
	}

	strcat(res, buffer);

    return res;
  }
