#pragma once

#include <string>
#include <Windows.h>

namespace support_ipc {

class NamedPipe {
public:
    NamedPipe(const std::wstring& pipe_name);

    int WriteData(const std::string& data) const;

    bool IsAvailable() const;
    
    ~NamedPipe();
private:
    void InitNamedPipe(const std::wstring& pipe_name);

private:
    HANDLE namedpipe_handle_ = INVALID_HANDLE_VALUE;
}; // class NamedPipe

}