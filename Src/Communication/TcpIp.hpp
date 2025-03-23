#pragma once
#include "Protocol.hpp"
// #include "TcpIpConfigurator.hpp"

class TcpIp
{
private:
    IProtocol& m_TcpIpConf;
public:
    TcpIp(IProtocol& tcpIpConf)
    : m_TcpIpConf(tcpIpConf)
    { }
    TcpIp(IProtocol&& tcpIpConf)
    : m_TcpIpConf(tcpIpConf)
    { }
    ~TcpIp()
    { }
    
    void Send();
    void Read();

    IProtocol& GetTcpIpConfiguration()
    {
        return m_TcpIpConf;
    }

    void TcpIpSenderThread();
    void TcpIpReaderThread(int connectionId);
};
