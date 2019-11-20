#pragma once

#include "args_parser.h"
#include "constants.h"
#include "namedpipe_client.h"
#include "namedpipe_server.h"
#include <string>
#include <windows.h>

namespace support_ipc {

struct StreamConfig {
    uint8_t protocol_ = 0;   // default is udp multicast.
    uint8_t video_source_ = 0;   // default is desktop.
    std::string local_ip_;
    uint16_t local_port_ = 0;
    std::string multicast_ip_;
    uint16_t multicast_port_ = 0;

    // json string about video params, like resolution, biterate,  framerate and so on.
    std::string video_param_;
};

class ArgsHandler {
public:
    ArgsHandler(int argc, char *argv[]);
    ~ArgsHandler();

    Role GetCurrentRole() const;
    StreamConfig& ParseServerParams();
    ControlCommand SendControlMessageToServer();
    void PrintStreamConfig() const;

private:
    void ParseControlCommand();
    std::string ReadConfig(const char* filename);

private:
    Args args_;
    Role current_role_ = Role::UNKNOWN;
    StreamConfig configuration_;
    HANDLE singleton_mutex_;
    ControlCommand control_command_;
    support_ipc::NamedPipe named_pipe_;
};

}