#pragma once
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>

inline std::vector<std::string> g_TcpIpAndUsbMsgs;

#define MAX_NUM_OF_CLIENTS 2

struct ServerAndTcpIpNotificationS
{
    std::mutex serverAndTcpIpMutex;
    std::condition_variable serverAndTcpIpCv;
};

extern ServerAndTcpIpNotificationS serverAndTcpIpNotification[MAX_NUM_OF_CLIENTS];

