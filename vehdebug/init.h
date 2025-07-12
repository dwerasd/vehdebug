#ifndef initH
#define initH 

/*$mode DELPHI*/


#include "System.h"



typedef unsigned int unsignedint;


//extern unsigned char ConfigName[ 256/*# range 0..255*/ ];
extern THandle fm;
extern PVEHDebugSharedMem VEHSharedMem;

//void InitializeVEH( );
//void UnloadVEH( );

typedef void* (__stdcall* init__0)(unsignedint FirstHandler, PVECTORED_EXCEPTION_HANDLER VectoredHandler);
extern init__0 func_AddVectoredExceptionHandler;
typedef ULONG(__stdcall* init__1)(PVOID VectoredHandlerHandle);
extern init__1 func_RemoveVectoredExceptionHandler;
typedef HANDLE(__stdcall* init__2)(DWORD dwFlags, DWORD th32ProcessID);
extern init__2 func_CreateToolhelp32Snapshot;
typedef BOOL(__stdcall* init__3)(HANDLE hSnapshot, THREADENTRY32& lpte);
extern init__3 func_Thread32First;
typedef BOOL(__stdcall* init__4)(HANDLE hSnapshot, THREADENTRY32& lpte);
extern init__4 func_Thread32Next;
extern void* oldExceptionHandler;

#endif //  initH