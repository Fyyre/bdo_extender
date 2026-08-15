#pragma once

#include <string>

unsigned GetConfigUInt(const char* ini, const char* section, const char* key, unsigned defaultVal = 0);
int GetConfigInt(const char* ini, const char* section, const char* key, int defaultVal = 0);
std::string GetConfigString(const char* ini, const char* section, const char* key, const char* defaultVal = "");
std::wstring GetConfigWString(const char* ini, const char* section, const char* key, const wchar_t* defaultVal = L"");
bool GetConfigBool(const char* ini, const char* section, const char* key, bool defaultVal = false);
