#pragma once

// Primary shared include, non-PCH.

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <wininet.h>

#include "ntdll/ntdll.h"
#include "ntdll/ntstatus.h"
#pragma warning(disable:4091)
#include <dbghelp.h>
#include <shlobj.h>
#include <strsafe.h>
#include <stdlib.h>
#pragma warning (default:4091)

#include <assert.h>
#include <psapi.h>
#include <TlHelp32.h>

#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <sal.h>



// reference additional headers your program requires here

#ifndef _NTDEF_
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
#endif

#define NtCurrentThread() ( (HANDLE)(LONG_PTR) -2 )
#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 

#define OffsetToPointer(Base, Offset) ((PVOID)(((PBYTE)(Base)) + ((UINT_PTR)(Offset))))
#define PointerToOffset(Base, Pointer) ((INT_PTR)(((PBYTE)(Pointer)) - ((PBYTE)(Base))))

#define RTL_NEW( p ) RtlAllocateHeap( RtlProcessHeap(), HEAP_ZERO_MEMORY, sizeof( *p ) )
#define RTL_FREE(p) \
	if(p!=NULL)\
	{\
	RtlFreeHeap( RtlProcessHeap(), 0, p);\
	p = NULL;\
	}\
	1