#include "../shared/Inc.h"

#include "IntraInc.h"

#include "../BDO/Common.h"

static void* NoopRework() { return nullptr; }
__int64 __fastcall ReturnReworkTrue()
{
	return 1;
}

//uint32_t dword_1463C2180 = 0;
//PAGameServiceTypeManager& TypeManager = *(PAGameServiceTypeManager*)0x01463C2190;
//PAGameServiceTypeManagerTest& singleton_3 = *(PAGameServiceTypeManagerTest*)0x01463C2190;

extern "C"
{
	void IsOnScreenSaver();
};

uint32_t unpack_lz4_rework(uint8_t* pInput, uint8_t* pOutput, uint32_t uiDecompressedLength, uint8_t* pOutput2,
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
			orig_len = hooker::stdcall<int>(0x0140816E40, pInput, pOutput, uiDecompressedLength, pOutput2, pInput_size);
			return orig_len;
		}
	}
	else
	{
		// Original compression
		orig_len = hooker::stdcall<int>(0x0140816E40, pInput, pOutput, uiDecompressedLength, pOutput2, pInput_size);
	}
	return orig_len;
}


//static const wchar_t* AUTHENTIC_DOMAIN = L"connect.yourlameserverhere.local";
DWORD WINAPI GetPrivateProfileStringW_Rework(LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR lpReturnedString, DWORD, LPCWSTR)
{
	//VMStart();
	//StrEncryptWStart();
	wcscpy(lpReturnedString, L"connect.yourlameserverhere.to");
	// connectString.clear();
	//	StrEncryptWEnd();
	//VMEnd();
	return 1;
}

void DisableSecurityModule()
{

	hooker::write_jmp(0x01407E2BC6, 0x01407E2BEA);
	hooker::nop(0x01403C2EA6);                     // 2022.22.06 - verified

	// NorthAmericaGameClientAuthManager::updateSecurityModule - bypass closure of game, via this shit.
	// overwrite first jmp with jmp to mov al, 1 and function return.
	hooker::write_jmp(0x014176452B, 0x01417647A7);

	hooker::write_call(0x01407DF630, &Noop);	// isUseEAC
	hooker::write_call(0x01407DF688, &Noop);	// isUseEACReport

	// skip the failed to init Auth nonsense.
	hooker::write_jmp(0x01403A0358, 0x01403A041E);

	hooker::write_jmp(0x0141764018, 0x014176415E);
	hooker::write_jmp(0x0141764367, 0x0141764413);
	hooker::write_jmp(0x01417641B7, 0x014176434B);

}

//extern "C" void DynamicBSS();

uint32_t& offset_maxZoom1 = *(uint32_t*)0x0144491838;	// 0x44160000 (600)
uint32_t& offset_maxZoom2 = *(uint32_t*)0x0144491858;	// 0x4528c000 (2700)

bool& pa_gIsServerBuild = *(bool*)0x0144489C7C;

bool& isScreenSaverOn = *(bool*)0x1445FEA1B;

uint64_t& ActorProxyFactory__singleton = *(uint64_t*)0x0144F1C668;

void Install_ReworkPatches()
{
	hooker::write_call(0x01407DEAD9, &GetPrivateProfileStringW_Rework);

	hooker::write_call(0x014081725F, &unpack_lz4_rework);
	hooker::write_call(0x01408172D6, &unpack_lz4_rework);


	hooker::write(0x01436921E0, L"\\LameServer Rework\\");
	hooker::write(0x01436253D8, L"LameServer Rework");

	// Anticheat? Find function that calls EnumProcesses. Nop call to that function. This
	// also has some code virtualized so keeping it around is not an option.
	hooker::nop(0x0140A0A550); // ref: "NAK!NAK!" (2022.22.06 verified)

	// hooker::nop(0x0140497250);

	// hooker::write(0x01444357BE, 0);

	// Disable crash uploader.
	hooker::write(0x014368B640, L"127.0.0.1"); // Original "52.x.y.z..".

	// ToClient_isUseDynamicBSS
	// hooker::write_jmp(0x0140AB2C82, 0x0140AB2CB3);

	// isUseDynamicBSS client crash prevent
	hooker::write_jmp(0x0140CCDE30, &ReturnReworkTrue);

	hooker::nop(0x0141AE5981);

	//hooker::write(0x141AEE9B8, 0x32);
	//hooker::write(0x141AEE9B9, 0xC0);

	//offset_maxZoom1 = 1050;
	//offset_maxZoom2 = 1000;

//	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetConnectW", &Noop);
//	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetConnectA", &Noop);

//	hooker::hook_iat<void*>(nullptr, "wininet.dll", "FtpPutFileW", &Noop);
//	hooker::hook_iat<void*>(nullptr, "wininet.dll", "FtpPutFileA", &Noop);

//	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetOpenUrlA", &Noop);
//	hooker::hook_iat<void*>(nullptr, "wininet.dll", "InternetOpenUrlW", &Noop);

	DisableSecurityModule();
}
