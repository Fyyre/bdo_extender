#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <Inc.h>

extern "C"
{
    FARPROC OrignalD3DPERF_BeginEvent;
    FARPROC OrignalD3DPERF_EndEvent;
    FARPROC OrignalD3DPERF_GetStatus;
    FARPROC OrignalD3DPERF_QueryRepeatFrame;
    FARPROC OrignalD3DPERF_SetMarker;
    FARPROC OrignalD3DPERF_SetOptions;
    FARPROC OrignalD3DPERF_SetRegion;
    FARPROC OrignalDebugSetLevel;
    FARPROC OrignalDebugSetMute;
    FARPROC OrignalDirect3D9EnableMaximizedWindowedModeShim;
    FARPROC OrignalDirect3DCreate9;
    FARPROC OrignalDirect3DCreate9Ex;
    FARPROC OrignalDirect3DShaderValidatorCreate9;
    FARPROC OrignalPSGPError;
    FARPROC OrignalPSGPSampleTexture;
}

wchar_t* _strcpy_w(wchar_t* dest, const wchar_t* src)
{
	wchar_t* p;

	if ((dest == 0) || (src == 0))
		return dest;

	if (dest == src)
		return dest;

	p = dest;
	while (*src != 0) {
		*p = *src;
		p++;
		src++;
	}

	*p = 0;
	return dest;
}

wchar_t* _strcat_w(wchar_t* dest, const wchar_t* src)
{
	if ((dest == 0) || (src == 0))
		return dest;

	while (*dest != 0)
		dest++;

	while (*src != 0) {
		*dest = *src;
		dest++;
		src++;
	}

	*dest = 0;
	return dest;
}

HMODULE clone_d3d9()
{
	WCHAR szBuffer[MAX_PATH + 1];

	// let's do this, hFiref0x style <3
	_strcpy_w(szBuffer, TEXT("\\\\?\\globalroot\\systemroot\\sys"));
	_strcat_w(szBuffer, TEXT("tem32"));
	_strcat_w(szBuffer, TEXT("\\d3d9.dll"));

	// Replace with LdrLoadDll... *shrug*
	HMODULE d3d9_handle = LoadLibraryExW(szBuffer, NULL, 0);

	// resolve imports from the original d3d9.dll -- do without calling GetProcAddress
	if (d3d9_handle)
	{
		OrignalD3DPERF_BeginEvent = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_BeginEvent");
		OrignalD3DPERF_EndEvent = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_EndEvent");
		OrignalD3DPERF_GetStatus = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_GetStatus");
		OrignalD3DPERF_QueryRepeatFrame = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_QueryRepeatFrame");
		OrignalD3DPERF_SetMarker = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_SetMarker");
		OrignalD3DPERF_SetOptions = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_SetOptions");
		OrignalD3DPERF_SetRegion = (FARPROC)GetProcAddress(d3d9_handle, "D3DPERF_SetRegion");
		OrignalDebugSetLevel = (FARPROC)GetProcAddress(d3d9_handle, "DebugSetLevel");
		OrignalDebugSetMute = (FARPROC)GetProcAddress(d3d9_handle, "DebugSetMute");
		OrignalDirect3D9EnableMaximizedWindowedModeShim = (FARPROC)GetProcAddress(d3d9_handle, "Direct3D9EnableMaximizedWindowedModeShim");
		OrignalDirect3DCreate9 = (FARPROC)GetProcAddress(d3d9_handle, "Direct3DCreate9");
		OrignalDirect3DCreate9Ex = (FARPROC)GetProcAddress(d3d9_handle, "Direct3DCreate9Ex");
		OrignalDirect3DShaderValidatorCreate9 = (FARPROC)GetProcAddress(d3d9_handle, "Direct3DShaderValidatorCreate9");
		OrignalPSGPError = (FARPROC)GetProcAddress(d3d9_handle, "PSGPError");
		OrignalPSGPSampleTexture = (FARPROC)GetProcAddress(d3d9_handle, "PSGPSampleTexture");
	}
	return d3d9_handle;
}