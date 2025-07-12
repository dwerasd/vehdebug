
#include "stdafx.h"

#include "init.h"
#include "DebugHandler.h"
#include "threadpoll.h"


//unsigned char ConfigName[ 256/*# range 0..255*/ ];
THandle fm;
PVEHDebugSharedMem VEHSharedMem;
init__0 func_AddVectoredExceptionHandler;
init__1 func_RemoveVectoredExceptionHandler;
init__2 func_CreateToolhelp32Snapshot;
init__3 func_Thread32First;
init__4 func_Thread32Next;
void* oldExceptionHandler = NULL;



void EmulateInitializeEvents()
{
	TEXCEPTIONPOINTERS ep;
	TEXCEPTIONRECORD er;
	THandle ths;
	TThreadEntry32 lpte;
	bool check = false;
	DWORD cpid = 0;
	bool isfirst = false;

	//OutputDebugString('EmulateInitializeEvents');
	cpid = GetCurrentProcessId();
	ths = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (ths != INVALID_HANDLE_VALUE)
	{
		memset(&lpte, 0, sizeof(lpte));
		lpte.dwSize = sizeof(lpte);
		isfirst = true;
		check = Thread32First(ths, &lpte);
		while (check)
		{
			if (lpte.th32OwnerProcessID == cpid)
			{
				ep.ContextRecord = NULL; //@c;
				ep.ExceptionRecord = &er;
				er.NumberParameters = 0;
				if (isfirst)
				{
					//create process
					er.ExceptionCode = 0xce000000; // $ce000000=create process (just made up)
					InternalHandler(&ep, lpte.th32ThreadID); //I don't care what the return value is
					isfirst = false;
				}
				else
				{
					//create thread
					er.ExceptionCode = 0xce000001;
					InternalHandler(&ep, lpte.th32ThreadID);
				}
			}
			check = Thread32Next(ths, &lpte);
		}
		CloseHandle(ths);
	}

	//if ( VEHSharedMem->ThreadWatchMethod == 0 )
	//{
	//  ThreadPoller = (TThreadPoller*)AfxBeginThread( RUNTIME_CLASS( TThreadPoller ));	
	//}
}

extern "C" void PASCAL EXPORT UnloadVEH()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	if ((RemoveVectoredExceptionHandler != NULL))
	{
		if (oldExceptionHandler != NULL)
		{
			RemoveVectoredExceptionHandler(oldExceptionHandler);
			oldExceptionHandler = NULL;
		}
	}
}


extern "C" BOOL __declspec(dllexport) InitializeVEH(DWORD guid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString mapName;
	/*
	  k = LoadLibrary( _T("kernel32.dll" ));
	  func_AddVectoredExceptionHandler = GetProcAddress((HMODULE)k, "AddVectoredExceptionHandler" );
	  func_RemoveVectoredExceptionHandler = GetProcAddress( k, "RemoveVectoredExceptionHandler" );
	  func_CreateToolhelp32Snapshot = GetProcAddress( k, "CreateToolhelp32Snapshot" );
	  func_Thread32First = GetProcAddress( k, "Thread32First" );
	  func_Thread32Next = GetProcAddress( k, "Thread32Next" );
	*/
	UnloadVEH();
	testandfixcs_start();
	OutputDebugString(_T("VEHDebug init"));

	if (ThreadPoller != NULL)
	{
		ThreadPoller->SuspendThread();
		::TerminateThread(ThreadPoller->m_hThread, 0);

		delete ThreadPoller;
		ThreadPoller = NULL;
	}

	testandfixcs_final();


	//get the shared memory object
	mapName.Format(_T("%d"), guid);
	//  memset( ConfigName, 0, sizeof( ConfigName ));
	//  sprintf( (char*)ConfigName, "%d", guid );

	//  m = (Char*)&ConfigName[0];
	//  {
	//    CStringA init__5 = "ConfigName=";
	//    init__5 += m;

	//    OutputDebugString( init__5 );
	//  }

	//  fm:=CreateFileMapping(INVALID_HANDLE_VALUE,nil,PAGE_READWRITE,0,sizeof(TVEHDebugSharedMem),@ConfigName[0]);
	fm = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, mapName);
	//  OutputDebugString( ( pchar ) ( "fm=" + inttohex( fm, 8 ) ) );
	if (fm == 0)
	{
		//    OutputDebugString( ( pchar ) ( "GetLastError=" + inttostr( getlasterror ) ) );
		return false;
	}
	VEHSharedMem = (PVEHDebugSharedMem)MapViewOfFile(fm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	//  OutputDebugString( ( pchar ) ( "VEHSharedMem=" + inttohex( ptruint( VEHSharedMem ), 8 ) ) );
	if (VEHSharedMem == NULL)
	{
		//    OutputDebugString( ( pchar ) ( "GetLastError=" + inttostr( getlasterror ) ) );
		return false;
	}
	/*
	  OutputDebugString( ( pchar ) ( "HasDebugEvent=" + inttohex( VEHSharedMem.HasDebugEvent, 8 ) ) );
	  OutputDebugString( ( pchar ) ( "HasHandledDebugEvent=" + inttohex( VEHSharedMem.HasHandledDebugEvent, 8 ) ) );
	  OutputDebugString( ( pchar ) ( "@HasDebugEvent=" + inttohex( ptruint( &VEHSharedMem.HasDebugEvent ), 8 ) ) );
	  OutputDebugString( ( pchar ) ( "@HasHandledDebugEvent=" + inttohex( ptruint( &VEHSharedMem.HasHandledDebugEvent ), 8 ) ) );
	*/
	//  if (( AddVectoredExceptionHandler != NULL ) )
	//  {

		//if ( oldExceptionHandler != NULL )
		//OutputDebugString( "Old exception handler should have been deleted. If not, this will crash" );
		//OutputDebugString( "Testing if it handles normal debug events" );
		//OutputDebugString( '1' );
		//OutputDebugString( '2' );
		//OutputDebugString( '3' );
		//OutputDebugString( "Calling EmulateInitializeEvents" );
	EmulateInitializeEvents();
	//OutputDebugString( "returned from EmulateInitializeEvents" );
	//OutputDebugString( "Registering exception handler" );

	oldExceptionHandler = AddVectoredExceptionHandler(1, Handler);
	//	_asm {cli};

		//if ( oldExceptionHandler != NULL )
		//  OutputDebugString( ( pchar ) ( "Created exception handler:" + inttohex( ptruint( oldExceptionHandler ), 8 ) ) );
		//else
		//  OutputDebugString( "Failed creating exception handler" );

	//  }

	return true;
}
