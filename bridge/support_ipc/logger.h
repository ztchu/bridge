#ifndef _LOGGER_H
#define _LOGGER_H

//#define GLOG
//#define DEBUG


#ifdef GLOG
#define LOG_INFO LOG(INFO)
#define LOG_WARN LOG(WARNING)
#define LOG_ERROR LOG(ERROR)
#define LOG_DEBUG DLOG(INFO)
#define ENDLINE ""

#else   // GLOG
#include <iostream>
#define LOG_INFO std::cout
#define LOG_WARN std::cerr
#define LOG_ERROR std::cerr
#define LOG_DEBUG std::cerr
#define ENDLINE std::endl
#endif   // GLOG

#endif   // _LOGGER_H
