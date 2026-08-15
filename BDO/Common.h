#pragma once

#include "../shared/Inc.h"

#include "../BDO/Enum.h"

//#pragma pack(1)


using PA_LOGXXXXX_SVC_FUNC_T = void (*)(const char* developer, bool doNotifyToManager, int category, ELogLevel level,
int symNo, const wchar_t* formattedString, const wchar_t* traceFileLineNo);

class PAGameServiceTypeManager2
{
    /* 0000 */ bool _isLoaded;
    /* 0001 */ bool _isStartXboxLoaded;
    /* 0002 */ bool _isXboxLoaded;
    /* 0003 */ uint8_t Type;
    /* 0004 */ uint8_t unk0004[380];
    /* 0182 */ bool isUseServerBExcel;
    /* 0183 */ bool isConferenceMode;
    /* 0184 */ bool isLoadBinaryStaticStatus;
    /* 0185 */ bool isLoadBinaryBriefActionChart;
    /* 0186 */ bool isUseSecureDev;
    /* 0187 */ bool isLastTrIdLog;
    /* 0188 */ uint8_t unk0186;
    /* 0189 */ bool isShowSystemMessageInKorean;
    /* 018A */ bool isLoadInstanceField;
    /* 018B */ bool isNotUseInstanceTutorial;

    /* 018C */ uint8_t unk018A[258]; // 0x104

    /* 0210 std::string viewTradeMarketUrl; */
    /* 0230 std::string gameTradeMarketUrl; */
    /* 0250 std::string AuthUrl; */
    /* 0270 std::string BillingUrl;  */

    /* 0290 */ bool isConsoleDevServer;
    /* 0291*/ bool isLoadDynamicBSS;
    /* 0292*/ bool isUseSimplePlayerAction;
    /* 0293*/ bool isShadowConsole;
    /* 0294 */ bool isUseNexonPCRoomTest;
    /* 0295 */ bool isNotSnailLauncher;
    /* 0296 */ bool isWeGameLauncher;
    /* 0296 */ bool isUseCrcCheck;

    /* 018A */ uint8_t unk0297[76]; // 0x104

    /* 02E0 */ bool isNotQA;
    /* 02E1 */ uint8_t unk02E1[238];
    /* 03CF */ bool UseGameguard;
    /* 03D0 */ uint8_t pad03D0[141]; //[0x047];
    /* 045D */ bool UseEAC;
    /* 045E */ uint8_t pad0239[301];
    /* 04D8 */ bool isUseEACReport;
    /* 0500 */ bool isUseTimeZone;
};
//constexpr size_t sizeOfT = sizeof(PAGameServiceTypeManager2);

struct PAGameServiceTypeManager
{
    /* 0000 */ bool _isLoaded;
    /* 0001 */ bool _isStartXboxLoaded;
    /* 0002 */ bool _isXboxLoaded;
    /* 0003 */ uint8_t Type;
    /* 0004 */ uint8_t unk0004[380];
    /* 0182 */ bool isUseServerBExcel;
    /* 0183 */ bool isConferenceMode;
    /* 0184 */ bool isLoadBinaryStaticStatus;
    /* 0185 */ bool isLoadBinaryBriefActionChart;
    /* 0186 */ bool isUseSecureDev;
    /* 0187 */ bool isLastTrIdLog;
    /* 0188 */ uint8_t unk0188;
    /* 0189 */ bool isShowSystemMessageInKorean;
    /* 018A */ bool isLoadInstanceField;
    /* 018B */ bool isNotUseInstanceTutorial;

    /* 018C */ uint8_t unk018A[0x102]; // 0x104

    /* 0210 std::string viewTradeMarketUrl; */
    /* 0230 std::string gameTradeMarketUrl; */
    /* 0250 std::string AuthUrl; */
    /* 0270 std::string BillingUrl;  */

    /* 0290 */ bool isConsoleDevServer;
    /* 0291*/ bool isLoadDynamicBSS;
    /* 0292*/ bool isUseSimplePlayerAction;
    /* 0293*/ bool isShadowConsole;
    /* 0294 */ bool isUseNexonPCRoomTest;
    /* 0295 */ bool isNotSnailLauncher;
    /* 0296 */ bool isWeGameLauncher;
    /* 0296 */ bool isUseCrcCheck;

    /* 018A */ uint8_t unk0297[76]; // 0x104

    /* 02E0 */ bool isNotQA;
    /* 02E1 */ uint8_t unk02E1[238];
    /* 03CF */ bool UseGameguard;
    /* 03D0 */ uint8_t pad03D0[141]; //[0x047];
               uint8_t pad160[160];
    /* 045D */ bool UseEAC;
    /* 045E */ uint8_t pad0239[239];
    /* 04D1    bool isUseEACReport */
    /* 0500 */ bool isUseTimeZone;
};

static_assert(sizeof(PAGameServiceTypeManager) == 1518, "");
//constexpr size_t sizeOfT = sizeof(PAGameServiceTypeManager);

// 0000000145DA8AC0
// byte ptr ds:[rax+417]=[blackdesert64.0000000145DA8ED7]=0
// ClientAuthManager::updateSecurityModule ���� FALSE
// isNotQa -> 0000000145DA8DA0;

struct _NEWPAGameServiceTypeManager
{
                bool _isLoaded;
                bool _isStartXboxLoaded;
                bool _isXboxLoaded;
    /* 0003 */  PA_GAME_SERVICE_TYPE _serviceType;
    /* 0004 */  PA_GAME_NATION_TYPE _serviceNation;
                std::string _authenticationDomainOrIp;
                uint16_t _authenticPortNo;
                std::string _serverManagerDomainOrIp;
                uint16_t _serverManagerPortNo;
    /* 0134 */  uint8_t unk0134[0x134];


    /* 0180 */  bool dontUsePublisherAuthentication;
    /* 0181 */  bool dontCreateType;
    /* 0182 */  bool isUseServerBExcel;
    /* 0183 */  bool isConferenceMode;
    /* 0184 */  bool isLoadBinaryStaticStatus;
    /* 0185 */  bool isLoadBinaryBriefActionChart;
    /* 0186 */  bool isUseSecureDev;
    /* 0187 */  bool isLastTrIdLog;
    /* 0188 */  bool unk0188;
    /* 0189 */  bool isShowSystemMessageInKorean;
    /* 018A */ bool isLoadInstanceField;
    /* 018B */ bool isNotUseInstanceTutorial;

    /* 018A */  char padding0157[0x9e];

    /* 0230 */  std::string gameTradeMarketUrl;
    /* 0250 */  std::string AuthUrl;
    /* 0270 */  std::string BillingUrl;

    /* 0290 */ bool isConsoleDevServer;
    /* 0291 */  bool isLoadDynamicBSS;
    /* 0292*/  bool isUseSimplePlayerAction;
    /* 0293*/  bool isShadowConsole;
    /* 0294 */ bool isUseNexonPCRoomTest;
    /* 0295 */ bool isNotSnailLauncher;
    /* 0296 */ bool isWeGameLauncher;
    /* 0296 */ bool isUseCrcCheck;

               char padding0297[0x4A];

    /* 02E0 */ bool isNotQA;

    /* 02E1 */ uint32_t unk02E1;

    /* 02E4 */ uint64_t unk02E4;
    /* 02E8 */

    /* 02EC */ uint64_t unk02EC;
    /* 02F0 */

    /* 02E4 */ uint64_t unk02F4;
    /* 02F8 */

    /* 0301 */ bool NEWUI;
    /* 0302 */ bool CONSOLENEWUI;
    /* 0303 */ bool USETEXUREID;

    /* 03CB */ bool testBotMode;
    /* 03CC */ bool isSkipSecurityModule;    // byte ptr ds:[rax+3CC]=[blackdesert64.0000000145DA8E8C]=0
    /* 03CD */ bool unk03C8;
    /* 03CF */ bool UseGameguard;

               char padding0042[0x42];

    /* 0411 */ uint8_t ShadowServiceType;

    /* 0418 */
               char padding004C[0x45];
    /* 045D */ bool UseEAC;     // Offset relocation from 0x417 to 0x45D with version 398535
               char padding0074[0xB9];
    /* 04D1 */ bool isUseEACReport;
               char padding04D2[0x78];
    /* 0549 */ bool REBOOTCHARACTER;
};

struct __declspec(align(8)) PAGameServiceTypeManagerTest
{
	bool _isLoaded;
	bool _isStartXboxLoaded;
	bool _isXboxLoaded;
	PA_GAME_SERVICE_TYPE _serviceType;
	PA_GAME_NATION_TYPE _serviceNation;
	std::string _authenticDomainOrIp;
	unsigned __int16 _authenticPortNo;
	std::string _serverManagerDomainOrIp;
	unsigned __int16 _serverManagerPortNo;
	std::wstring _dataUrl;
	std::wstring _pathchUrl;
	std::wstring _dumpUrl;
	std::wstring _dumpServerID;
	std::wstring _dumpServerPW;
	std::string _ingameWebUrl;
	std::string _influxUrl;
	int _portNumber;
	bool _dontUsePublisherAuthentication;
	bool _isUseServerBexcel;
	bool _isConferenceMode;
	bool _isLoadBinaryStaticStatus;
	bool _isLoadBinaryBriefActionChart;
	bool _isUseSecureDev;
	bool _isLastTrIdLog;
	bool _isTestField;
	bool _isShowSystemMessageInKorean;
	bool _isLoadInstanceField;
	std::wstring _xboxAuthUrl;
	std::wstring _viewTradeMarketUrl;
	std::wstring _gameTradeMarketUrl;
	bool _isDynamicStaticStatusforDV;
	bool _isConsoleDevServer;
	std::wstring _serviceTypeString;
	bool _isRealServiceMode;
	int _userNicknameLength;
	int _characterNameLength;
	int _guildNameLength;
	int _servantNameLength;
	int _petNameLength;
	bool _isWordBreak;
	std::string _pcRoomUrl;
	std::string _liveIp1[5];
	PA_SERVICE_RESOURCE_TYPE _serviceResourceType;
	PA_SERVICE_RESOURCE_TYPE _serviceResourceType__XXX__FORDEVELOP;
	bool _isTeenManager;
	bool _isOptimizationInitMarketList;
};

//constexpr size_t sizeOfT = sizeof(_NEWPAGameServiceTypeManager);


//constexpr size_t sizeOfT = sizeof(PAGameServiceManager);

// Objects
extern PAGameServiceTypeManager &TypeManager;   // Function that reads service.ini initializes this object.
extern PAGameServiceTypeManagerTest& singleton_3;
// 144229C7C UseDynamicBSS   db 1

//#pragma pack()
