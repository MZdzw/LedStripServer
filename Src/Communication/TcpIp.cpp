#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <chrono>

#include "TcpIp.hpp"
#include "Common.hpp"

void TcpIp::Send()
{

}

void TcpIp::Read()
{
    m_TcpIpConf.Read();
}

void TcpIp::TcpIpSenderThread()
{
}

void TcpIp::TcpIpReaderThread(int connectionId)
{
    for(;;)
    {
        Read();

        if (m_TcpIpConf.GetReceptionBuffer() == "Disconnect")
        {
            serverAndTcpIpNotification[connectionId].serverAndTcpIpCv.notify_one();
            break;
        }
        else
        {
            std::cout << m_TcpIpConf.GetReceptionBuffer() << std::endl;
        }
    }
}
