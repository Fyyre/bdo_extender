#pragma once

// Primary shared include, non-PCH.

#include <windows.h>
#include <wininet.h>

#include "../Shared/ntdll/ntdll.h"
#include "../Shared/ntdll/ntstatus.h"
#pragma warning(disable:4091)
#include <dbghelp.h>
#pragma warning (default:4091)

#include <assert.h>

#include <fstream>

#include <string>
#include <vector>
#include <xstring>

#include <stdint.h>

#include <emmintrin.h>
#include <intrin.h>

#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <algorithm>
#include <cctype>

#include <stdint.h>
#include <mutex>

#include <unordered_map>

#include <stdlib.h>
#include <sal.h>

#include <unordered_set>
#include <exception>

#include <sys/types.h>
#include <sys/stat.h>

#include "../Shared/hooker/hooker.hpp"
#include "Ini.h"

#include "../BDO/Common.h"

#include "../shared/lz4/lz4.h"

#ifndef _call
typedef void* (*TUniversalCall)(...);
typedef void (*TUniversalCallV)(...);

#define _call(rT, adr, ...) (rT) TUniversalCall(adr)(__VA_ARGS__);
#define _callv(adr, ...) TUniversalCallV(adr)(__VA_ARGS__);
#endif

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : Warning: " #desc)

#ifndef NORM_STOP_ON_NULL
#define NORM_STOP_ON_NULL 0x10000000
#endif

#define READNATIVEWORD(x) (*(UNALIGNED unsigned short*)x)

#define MM_SHARED_USER_DATA_VA 0x7FFE0000	// hurray for static addresses!
#define USER_SHARED_DATA ((KUSER_SHARED_DATA * const)MM_SHARED_USER_DATA_VA)

#ifndef UserSharedData
#define UserSharedData USER_SHARED_DATA
#endif

#define REPLACE_LEA(addr, func) hooker::write((addr + 3), ((unsigned int)func - (addr + 7)))
#define REPLACE_BYTE(addr, val) hooker::write((addr), ((unsigned char)val))

#define REPLACE_MOV(addr) hooker::write((addr + 2, 0x000000))

static void* Noop() { return nullptr; }
static bool Nope() { return false; }
static bool Uno() { return true; }

#define DEREF(name)*(UINT_PTR *)(name)
#define DEREF_64(name)*(DWORD64 *)(name)
#define DEREF_32(name)*(DWORD *)(name)
#define DEREF_16(name)*(WORD *)(name)
#define DEREF_8(name)*(BYTE *)(name)

#ifndef _T
#if defined(_MSC_VER) && defined(UNICODE)
#define _T(x) L##x
#else
#define _T(x) x
#endif
#endif


