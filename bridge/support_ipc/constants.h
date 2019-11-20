#pragma once

#include <string>

namespace support_ipc {
const std::string kProductName = "StreamController";
const std::string kPipeNamePrefix  = "\\\\.\\pipe\\";
const std::string kPipeName = kPipeNamePrefix + kProductName;
const char kStopMessage[] = "stop_stream";
const char kPauseMessage[] = "pause_stream";
const char kResumeMessage[] = "resume_stream";

const std::string kSingletonMutexPostfix = "ToAvoidDuplicateInstance";
const std::string kSingletonMutex = kProductName + "_" + kSingletonMutexPostfix;

namespace switches 
{
    const char kUrl[] = "-url";
    const char kConfigFile[] = "-json_file";
    const char kConfigStr[] = "-json_str";
    const char kStart[] = "-start";
    const char kStop[] = "-stop";
    const char kPause[] = "-pause";
    const char kResume[] = "-resume";
    const char kProtocol[] = "-protocol";
    const char kVideoSource[] = "-video_source";
    const char kLocalIp[] = "-local_ip";
    const char kLocalPort[] = "-local_port";
    const char kMulticastIp[] = "-multicast_ip";
    const char kMulticastPort[] = "-multicast_port";
}


enum class ControlCommand
{
    START,
    STOP,
    PAUSE,
    RESUME,
    COUNT
};

enum ErrorCode 
{
    UNSUPPORT_PROTOCOL = 10,
    UNSUPPORT_VIDEO_SOURCE = 11,
    MULTICAST_IP_EMPTY = 12,
    MULTICAST_PORT_INVALID = 13,
    LOCAL_IP_EMPTY = 14,
    LOCAL_PORT_INVALID = 15,
    INSTANCE_ALREADY_EXIST = 20,
    INSTANCE_IS_STOPPED = 21,
    INSTANCE_IS_PAUSED = 22,
    INSTANCE_IS_RESUMED = 23
};

enum Role
{
    UNKNOWN,
    SERVER,
    CLIENT
};

}

