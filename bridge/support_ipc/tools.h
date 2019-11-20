#pragma once

#include <string>

#ifdef _WIN32
#define _TO_UNICODE(str)  support_ipc::Utf8ToWstring((str))
#define _TO_UTF8(str) support_ipc::Wstring2Utf8((str))
#else
#define _TO_UNICODE(str) (str)
#define _TO_UTF8(str) (str)
#endif // _WIN32


namespace support_ipc {
#ifdef _WIN32
std::string Wstring2Utf8(const std::wstring &unicodeStr);

std::wstring Utf8ToWstring(const std::string &utf8);
#endif

std::string GetNowFromatTime(const std::string &timeFormat);

std::string GetFileDirectory(const std::string &fullPath);
}