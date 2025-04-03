#pragma once
#include "Protocol.hpp"

class TcpIp
{
private:
    IProtocol& m_TcpIpConf;
    std::string m_ReceivedMessage;
    bool m_TriggerUsb;
public:
    TcpIp(IProtocol& tcpIpConf)
    : m_TcpIpConf(tcpIpConf),
      m_ReceivedMessage(""),
      m_TriggerUsb(false)
    { }
    TcpIp(IProtocol&& tcpIpConf)
    : m_TcpIpConf(tcpIpConf),
      m_ReceivedMessage(""),
      m_TriggerUsb(false)
    { }
    ~TcpIp()
    { }
    
    void Send();
    void Read();

    bool IsUsbTriggerMsg();

    IProtocol& GetTcpIpConfiguration()
    {
        return m_TcpIpConf;
    }
    const std::string& GetReceivedMsg() const
    {
        return m_ReceivedMessage;
    }

    const bool GetTriggerUsb() const
    {
        return m_TriggerUsb;
    }

    void TcpIpSenderThread();
    void TcpIpReaderThread(int connectionId);
};
