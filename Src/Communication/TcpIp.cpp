#include <iostream>
#include <algorithm>
#include "TcpIp.hpp"
#include "Common.hpp"

void TcpIp::Send()
{

}

void TcpIp::Read()
{
    m_TcpIpConf.Read();
    const int timeDiff = m_TcpIpConf.GetElapsedTimeBetweenRead();
    if (timeDiff < 1)
    {
        m_ReceivedMessage += m_TcpIpConf.GetReceptionBuffer();
        m_TriggerUsb = IsUsbTriggerMsg();
    }
    else
    {
        m_ReceivedMessage = "";
        ClearQueue(g_TcpIpAndUsbMsgs);
        m_ReceivedMessage = m_TcpIpConf.GetReceptionBuffer();
        m_TriggerUsb = IsUsbTriggerMsg();
    }
}

bool TcpIp::IsUsbTriggerMsg()
{
    std::vector<int> positions;
    int pos = m_ReceivedMessage.find("A");
    while (pos != std::string::npos)
    {
        positions.push_back(pos);
        pos = m_ReceivedMessage.find("A", pos + 1);
    }
    if (positions.size() >= 2)
    {
        // we got at least one pair
        for (auto it = positions.begin(); it != positions.end() - 1; it++)
        {
            g_TcpIpAndUsbMsgs.push(m_ReceivedMessage.substr(*it, *(it + 1) - *it + 1));
        }

        m_ReceivedMessage = m_ReceivedMessage.substr(*(positions.end() - 1),
                                                     m_ReceivedMessage.size() - *(positions.end() - 1));

        return true;
    }
    return false;
}

void TcpIp::TcpIpSenderThread()
{
}

void TcpIp::TcpIpReaderThread(int connectionId)
{
    for(;;)
    {
        Read();

        if (!m_TcpIpConf.IsConnected())
        {
            serverAndTcpIpNotification[connectionId].serverAndTcpIpCv.notify_one();
            break;
        }
        else
        {
            std::cout << m_TcpIpConf.GetReceptionBuffer() << std::endl;
        }
    }
}
