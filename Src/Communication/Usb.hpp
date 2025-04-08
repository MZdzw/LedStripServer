#pragma once
#include <string>
#include <vector>
#include "Protocol.hpp"

class Usb
{
    private:
    IProtocol& m_UsbConf;
    std::string m_ReceivedMsg;
public:
    Usb(IProtocol& usbConf)
    : m_UsbConf(usbConf)
    { }
    ~Usb()
    { }

    void Send();
    void Read();

    bool TranslateMsgToUsb(const std::string& tcpIpMsg);

    std::string GetReceivedMsg() const
    {
        return m_ReceivedMsg;
    }

    void UsbSenderThread();
    void UsbReaderThread();


};
