#pragma once
#include "Protocol.hpp"
#include "SocketInterface.hpp"

class TcpIpConfigurator : public IProtocol
{
private:
    SocketInterfaceT m_TcpIpData;
public:
    TcpIpConfigurator(SocketInterfaceT&& tcpIpData)
    : IProtocol(128), m_TcpIpData(tcpIpData)
    { }
    ~TcpIpConfigurator()
    { }

    void Send() override;
    void Read() override;

    SocketInterfaceT& GetSocketInterface()
    {
        return m_TcpIpData;
    }
};