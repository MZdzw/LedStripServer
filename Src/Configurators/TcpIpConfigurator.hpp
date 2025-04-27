#pragma once
#include "Protocol.hpp"
#include "SocketInterface.hpp"

class TcpIpConfigurator : public IProtocol
{
private:
    SocketInterfaceT m_TcpIpData;
    int m_ElapsedTimeDiffReadSeconds;
public:
    TcpIpConfigurator(SocketInterfaceT&& tcpIpData)
    : IProtocol(128), m_TcpIpData(tcpIpData)
    { }
    ~TcpIpConfigurator()
    { }

    void Send() override;
    void Read() override;
    int GetElapsedTimeBetweenRead() const override;
    bool IsConnected() const override;
    void ChangeMode(ModeE mode) override;

    SocketInterfaceT& GetSocketInterface()
    {
        return m_TcpIpData;
    }
};