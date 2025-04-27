#pragma once
#include "Protocol.hpp"
#include "TerminalInterface.hpp"

class UsbConfigurator : public IProtocol
{
private:
    TerminalInterfaceT m_UsbData;
    int m_ElapsedTimeDiffReadSeconds;
public:
    UsbConfigurator(TerminalInterfaceT&& usbData);
    ~UsbConfigurator()
    {
        close(m_UsbData.serialPort);
    }

    void Send() override;
    void Read() override;
    int GetElapsedTimeBetweenRead() const override;
    bool IsConnected() const override;
    void ChangeMode(ModeE mode) override;

    TerminalInterfaceT GetTerminalInterface()
    {
        return m_UsbData;
    }
};