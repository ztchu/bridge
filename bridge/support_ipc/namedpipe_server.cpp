#include "namedpipe_server.h"

#include <thread> 
#include <Windows.h>

#include "constants.h"
#include "logger.h"
#include "tools.h"

namespace support_ipc {

ControlCommand WaitForCommand() {
    HANDLE named_pipe = CreateNamedPipe((_TO_UNICODE(support_ipc::kPipeName)).c_str(),  // Name of pipe
       PIPE_ACCESS_DUPLEX |FILE_FLAG_OVERLAPPED,       // The way to open pipe.
       PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // Type of pipeline handle, read and wait mode
       5,      // Number of instances of pipeline (maximum)
       4096,   // Output cache size of pipeline
       4096,   // Input buffer size of pipeline
       0,      // Set timeout, use default 50ms
       NULL);
    
    if (INVALID_HANDLE_VALUE == named_pipe) {
        CloseHandle(named_pipe); 
        LOG_ERROR << "Create named pipe failed." << ENDLINE;
        return support_ipc::ControlCommand::COUNT;
    }

    HANDLE event;  
    OVERLAPPED ovlpd; 
    event = CreateEvent(NULL, TRUE, FALSE, NULL);  
    if (!event) {
        LOG_ERROR << "Create event failed." << ENDLINE;
        CloseHandle(named_pipe);
        return support_ipc::ControlCommand::COUNT;
    }

    memset(&ovlpd, 0, sizeof(OVERLAPPED));
    ovlpd.hEvent = event;

    if (!ConnectNamedPipe(named_pipe, &ovlpd)) {
        if (ERROR_IO_PENDING != GetLastError()) {
            CloseHandle(named_pipe);  
            CloseHandle(event); 
            LOG_ERROR << "Wait for client to connect failed." << ENDLINE;
            return support_ipc::ControlCommand::COUNT;
        }
    }

    if (WAIT_FAILED == WaitForSingleObject(event, INFINITE)) {
        CloseHandle(named_pipe);  
        CloseHandle(event);
        LOG_ERROR << "Wait for client to connect failed." << ENDLINE;
        return support_ipc::ControlCommand::COUNT;
    }

    std::string result;
    ControlCommand command_ans = support_ipc::ControlCommand::COUNT;
    ReadData(named_pipe, result);
    if (result == kStopMessage) {
        command_ans = support_ipc::ControlCommand::STOP;
    }
    else if (result == kPauseMessage) {
        command_ans = support_ipc::ControlCommand::PAUSE;
    }
    else if (result == kResumeMessage) {
        command_ans = support_ipc::ControlCommand::RESUME;
    }
    else {
        command_ans = support_ipc::ControlCommand::COUNT;
    }

    CloseHandle(event);
    CloseHandle(named_pipe);
    return command_ans;
}

int ReadData(HANDLE namedpipe, std::string& result){
    if (INVALID_HANDLE_VALUE == namedpipe) {
        LOG_WARN << "Open named pipe failed." << ENDLINE;
        return -1;
    }

    DWORD data_read;
    TCHAR data[4096] = { 0 };
    memset(data, 0, sizeof(data));
    if (!ReadFile(namedpipe, data, 4096 * sizeof(TCHAR), &data_read, NULL)) {
        LOG_WARN << "Read data failed." << ENDLINE;
        return -1;
    }
    result = _TO_UTF8(data);
    return data_read;
}

}