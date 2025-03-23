#include <iostream>
#include <cstring>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "TcpIpConfigurator.hpp"

void TcpIpConfigurator::Send()
{
    std::cout << "Sending the data...\n";
    write(m_TcpIpData.clientSocketFd,
          GetSendBuffer().c_str(),
          GetSendBuffer().length());
}

void TcpIpConfigurator::Read()
{
    GetReceptionBuffer().clear();
    memset(GetReceptionBuffer().data(), 0, GetReceptionBuffer().capacity());
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    if (recv(m_TcpIpData.clientSocketFd,
         GetReceptionBuffer().data(),
         GetReceptionBuffer().capacity(), 0) <= 0)
    {
        // The connection is probably closed
        // Inform Server thread about that
        m_TcpIpData.clientConnected = false;
        GetReceptionBuffer() = "Disconnect";
        return;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    GetReceptionBuffer() = std::string(GetReceptionBuffer().data());
}