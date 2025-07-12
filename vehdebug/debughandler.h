#ifndef DebugHandlerH
#define DebugHandlerH

/*$mode delphi*/


#include "System.h"


LONG WINAPI Handler(struct _EXCEPTION_POINTERS* ExceptionInfo);
LONG InternalHandler(PEXCEPTION_POINTERS ExceptionInfo, DWORD ThreadID);
void testandfixcs_start();
void testandfixcs_final();

#endif //  DebugHandlerH