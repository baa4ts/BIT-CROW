#ifndef CONVERSORES_HPP
#define CONVERSORES_HPP

#include <string>
#include <codecvt>
#include <locale>
#include <windows.h>

inline std::string ConvertirWcharAString(const WCHAR *wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

inline std::wstring ConvertirStringAWchar(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

#endif
