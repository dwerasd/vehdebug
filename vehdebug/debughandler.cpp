
#include "stdafx.h"
#include "DebugHandler.h"
#include "init.h"



CCriticalSection* HandlerCS;
THandle emergency; //event that is set when



void testandfixcs_start()
{
	SetEvent(emergency);
}


void testandfixcs_final()
{
	HandlerCS->Lock();
	HandlerCS->Unlock();

	ResetEvent(emergency);
}


LONG InternalHandler(PEXCEPTION_POINTERS ExceptionInfo, DWORD ThreadID)
{
	LONG result = 0;
	
	
	HandlerCS->Lock(); //block any other thread that has an single step exception untill this is handles

	//fill in the exception and context structures
	VEHSharedMem->Exception32.ExceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	VEHSharedMem->Exception32.ExceptionFlags = ExceptionInfo->ExceptionRecord->ExceptionFlags;
	VEHSharedMem->Exception32.ExceptionRecord = ((DWORD)ExceptionInfo->ExceptionRecord->ExceptionRecord);
	VEHSharedMem->Exception32.NumberParameters = ExceptionInfo->ExceptionRecord->NumberParameters;
	const int stop = ExceptionInfo->ExceptionRecord->NumberParameters;
	for (int i = 0; i <= stop; ++i)
	{
		VEHSharedMem->Exception32.ExceptionInformation[i] = ExceptionInfo->ExceptionRecord->ExceptionInformation[i];
	}
	//setup the context
	int contextsize = 0;
	if (ExceptionInfo->ContextRecord != NULL)
	{
		contextsize = sizeof(TContext);

		// messagebox(0,pchar('Copying context:'+inttohex(ptruint(ExceptionInfo->ContextRecord),8)+':'+inttostr(contextsize)), 'InternalHandler', 0);
		CopyMemory(&VEHSharedMem->CurrentContext[0], ExceptionInfo->ContextRecord, contextsize);
	}
	else
		memset(&VEHSharedMem->CurrentContext[0], 0, sizeof(TContext));

	VEHSharedMem->ProcessID = GetCurrentProcessId();
	VEHSharedMem->ThreadID	= ThreadID;
	if (SetEvent((HANDLE)VEHSharedMem->HasDebugEvent))
	{
		THandle eventhandles[2/*# range 0..1*/];
		eventhandles[0] = (HANDLE)VEHSharedMem->HasHandledDebugEvent;
		eventhandles[1] = emergency;
		const DWORD wr = WaitForMultipleObjects(2, &eventhandles[0], false, INFINITE);
		const int i = wr - WAIT_OBJECT_0;
		if (i == 0) //hashandleddebugevent has been set.  After ce is done with it use the new context
		{
			if (ExceptionInfo->ContextRecord != NULL)
			{
				CopyMemory(ExceptionInfo->ContextRecord, &VEHSharedMem->CurrentContext[0], contextsize);

				//set the debug registers
				((PContext)&VEHSharedMem->CurrentContext[0])->ContextFlags = CONTEXT_DEBUG_REGISTERS;
				SetThreadContext(GetCurrentThread, PContext(&VEHSharedMem->CurrentContext[0]));
			}
		}
		else
		{
			//MessageBox(0,'WaitForMultipleObjects failed', 'VEH Debug Error', MB_OK);
			result = EXCEPTION_CONTINUE_EXECUTION; //something went wrong VEHSharedmem might even be broken
			HandlerCS->Unlock();
			return result;
		}


		//depending on user options either return EXCEPTION_CONTINUE_SEARCH or EXCEPTION_CONTINUE_EXECUTION
		if (VEHSharedMem->ContinueMethod == DBG_CONTINUE)
			result = EXCEPTION_CONTINUE_EXECUTION;
		else
			result = EXCEPTION_CONTINUE_SEARCH;
	}
	//else
	 // MessageBox(0,'SetEvent failed', 'VEH Debug Error', MB_OK);
	HandlerCS->Unlock();
	return result;
}


LONG WINAPI Handler(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	LONG result;
	result = InternalHandler(ExceptionInfo, GetCurrentThreadId());
	return result;
}

void DebugHandler_initialization()
{
	HandlerCS = new CCriticalSection();
	emergency = CreateEvent(NULL, true, false, _T(""));
}

void DebugHandler_finalization()
{
	if (HandlerCS != NULL)
		delete HandlerCS;
}

class DebugHandler_unit
{
public:
	DebugHandler_unit()
	{
		DebugHandler_initialization();
	}
	~DebugHandler_unit() { DebugHandler_finalization(); }
};
DebugHandler_unit _DebugHandler_unit;
