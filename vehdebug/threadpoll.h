#pragma once

/*
Keeps a list of all the threads and notifies the debugger when a change has happened
*/

/*$mode delphi*/


#include "System.h"



class TThreadPoller;



class TThreadPoller : public CWinThread
{
	typedef TThread inherited;
private:
	TList threadlist;
	void GetCurrentList(TList& list);
	void UpdateList();
	void CreateThreadEvent(DWORD ThreadID);
	void DestroyThreadEvent(DWORD ThreadID);
public:
	void execute(void);
};


extern TThreadPoller* ThreadPoller;
