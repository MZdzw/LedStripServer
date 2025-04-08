#pragma once
#include "Protocol.hpp"
#include "TerminalInterface.hpp"

enum class UsbModeE
{
    BLOCKING, NONBLOCKING
};

class UsbConfigurator : public IProtocol
{
private:
    TerminalInterfaceT m_UsbData;
    int m_ElapsedTimeDiffReadSeconds;
public:
    UsbConfigurator(TerminalInterfaceT&& usbData);
    ~UsbConfigurator()
    { }

    void Send() override;
    void Read() override;
    int GetElapsedTimeBetweenRead() const override;
    bool IsConnected() const override;

    TerminalInterfaceT GetTerminalInterface()
    {
        return m_UsbData;
    }
    void ChangeMode(UsbModeE mode);
};