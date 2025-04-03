#pragma once
#include <string>

class IProtocol
{
private:
    std::string m_SendBuffer;
    std::string m_ReceptionBuffer;

public:
    IProtocol(size_t size)
    : m_SendBuffer(std::string(size, '\0')),
      m_ReceptionBuffer(std::string(size, '\0'))
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
};