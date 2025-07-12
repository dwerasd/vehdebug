
#include "stdafx.h"

#include "threadpoll.h"



#include "DebugHandler.h"



#include "System.h"


TThreadPoller* ThreadPoller = NULL;



void TThreadPoller::CreateThreadEvent(DWORD ThreadID)
{
	TEXCEPTIONPOINTERS ep;
	TEXCEPTIONRECORD er;
	TContext c;
	ep.ContextRecord = &c;
	ep.ExceptionRecord = &er;
	er.NumberParameters = 0;
	er.ExceptionCode = 0xce000001;
	InternalHandler(&ep, ThreadID);
}


void TThreadPoller::DestroyThreadEvent(DWORD ThreadID)
{
	TEXCEPTIONPOINTERS ep;
	TEXCEPTIONRECORD er;
	TContext c;
	ep.ContextRecord = &c;
	ep.ExceptionRecord = &er;
	er.NumberParameters = 0;
	er.ExceptionCode = 0xce000002; //destroythread
	InternalHandler(&ep, ThreadID);
}


void TThreadPoller::GetCurrentList(TList& list)
{
	THandle ths;
	TThreadEntry32 lpte;
	bool check = false;
	DWORD cpi = 0;
	cpi = GetCurrentProcessId();
	ths = CreateToolhelp32Snapshot(TH32CS_SNAPALL, cpi);
	if (ths != INVALID_HANDLE_VALUE)
	{
		memset(&lpte, 0, sizeof(lpte));
		lpte.dwSize = sizeof(lpte);
		check = Thread32First(ths, &lpte);
		while (check)
		{
			if (lpte.th32OwnerProcessID == cpi)
				list.Add(((DWORD)lpte.th32ThreadID));
			check = Thread32Next(ths, &lpte);
		}
		CloseHandle(ths);
	}
}


void TThreadPoller::UpdateList()
{
	TList newlist;
	int i = 0;
	//  newlist = TList.create;
	GetCurrentList(newlist);

	//now try to find the differences

	//is there a threadid that's not in the current threadlist?
	for (int stop = newlist.GetSize() - 1, i = 0; i <= stop; ++i)
		if (threadlist.GetAt(newlist[i]) == -1) //not found
			CreateThreadEvent(((DWORD)newlist[i]));
	for (int stop = threadlist.GetSize() - 1, i = 0; i <= stop; ++i)
		if (newlist.GetAt(threadlist[i]) == -1) //the new list doesn't contain this threadid
			DestroyThreadEvent(((DWORD)threadlist[i]));

	//free the old list and make the new list the current list
  //  delete threadlist;
	threadlist.RemoveAll();

	for (int i = 0; i < newlist.GetSize(); ++i)
		threadlist.Add(newlist[i]);

}


void TThreadPoller::execute(void)
{
	//  threadlist = TList.create;
	try
	{
		GetCurrentList(threadlist);
		while (true)
		{
			Sleep(500);
			UpdateList();
		}
	}
	catch (...)
	{
		//    OutputDebugString( "TThreadPoller terminated" );
		//    threadlist.free;
		throw;
	}
	/*# finally begin */
  //  OutputDebugString( "TThreadPoller terminated" );
  //  threadlist.free;
	/*# finally end */
}
