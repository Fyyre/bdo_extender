#include "Inc.h"

#include "ini.h"
#include "hooker/hooker.hpp"
#include "utilities.h"


unsigned GetConfigUInt(const char* ini, const char* section, const char* key, unsigned defaultVal)
{
    return GetPrivateProfileIntA(section, key, defaultVal, ini);
}

int GetConfigInt(const char* ini, const char* section, const char* key, int defaultVal)
{
    return hooker::bit_cast<int>(GetConfigUInt(ini, section, key, defaultVal));
}

std::string GetConfigString(const char* ini, const char* section, const char* key, const char* defaultVal)
{
    if (defaultVal == nullptr)
        defaultVal = "";
    std::string buffer;
    buffer.resize(std::max<size_t>(strlen(defaultVal), 128));
    int written = 0;
    for (;;)
    {
        written = GetPrivateProfileStringA(section, key, defaultVal, &buffer.front(), buffer.size() + 1, ini);
        if (written == buffer.size())
            buffer.resize(buffer.size() * 2);
        else
            break;
    }
    buffer.resize(written);
    return buffer;
}

std::wstring GetConfigWString(const char* ini, const char* section, const char* key, const wchar_t* defaultVal)
{
    if (defaultVal == nullptr)
        defaultVal = L"";
    std::wstring result = ToWString(GetConfigString(ini, section, key, "").c_str());
    if (result.empty())
        result = defaultVal;
    return result;
}

bool GetConfigBool(const char* ini, const char* section, const char* key, bool defaultVal)
{
    std::string value = GetConfigString(ini, section, key, "");
    if (value.empty())
        return defaultVal;
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
    return value == "true" || value == "yes" || value == "y" || value == "on" || value == "1";
}
