#pragma once

#include <string>
#include <Windows.h>

#include "constants.h"

namespace support_ipc {

ControlCommand WaitForCommand();

int ReadData(HANDLE namedpipe, std::string& result) ;

}