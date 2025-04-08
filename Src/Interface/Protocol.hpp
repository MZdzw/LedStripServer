#pragma once
#include <string>

class IProtocol
{
protected:
    std::string m_SendBuffer;
    std::string m_ReceptionBuffer;
    size_t m_ReceivedBytes;
    size_t m_SendBytes;

public:
    IProtocol(size_t size)
    : m_SendBuffer(std::string(size, '\0')),
      m_ReceptionBuffer(std::string(size, '\0')),
      m_ReceivedBytes(0),
      m_SendBytes(0)
    { }
    virtual ~IProtocol() { }

    virtual void Send() = 0;
    virtual void Read() = 0;
    virtual int GetElapsedTimeBetweenRead() const = 0;
    virtual bool IsConnected() const = 0;

    std::string& GetSendBuffer()
    {
        return m_SendBuffer;
    }

    std::string& GetReceptionBuffer()
    {
        return m_ReceptionBuffer;
    }

    size_t& GetReceivedBytes()
    {
        return m_ReceivedBytes;
    }

    size_t& GetSendBytes()
    {
        return m_SendBytes;
    }
};