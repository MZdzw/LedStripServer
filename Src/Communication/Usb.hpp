#pragma once
#include <string>
#include <vector>
#include "Protocol.hpp"

class Usb
{
    private:
    IProtocol& m_UsbConf;
    std::string m_ReceivedMsg;
    std::string m_SendMsg;
public:
    Usb(IProtocol& usbConf)
    : m_UsbConf(usbConf)
    { }
    ~Usb()
    { }

    void Send();
    void Read();

    std::pair<std::string, bool> TranslateMsgToUsb(const std::string& tcpIpMsg);

    std::string GetReceivedMsg() const
    {
        return m_ReceivedMsg;
    }
    const std::string& GetSendMsg() const
    {
        return m_SendMsg;
    }

    void UsbSenderThread();
    void UsbReaderThread();


};
