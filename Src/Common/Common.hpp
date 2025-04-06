#pragma once
#include <mutex>
#include <condition_variable>
#include <string>
#include <queue>

inline std::queue<std::string> g_TcpIpAndUsbMsgs;

#define MAX_NUM_OF_CLIENTS 2

struct ServerAndTcpIpNotificationS
{
    std::mutex serverAndTcpIpMutex;
    std::condition_variable serverAndTcpIpCv;
};

extern ServerAndTcpIpNotificationS serverAndTcpIpNotification[MAX_NUM_OF_CLIENTS];

template <typename T>
void ClearQueue(std::queue<T>& queue)
{
    std::queue<T> empty;
    std::swap(queue, empty);
}
