#pragma once
#include <string>

class ICommunication
{
    public:
    ICommunication(size_t size)
        : m_Buffer(std::string(size, '\0'))
    {}
    virtual ~ICommunication() {}

    virtual void Send(const std::string& data) = 0;
    virtual const std::string& Read() = 0;

    std::string& GetBuffer()
    {
        return m_Buffer;
    }

    private:
    std::string m_Buffer;
};

