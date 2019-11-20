#include "tools.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <Windows.h>

namespace support_ipc {
#ifdef _WIN32
std::string Wstring2Utf8(const std::wstring &unicodeStr)
{
    if (unicodeStr.empty())
        return "";

    auto dstLength = WideCharToMultiByte(CP_UTF8, 0, unicodeStr.c_str(), -1, static_cast<char *>(nullptr), 0, NULL, NULL);
    auto destData = new char[dstLength + 1];
    memset(destData, 0, dstLength);
    WideCharToMultiByte(CP_UTF8, 0, unicodeStr.c_str(), -1, destData, dstLength, NULL, NULL);
    std::string result = destData;
    delete[] destData;
    return result;
}
std::wstring Utf8ToWstring(const std::string &utf8)
{
    if (utf8.empty())
        return L"";

    auto dstLength = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    auto destData = new wchar_t[dstLength + 1];
    memset(destData, 0, dstLength);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, destData, dstLength);
    std::wstring result = destData;
    delete[] destData;
    return result;
}
#endif

std::string GetNowFromatTime(const std::string &timeFormat)
{
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm ttime;
    auto reslut = localtime_s(&ttime, &t);
    if (reslut != 0)
    {
        return "";
    }
    auto strTime = std::put_time(&ttime, timeFormat.c_str());
    std::stringstream ss;
    ss << strTime;
    std::string str;
    ss >> str;
    return str;
}

std::string GetFileDirectory(const std::string &fullPath)
{
    auto namePos = fullPath.rfind("\\");
    if (namePos != std::string::npos)
    {
        return fullPath.substr(0, namePos);
    }
    return "";
}
}