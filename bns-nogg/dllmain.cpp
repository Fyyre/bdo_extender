#include "../shared/Inc.h"

//#include "Rework.h"
#include "Corsair.h"
#include "IntraInc.h"

BYTE& reworkBytesToCheck = *(BYTE*)0x0141E43788;

bool bIsCheck = false;

static FARPROC GetSystemTimeAsFileTimePtr = nullptr;
static decltype(&GetSystemTimeAsFileTime) GetSystemTimeAsFileTimeOriginal = nullptr;

HMODULE clone_d3d9();

VOID
WINAPI
GetSystemTimeAsFileTime_h(
	LPFILETIME lpSystemTimeAsFileTime
)
{
	LARGE_INTEGER SystemTime{};

	do {
		SystemTime.HighPart = UserSharedData->SystemTime.High1Time;
		SystemTime.LowPart = UserSharedData->SystemTime.LowPart;
	} while (SystemTime.HighPart != UserSharedData->SystemTime.High2Time);

	lpSystemTimeAsFileTime->dwLowDateTime = SystemTime.LowPart;
	lpSystemTimeAsFileTime->dwHighDateTime = SystemTime.HighPart;

	if (!bIsCheck)
	{
		if (reworkBytesToCheck == 0x48)
		{
			bIsCheck = true;
			Install_CorsairPatches();
			hooker::unhook(GetSystemTimeAsFileTimePtr, GetSystemTimeAsFileTimeOriginal);
		}
	}
}

BOOL
APIENTRY
DllMain(
	const HINSTANCE instance,
	const DWORD reason,
	const PVOID reserved
)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
	{
		clone_d3d9();

		GetSystemTimeAsFileTimePtr = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetSystemTimeAsFileTime");

		GetSystemTimeAsFileTimeOriginal = hooker::hook<decltype(&GetSystemTimeAsFileTime)>(GetSystemTimeAsFileTimePtr, &GetSystemTimeAsFileTime_h, HOOKER_HOOK_FAT);

		LdrDisableThreadCalloutsForDll(instance);
	}
	break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
