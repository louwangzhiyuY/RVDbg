#ifndef RVDBG
#define RVDBG
#include <windows.h>
#include "exceptiondispatcher.h"
#include "execthread.h"
#include "..\CHooks\chooks.h"
#include "..\Injector\injector.h"
#include "..\IATResolution\iatresolve.h"


static BOOLEAN UseModule;

static DWORD ExceptionComparator; // If the exception is the address compared
static DWORD ExceptionCode; // The exception status code
static DWORD AccessException;
static DWORD SelectedAddress;
static BYTE SelectedARegister;

static BOOLEAN Debugger;
extern BOOLEAN tDSend;

static BOOLEAN ExceptionMode;
static BOOLEAN ChunkExecutable;

static PVOID Decision; // The code to jump to, what we would call the catch block
static PVOID KiUserRealDispatcher; // the code to the real exception dispatcher
static PVOID KiUser;

static CONDITION_VARIABLE Runnable;
static CRITICAL_SECTION RunLock;

extern CRITICAL_SECTION repr;
extern CONDITION_VARIABLE reprcondition;

static std::vector<PVOID> Swaps;
static HANDLE Threads[16];
static Dispatcher::PoolSect Sector[128];
static Dispatcher::PoolSect CurrentPool;

namespace Dbg
{

	struct VirtualRegisters
	{
		DWORD eax;
		DWORD ebx;
		DWORD ecx;
		DWORD edx;
		DWORD esi;
		DWORD edi;
		DWORD ebp;
		DWORD esp;
		PVOID eip;
		BOOLEAN SSESet;
		BOOLEAN bxmm0;
		BOOLEAN bxmm1;
		BOOLEAN bxmm2;
		BOOLEAN bxmm3;
		BOOLEAN bxmm4;
		BOOLEAN bxmm5;
		BOOLEAN bxmm6;
		BOOLEAN bxmm7;
		double dxmm0;
		double dxmm1;
		double dxmm2;
		double dxmm3;
		double dxmm4;
		double dxmm5;
		double dxmm6;
		double dxmm7;
		float xmm0;
		float xmm1;
		float xmm2;
		float xmm3;
		float xmm4;
		float xmm5;
		float xmm6;
		float xmm7;
		DWORD ReturnAddress;
	};

	enum GPRegisters
	{
		EAX,
		EBX,
		ECX,
		EDX,
		EDI,
		ESI,
		EBP,
		ESP,
		EIP
	};

	enum SSERegisters
	{
		xmm0,
		xmm1,
		xmm2,
		xmm3,
		xmm4,
		xmm5,
		xmm6,
		xmm7,
	};

	static void HandleSSE();
	static PVOID CallChain();
	static void SetKiUser();

	int WaitOptModule(const char* OriginalModuleName, const char* OptModuleName);
	void SetModule(BOOLEAN use);

	void AttachRVDbg();
	void DetachRVDbg();
	void ContinueDebugger();

	void SetRegister(DWORD Register, DWORD value);
	void SetRegisterFP(DWORD Register, BOOLEAN Precision, double Value);
	Dbg::VirtualRegisters GetRegisters();

	void SetExceptionMode(BOOLEAN lExceptionMode);
	BOOLEAN GetExceptionMode();
	DWORD GetExceptionAddress();
	Dispatcher::PoolSect GetPool();

	BOOLEAN IsAEHPresent();

	int AssignThread(HANDLE Thread);
	void RemoveThread(HANDLE Thread);

	Dispatcher::PoolSect* GetSector();
	int GetSectorSize();

}

static Dbg::VirtualRegisters r_registers;

#endif
