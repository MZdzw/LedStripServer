#pragma once
#include <string>
#include <vector>
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
public:
    UsbConfigurator(TerminalInterfaceT&& usbData)
    : IProtocol(128), m_UsbData(usbData)
    { }
    ~UsbConfigurator()
    { }

    void Send() override;
    void Read() override;
    int GetElapsedTimeBetweenRead() const override;
    bool IsConnected() const override;

    void ChangeMode(UsbModeE mode);
};


class Usb
{
    private:
    IProtocol& m_UsbConf;
public:
    Usb(IProtocol& usbConf)
    : m_UsbConf(usbConf)
    { }
    ~Usb()
    { }

    void Send();
    void Read();

    bool TranslateMsgToUsb(const std::string& tcpIpMsg);

    void UsbSenderThread();
    void UsbReaderThread();


};
