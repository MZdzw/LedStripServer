#pragma once
#include <mutex>
#include <condition_variable>
#include <string>

extern std::mutex g_Mutex;
extern std::condition_variable g_Cv;
extern std::string g_Msg;

extern std::string g_MsgToClose;
extern int g_PortNumber;
extern std::string g_SerialName;

extern bool g_Session;

