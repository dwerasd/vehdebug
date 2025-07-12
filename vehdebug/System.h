#pragma once


typedef void* Tfoundcodedialog;
typedef void* Tfrmchangedaddresses;
typedef void* TFrmTracer;
typedef void* TListview;
typedef void* TListBox;
typedef void* TCustomform;


//#define	MAX_MODULE_NAME32		MAX_PATH


#pragma pack(push, 1)
struct TVEHDebugSharedMem
{
	unsigned char CurrentContext[8192/*# range 0..8191*/]; //should be enough for a context...


	//Events created by CE, but duplicated for rights for the target process
	__int64 HasDebugEvent; //set by the dll (qword so the allignment isn't broken when used in 32-bit)
	__int64 HasHandledDebugEvent; //set by the debugger
	__int64 ContinueMethod;
	DWORD ProcessID;
	DWORD ThreadID;
	__int64 ThreadWatchMethod;
	/*# case integer */
	union {
		/*# 1 */
		struct {
			EXCEPTION_RECORD32 Exception32;
		};
		/*# 2 */
		struct {
			EXCEPTION_RECORD64 Exception64;
		};
		//  end;
	}; //union
};
#pragma pack(pop)



typedef TVEHDebugSharedMem* PVEHDebugSharedMem;


enum  TThreadPriority
{
	tpIdle,
	tpLowest,
	tpLower,
	tpNormal,
	tpHigher,
	tpHighest,
	tpTimeCritical
};



typedef TCHAR		Char;
typedef BOOL		WINBOOL;
typedef DWORD		ptrUint;
typedef UINT* uint_ptr;
typedef void** LPLPVOID;
typedef void*& pointeralias;

typedef HANDLE		hModule;
typedef HANDLE		THandle;
typedef GUID		TGUID;

typedef ULONGLONG TLargeInteger;
typedef DWORD_PTR PtrUInt;
typedef byte* pbyte;

typedef OVERLAPPED* POverlapped;
typedef FARPROC TFarProc;
typedef QWORD qword;
typedef LPCTSTR WideString;
typedef WORD& pword;
typedef HKEY tregistry;

enum TJclMMContentType {
	mt8Bytes,
	mt4Words,
	mt2DWords,
	mt1QWord,
	mt2Singles,
	mt1Double
};

#pragma pack(push, 1)
struct TJclMMRegister {
	/*# case TJclMMContentType */
	union {
		/*# mt8Bytes */
		struct {
			unsigned char Bytes[8/*# range 0..7*/];
		};
		/*# mt4Words */
		struct {
			WORD Words[4/*# range 0..3*/];
		};
		/*# mt2DWords */
		struct {
			unsigned int DWords[2/*# range 0..1*/];
		};
		/*# mt1QWord */
		struct {
			__int64 QWords;
		};
		/*# mt2Singles */
		struct {
			float Singles[2/*# range 0..1*/];
		};
		/*# mt1Double */
		struct {
			double Doubles;
		};
	}; //union
};
#pragma pack(pop)


enum TJclFPUContentType {
	ftExtended,
	ftMM
};

#pragma pack(push, 1)
struct TJclFPUData {
	/*# case TJclFPUContentType */
	union {
		/*# ftExtended */
		struct {
			long double FloatValue;
		};
		/*# ftMM */
		struct {
			TJclMMRegister MMRegister;
			WORD Reserved;
		};
	}; //union
};
#pragma pack(pop)


#pragma pack(push, 1)
struct TJclFPURegister {
	TJclFPUData Data;
	unsigned char Reserved[6/*# range 0..5*/];
};
#pragma pack(pop)


typedef TJclFPURegister TJclFPURegisters[8/*# range 0..7*/];
enum TJclXMMContentType {
	xt16Bytes,
	xt8Words,
	xt4DWords,
	xt2QWords,
	xt4Singles,
	xt2Doubles
};

#pragma pack(push, 1)
struct TJclXMMRegister {
	/*# case TJclXMMContentType */
	union {
		/*# xt16Bytes */
		struct {
			unsigned char Bytes[16/*# range 0..15*/];
		};
		/*# xt8Words */
		struct {
			WORD Words[8/*# range 0..7*/];
		};
		/*# xt4DWords */
		struct {
			unsigned int DWords[4/*# range 0..3*/];
		};
		/*# xt2QWords */
		struct {
			__int64 QWords[2/*# range 0..1*/];
		};
		/*# xt4Singles */
		struct {
			float Singles[4/*# range 0..3*/];
		};
		/*# xt2Doubles */
		struct {
			double Doubles[2/*# range 0..1*/];
		};
	}; //union
};
#pragma pack(pop)


enum TJclProcessorSize {
	ps32Bits,
	ps64Bits
};

#pragma pack(push, 1)
struct TJclXMMRegisters {
	/*# case TJclProcessorSize */
	union {
		/*# ps32Bits */
		struct {
			TJclXMMRegister LegacyXMM[8/*# range 0..7*/];
			unsigned char LegacyReserved[128/*# range 0..127*/];
		};
		/*# ps64Bits */
		struct {
			TJclXMMRegister LongXMM[16/*# range 0..15*/];
		};
	}; //union
};
#pragma pack(pop)


#pragma pack(push, 1)    //fxsave
//extended registers
struct TextendedRegisters {
	WORD FCW;                           // bytes from 0   to 1
	WORD FSW;                           // bytes from 2   to 3
	unsigned char FTW;                           // byte 4
	unsigned char Reserved1;                     // byte 5
	WORD FOP;                           // bytes from 6   to 7
	unsigned int FpuIp;                     // bytes from 8   to 11
	WORD CS;                            // bytes from 12  to 13
	WORD Reserved2;                     // bytes from 14  to 15
	unsigned int FpuDp;                     // bytes from 16  to 19
	WORD DS;                            // bytes from 20  to 21
	WORD Reserved3;                     // bytes from 22  to 23
	unsigned int MXCSR;                     // bytes from 24  to 27
	unsigned int MXCSRMask;                 // bytes from 28  to 31
	TJclFPURegisters FPURegisters;      // bytes from 32  to 159
	TJclXMMRegisters XMMRegisters;      // bytes from 160 to 415
	unsigned char Reserved4[96/*# range 416..511*/]; // bytes from 416 to 511
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TDebuggerstate {
	unsigned __int64 threadid;
	unsigned __int64 EFlags;
	unsigned __int64 Eax;
	unsigned __int64 Ebx;
	unsigned __int64 Ecx;
	unsigned __int64 Edx;
	unsigned __int64 Esi;
	unsigned __int64 Edi;
	unsigned __int64 Ebp;
	unsigned __int64 Esp;
	unsigned __int64 Eip;
	unsigned __int64 r8;
	unsigned __int64 r9;
	unsigned __int64 r10;
	unsigned __int64 r11;
	unsigned __int64 r12;
	unsigned __int64 r13;
	unsigned __int64 r14;
	unsigned __int64 r15;
	unsigned __int64 CS;
	unsigned __int64 DS;
	unsigned __int64 es;
	unsigned __int64 fs;
	unsigned __int64 gs;
	unsigned __int64 ss;
	unsigned __int64 Dr0;
	unsigned __int64 Dr1;
	unsigned __int64 Dr2;
	unsigned __int64 Dr3;
	unsigned __int64 Dr6;
	unsigned __int64 Dr7;
	TextendedRegisters fxstate;
	unsigned __int64 LBR_Count;
	unsigned __int64 LBR[16/*# range 0..15*/];
};
#pragma pack(pop)



typedef TDebuggerstate* PDebuggerstate;

enum TBreakType {
	bt_OnInstruction = 0,
	bt_OnWrites = 1,
	bt_OnIOAccess = 2,
	bt_OnReadsAndWrites = 3
};


enum TBreakLength {
	bl_1byte = 0,
	bl_2byte = 1,
	bl_8byte = 2,
	bl_4byte = 3
};


//typedef struct tagTHREADENTRY32 
//{  
//	DWORD dwSize;  
//	DWORD cntUsage;  
//	DWORD th32ThreadID;  
//	DWORD th32OwnerProcessID;  
//	LONG tpBasePri;  
//	LONG tpDeltaPri;  
//	DWORD dwFlags;
//} 
//THREADENTRY32,  *PTHREADENTRY32;
typedef THREADENTRY32	TThreadEntry32;

//typedef struct tagHEAPLIST32 
//{  
//	SIZE_T dwSize;  
//	DWORD th32ProcessID;  
//	ULONG_PTR th32HeapID;  
//	DWORD dwFlags;
//} 
//HEAPLIST32,  *PHEAPLIST32;
//typedef HEAPLIST32	THeapList32;

typedef EXCEPTION_POINTERS TEXCEPTIONPOINTERS;
typedef EXCEPTION_RECORD	TEXCEPTIONRECORD;

typedef CRITICAL_SECTION	TRTLCriticalSection;
typedef CRITICAL_SECTION	TCRITICALSECTION;

typedef DEBUG_EVENT	TDebugEvent;

typedef MEMORY_BASIC_INFORMATION TMemoryBasicInformation;

typedef LPTHREAD_START_ROUTINE TFNThreadStartRoutine;

typedef FLOATING_SAVE_AREA _FLOATING_SAVE_AREA;
typedef FLOATING_SAVE_AREA TFloatingSaveArea;
typedef FLOATING_SAVE_AREA* PFLOATINGSAVEAREA;

typedef CONTEXT TContext;
typedef TContext* PContext;


typedef void* TObject;
typedef CWinThread	TThread;
typedef CString		String;

typedef CArray< DWORD, DWORD > TList;
//pakcj typedef CArray< TCHAR*, TCHAR*> TStrings;


// unsupport form
extern void* frmThreadlist;
extern void* frmDebugEvents;
extern void* frmBreakpointlist;

// unsupport function
BOOL CheckIfConditionIsMetContext(PContext context, String script);
int LUA_functioncall(String routinetocall, void* parameters);

BOOL getmem(void*& pBuffer, int size);
BOOL freemem(void* pBuffer);

Char* strnew(String str);

void* ARRAYOFCONST(void* pBuffer, ptrUint pAddress);

void StrDispose(void*);

int TListIndexOf(TList* list, DWORD dwItem);

BOOL GetFullPath(TCHAR* lptszFile, TCHAR* lptszPath);

