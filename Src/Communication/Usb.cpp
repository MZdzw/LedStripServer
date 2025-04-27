#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <array>
#include "Usb.hpp"
#include "Common.hpp"

enum class UsbMessageE
{
    REMOVE_SECTOR, ADD_SECTOR, SET_DIODE_HSV,
    SET_DIODE_RGB, SET_SECTOR_HSV, SET_SECTOR_RGB,
    SET_SECTOR_RAINBOW, SPAWN_DIODE, SET_ANIMATION_SPEED,
    SET_ROLLING_EFFECT, SET_DIMMING_ENTIRE_EFFECT,
    SET_NO_ANIMATION, LED_STRIP_REQ, MAX
};

static std::array<std::pair<UsbMessageE, int>, 13> usbMsgsLengthLookUpTable
{
    {
    {UsbMessageE::REMOVE_SECTOR, 2}, {UsbMessageE::ADD_SECTOR, 6}, {UsbMessageE::SET_DIODE_HSV, 7},
    {UsbMessageE::SET_DIODE_RGB, 6}, {UsbMessageE::SET_SECTOR_HSV, 6}, {UsbMessageE::SET_SECTOR_RGB, 5},
    {UsbMessageE::SET_SECTOR_RAINBOW, 2}, {UsbMessageE::SPAWN_DIODE, 5}, {UsbMessageE::SET_ANIMATION_SPEED, 6},
    {UsbMessageE::SET_ROLLING_EFFECT, 2}, {UsbMessageE::SET_DIMMING_ENTIRE_EFFECT, 2},
    {UsbMessageE::SET_NO_ANIMATION, 2}, {UsbMessageE::LED_STRIP_REQ, 1}
    }
};

static bool closeReaderThread = false;

std::vector<std::string> split(const std::string& s, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t position = 0;
    size_t next = 0;
    next = s.find(delimiter, position);
    while (next != std::string::npos)
    {
        if (s.substr(position, next - position) != "A")
        {
            tokens.push_back(s.substr(position, next - position));
        }

        position = next + 1;
        next = s.find(delimiter, position);
    }

    return tokens;
}

void Usb::Send()
{
    // the messages are in queue. Take first and dequeue it
    std::string tcpIpMsg = g_TcpIpAndUsbMsgs.front();
    g_TcpIpAndUsbMsgs.pop();
    auto [usbMsg, success] = TranslateMsgToUsb(tcpIpMsg);
    if (success == true)
    {
        // Set send buffer
        m_SendMsg = std::string(usbMsg.begin(), usbMsg.end());
        m_UsbConf.GetSendBuffer() = m_SendMsg;
        m_UsbConf.Send();
    }
}

void Usb::Read()
{
    m_ReceivedMsg.clear();
    std::cout << "[USB] Before read\n";
    m_UsbConf.Read();
    const int timeDiff = m_UsbConf.GetElapsedTimeBetweenRead();
    std::cout << "[USB] After Read, ReceivedBytes: " << m_UsbConf.GetReceivedBytes() << std::endl;
    // timeDiff not usefull here?
    // The data in Reception buffer in UsbConfigurator should be binary
    // we need to convert it from binary to ASCII
    if (m_UsbConf.GetReceivedBytes() > 0 && m_UsbConf.GetReceivedBytes() != std::numeric_limits<size_t>::max())
    {
        const char* const rawBytes = m_UsbConf.GetReceptionBuffer().data();
        std::for_each(rawBytes, rawBytes + m_UsbConf.GetReceivedBytes(), [this](const char& byte){
            this->m_ReceivedMsg += std::to_string(byte);
        });
    }
}

std::pair<std::string, bool> Usb::TranslateMsgToUsb(const std::string &tcpIpMsg)
{
    std::string usbString = "";
    std::vector<uint8_t> tokensNum;
    std::vector<std::string> tokens = split(tcpIpMsg, "-");
    int commandId = stoi(*tokens.begin());
    if (commandId >= usbMsgsLengthLookUpTable.size())
    {
        return {usbString, false};
    }

    if (tokens.size() != usbMsgsLengthLookUpTable[commandId].second)
    {
        return {usbString, false};
    }
    // the message is great. We can construct usb message
    for (int i = 0; i < 6; i++)
        tokensNum.push_back(0xAA);
    for (const auto& field : tokens)
        tokensNum.push_back(stoi(field));
    for (int i = 0; i < 6; i++)
        tokensNum.push_back(0xAA);

    usbString = std::string(tokensNum.begin(), tokensNum.end());
    return {usbString, true};
}

void Usb::UsbSenderThread()
{
    std::unique_lock<std::mutex> lock(tcpIpAndUsbNotification.tcpIpAndUsbMutex);
    while(1)
    {
        tcpIpAndUsbNotification.tcpIpAndUsbCv.wait(lock);
        if (tcpIpAndUsbNotification.isDisconnected == true)
        {
            tcpIpAndUsbNotification.isDisconnected = false;
            // Change to non blocking mode
            closeReaderThread = true;
            m_UsbConf.ChangeMode(ModeE::NONBLOCKING);
            break;
        }
        Send();
    }
}

void Usb::UsbReaderThread()
{
    m_UsbConf.ChangeMode(ModeE::BLOCKING);
    while(1)
    {
        Read();
        std::cout << "Received USB: " << GetReceivedMsg() << std::endl;

        // if sender thread closed
        if (closeReaderThread == true)
        {
            tcpIpAndUsbNotification.isDisconnected = false;
            closeReaderThread = false;
            break;
        }
    }
}
