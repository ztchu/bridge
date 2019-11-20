#include "namedpipe_client.h"

#include "logger.h"
#include "tools.h"


namespace support_ipc {

NamedPipe::NamedPipe(const std::wstring& pipe_name) {
    InitNamedPipe(pipe_name);
}

int NamedPipe::WriteData(const std::string& data_to_write) const {
    DWORD data_len;
    TCHAR buff[4096] = {0};
    swprintf_s(buff, 4096, _TO_UNICODE(data_to_write).c_str());
    if (!WriteFile(namedpipe_handle_, buff, 4096*sizeof(TCHAR), &data_len, NULL))  
    {  
        LOG_WARN << "Send data failed." << ENDLINE;
        return -1;  
    }
    return data_len;
}

bool NamedPipe::IsAvailable() const {
    return INVALID_HANDLE_VALUE != namedpipe_handle_;
}

void NamedPipe::InitNamedPipe(const std::wstring& pipe_name) {
    if (!WaitNamedPipe(pipe_name.c_str(), NMPWAIT_USE_DEFAULT_WAIT )) {
        LOG_INFO << "There is no named pipe instance." << ENDLINE;
        return;
    }

    namedpipe_handle_ = CreateFile(pipe_name.c_str(), GENERIC_READ | GENERIC_WRITE,  
       0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (INVALID_HANDLE_VALUE == namedpipe_handle_) {
        LOG_INFO << "Open named pipe failed: " << GetLastError() << ENDLINE;
        return;
    }
}

NamedPipe::~NamedPipe() {
    CloseHandle(namedpipe_handle_);
}

}