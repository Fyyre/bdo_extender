#pragma once


template<typename _Char>
std::vector<std::basic_string<_Char>> SplitString(const std::basic_string<_Char>& s, const std::basic_string<_Char>& delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::basic_string<_Char> token;
    std::vector<std::basic_string<_Char>> result;
    while ((pos_end = s.find(delimiter, pos_start)) != std::basic_string<_Char>::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (!token.empty())
            result.push_back(token);
    }
    token = s.substr(pos_start);
    if (!token.empty())
        result.push_back(token);
    return result;
}

template <typename _Char>
std::vector<std::basic_string<_Char>> SplitStringLower(const std::basic_string<_Char>& s,
                                                  const std::basic_string<_Char>& delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::basic_string<_Char> token;

    std::basic_string<_Char> lower(s);
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    std::vector<std::basic_string<_Char>> result;
    while ((pos_end = s.find(delimiter, pos_start)) != std::basic_string<_Char>::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (!token.empty())
            result.push_back(token);
    }
    token = s.substr(pos_start);
    if (!token.empty())
        result.push_back(token);
    return result;
}

std::string FromWString(const wchar_t* str);
std::wstring ToWString(const char* str);
int GetStringIndexFromArray(const wchar_t* string, const wchar_t** array, int array_size);



PIMAGE_NT_HEADERS
RtlImageNtHeader(
    IN PVOID Base
);