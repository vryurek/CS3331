// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 11/02/2018
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//		This file contains the class definitions and other
//		definitions needed for thread.cpp and thread-main.cpp.
//	
// -----------------------------------------------------------

#ifndef THREAD_H		//check to be sure global array is only declared once
#define THREAD_H

#include "ThreadClass.h"

extern int B[500][500];		//declare global array

class PrefixSum: public Thread {
  public:
    PrefixSum(int i, int j);		//constructor
  private:
    int i;
    int j;
    void ThreadFunc();
};

#endif
