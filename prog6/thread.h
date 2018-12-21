// -----------------------------------------------------------
// NAME : Veronica Yurek                     User ID: vryurek
// DUE DATE : 12/14/2018
// PROGRAM ASSIGNMENT #6
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
//definition for row threads
//-----------------------------------------------------------------------
class RowProcessor : public Thread {
public:
	RowProcessor(int id, int aCols);
private:
	int id;
	int aCols;
	int number;
	void ThreadFunc();
};

//-----------------------------------------------------------------------
//definition for column threads
//-----------------------------------------------------------------------
class ColProcessor : public Thread {
public:
	ColProcessor(int id, int bRows);
private:
	int id;
	int bRows;
	int number;
	void ThreadFunc();
};

//-----------------------------------------------------------------------
//definition for inner threads
//-----------------------------------------------------------------------
class InnerProcessor : public Thread {
public:
	InnerProcessor(int rid, int cid, int cRows, int cCols);
private:
	int rid;
	int cid;
	int cCols;
	int cRows;
	int above;
	int left;
	int number;
	void ThreadFunc();
};
#endif
