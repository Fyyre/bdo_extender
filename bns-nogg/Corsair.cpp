#include "../shared/Inc.h"

#include "IntraInc.h"

#include "hooker.hpp"

uint32_t unpack_lz4_corsair(uint8_t* pInput, uint8_t* pOutput, uint32_t uiDecompressedLength, uint8_t* pOutput2,
	uint32_t pInput_size)
{
	int orig_len = 0;
	if (*pInput & 0x80) // Last bit of flags byte set
	{
		// Our injected files with custom compression
		int hdr_len = 0;
		int comp_len = 0;
		if (*pInput & 2)
		{
			hdr_len = 9;
			comp_len = *(unsigned*)(pInput + 1);
			orig_len = *(unsigned*)(pInput + 5);
		}
		else
		{
			hdr_len = 3;
			comp_len = *(unsigned char*)(pInput + 1);
			orig_len = *(unsigned char*)(pInput + 2);
		}
		try
		{
			int bytes_decompressed = LZ4_decompress_fast((const char*)pInput + hdr_len, (char*)pOutput, orig_len);
		}
		catch (...)
		{
			orig_len = hooker::stdcall<int>(0x01407F8180, pInput, pOutput, uiDecompressedLength, pOutput2, pInput_size);
			return orig_len;
		}
	}
	else
	{
		// Original compression
		orig_len = hooker::stdcall<int>(0x01407F8180, pInput, pOutput, uiDecompressedLength, pOutput2, pInput_size);
	}
	return orig_len;
}

//static const wchar_t* AUTHENTIC_DOMAIN = L"connect.yourlameserverhere.local";
DWORD WINAPI GetPrivateProfileStringW_Corsair(LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR lpReturnedString, DWORD, LPCWSTR)
{
	wcscpy(lpReturnedString, L"connect.yourlameserverhere.net");
	return 1;
}

__int64 __fastcall ReturnTrue()
{
	return 1;
}

void DisableCorsairSecurityModule()
{
	hooker::write_jmp(0x01407C42E6, 0x01407C430A);

	// NorthAmericaGameClientAuthManager::updateSecurityModule - bypass closure of game, via this shit.
	// overwrite first jmp with jmp to mov al, 1 and function return.
	hooker::write_jmp(0x014118BC6B, 0x014118BEE7);

	hooker::write_call(0x01407C1124, &Noop);
	hooker::write_call(0x01407C117C, &Noop);

	// skip the failed to init Auth nonsense.
	hooker::write_jmp(0x0140387030, 0x01403870F6);

	hooker::write_jmp(0x014118B758, 0x014118B89E);
	// NorthAmericaGameClientAuthManager::initializeSecurityModule Skip
	hooker::write_jmp(0x014118BAA7, 0x014118BB53);

	// search for: 89 78 20 48 8B F9 33 F6  89 75 07
	// then string NorthAmericaGameClientAuthManager::initializeSecurityModule
	// we are looking for the function starting with the above byte sequence that
	// has the string reference to NorthAmericaGameClientAuthManager::initializeSecurityModule
	// or search for 0F 87 8E 01 00 00 48 B9 EC 79 7E 18 18 00 00 00 48 0F A3 D1 0F 83 7A 01 00 00
	// and patch the ja addr to jmp addr
	hooker::write_jmp(0x014118B8F7, 0x014118BA8B);
}

//extern "C" void SetOnScreenSaver();
//extern "C" void ScreenSaverTest();

void Install_CorsairPatches()
{
	hooker::nop(0x0141898980); // call near ptr sub_141552960 ; #STR: "NAK!NAK!"

	hooker::write(0x01435DD0C0, L"\\LameServer Corsair\\");
	hooker::write(0x01435725F8, L"LameServer Corsair");

	hooker::write_jmp(0x0141B592E0, &ReturnTrue);

	hooker::write_jmp(0x0141943420, &Nope);
	
	// hook lz4_decompress for custom PAZ
	hooker::write_call(0x01407F83FF, &unpack_lz4_corsair);
	hooker::write_call(0x01407F8476, &unpack_lz4_corsair);

	// Disable crash uploader.
	hooker::write(0x1435D6878, L"127.0.0.1"); // Original "52.x.y.z".

	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetConnectW", &Noop);
	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetConnectA", &Noop);

	hooker::hook_iat<void*>(nullptr, "wininet.dll", "FtpPutFileW", &Noop);
	hooker::hook_iat<void*>(nullptr, "wininet.dll", "FtpPutFileA", &Noop);

	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetOpenUrlA", &Noop);
	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetOpenUrlW", &Noop);

	DisableCorsairSecurityModule();

	hooker::write_jmp(0x0141B592E0, &ReturnTrue);

	// skip the failed to init Auth nonsense.
	hooker::write_jmp(0x0140387030, 0x01403870F6); // 2022.22.06 - verified

	hooker::write_call(0x01407C07B9, &GetPrivateProfileStringW_Corsair);

	// disable IsUseDynamicBSS
	hooker::write_jmp(0x0141536D7B, 0x141536D9A);



}
