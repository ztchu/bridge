#include "args_handler.h"

#include "args_parser.h"
#include "constants.h"
#include "logger.h"
#include "tools.h"


#include <chrono>
#include <fstream>
#include <sstream>

namespace support_ipc {

ArgsHandler::ArgsHandler(int argc, char *argv[])
    :named_pipe_(_TO_UNICODE(support_ipc::kPipeName)) {
    if (argc < 1) {
        return;
    }
    if (argv[0][0] != '-') {
        args_ = support_ipc::ArgsParser::ParseArgs(argc - 1, argv + 1);
    } else {
        args_ = support_ipc::ArgsParser::ParseArgs(argc, argv);
    }
    ParseControlCommand();

    // Guarantee that only one instance is started
    singleton_mutex_ = OpenMutex(MUTEX_ALL_ACCESS, FALSE,
        (_TO_UNICODE(support_ipc::kSingletonMutex)).c_str());
    if (singleton_mutex_ == nullptr) {
        singleton_mutex_ = CreateMutex(NULL, TRUE,
            (_TO_UNICODE(support_ipc::kSingletonMutex)).c_str());
        current_role_ = Role::SERVER;
    }
    else {
      current_role_ = Role::CLIENT;
    }
}

ArgsHandler::~ArgsHandler() {
    CloseHandle(singleton_mutex_);
}

Role ArgsHandler::GetCurrentRole() const {
    return current_role_;
}

StreamConfig& ArgsHandler::ParseServerParams() {
    if (args_.Exist(support_ipc::switches::kConfigStr)) {
        configuration_.video_param_ = args_[support_ipc::switches::kConfigStr];
    }
    if (args_.Exist(support_ipc::switches::kConfigFile)) {
        auto json_str = ReadConfig(args_[support_ipc::switches::kConfigFile].c_str());
        if (json_str.empty()) {
            LOG_ERROR << "Read config failed: " << args_[support_ipc::switches::kConfigFile] << ENDLINE;
        }
        else {
            configuration_.video_param_ = json_str;
        }
    }
    if (args_.Exist(support_ipc::switches::kProtocol)) {
        configuration_.protocol_ = std::atoi(args_[support_ipc::switches::kProtocol].c_str());
    }
    if (args_.Exist(support_ipc::switches::kVideoSource)) {
        configuration_.video_source_ = std::atoi(args_[support_ipc::switches::kVideoSource].c_str());
    }

    if (args_.Exist(support_ipc::switches::kLocalIp)) {
        configuration_.local_ip_ = args_[support_ipc::switches::kLocalIp];
    }
    if (args_.Exist(support_ipc::switches::kLocalPort)) {
        configuration_.local_port_ = std::atoi(args_[support_ipc::switches::kLocalPort].c_str());
    }
    if (args_.Exist(support_ipc::switches::kMulticastIp)) {
        configuration_.multicast_ip_ = args_[support_ipc::switches::kMulticastIp];
    }
    if (args_.Exist(support_ipc::switches::kMulticastPort)) {
        configuration_.multicast_port_
            = std::atoi(args_[support_ipc::switches::kMulticastPort].c_str());
    }
    return std::ref(configuration_);
}

ControlCommand ArgsHandler::SendControlMessageToServer() {
    if (current_role_ != Role::CLIENT) {
        LOG_ERROR << "This function can only be called from client." << ENDLINE;
        return ControlCommand::COUNT;
    }

    if (control_command_ == ControlCommand::STOP) {
        support_ipc::NamedPipe named_pipe(_TO_UNICODE(support_ipc::kPipeName));
        LOG_INFO << "Parse kStop option.";
        if (named_pipe_.IsAvailable()) {
            named_pipe_.WriteData(support_ipc::kStopMessage);
            return support_ipc::ControlCommand::STOP;
        }
    }
    return ControlCommand::COUNT;
}

void ArgsHandler::PrintStreamConfig() const {
    LOG_INFO << "stream configuration:" << ENDLINE;
    LOG_INFO << "protocol: " << static_cast<int>(configuration_.protocol_) << ENDLINE;
    LOG_INFO << "video source: " << static_cast<int>(configuration_.video_source_) << ENDLINE;
    LOG_INFO << "local ip: " << configuration_.local_ip_ << ENDLINE;
    LOG_INFO << "local port: " << configuration_.local_port_ << ENDLINE;
    LOG_INFO << "multicast ip: " << configuration_.multicast_ip_ << ENDLINE;
    LOG_INFO << "multicast port: " << configuration_.multicast_port_ << ENDLINE;
    LOG_INFO << "video config: " << configuration_.video_param_ << ENDLINE;
}

void ArgsHandler::ParseControlCommand() {
    if (args_.Exist(support_ipc::switches::kStart)) {
        control_command_ = ControlCommand::START;
    }
    else if (args_.Exist(support_ipc::switches::kStop)) {
        control_command_ = ControlCommand::STOP;
    }
    else {
        control_command_ = ControlCommand::COUNT;
    }
}

std::string ArgsHandler::ReadConfig(const char* filename) {
    std::ifstream paramFile(filename, std::ios::in | std::ios::binary);
    if (paramFile.is_open()) {
        std::stringstream stringBuffer;
        stringBuffer << paramFile.rdbuf();
        return std::string(stringBuffer.str());
    } else {
        return std::string();
    }
}
}