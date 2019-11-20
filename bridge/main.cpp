#include "args_handler.h"
#include "constants.h"
#include "logger.h"

#include <chrono>
#include <thread>

int main(int argc, char **argv) {
    support_ipc::ArgsHandler args_handler(argc, argv);
    if (args_handler.GetCurrentRole() == support_ipc::Role::CLIENT) {
        auto ret = args_handler.SendControlMessageToServer();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        LOG_INFO << "Command sent to server: " << static_cast<int>(ret) << ENDLINE;
        return static_cast<int>(ret);
    }
    else if (args_handler.GetCurrentRole() == support_ipc::Role::SERVER) {
        auto stream_config = args_handler.ParseServerParams();
        args_handler.PrintStreamConfig();

        auto command = support_ipc::ControlCommand::COUNT;
        while (command != support_ipc::ControlCommand::STOP) {
            command = support_ipc::WaitForCommand();
            LOG_INFO << "Command received: " << static_cast<int>(command) << ENDLINE;
        }
    }
    else {
        LOG_ERROR << "Unknown role: " << args_handler.GetCurrentRole() << ENDLINE;
    }
}