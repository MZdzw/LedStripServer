#pragma once
#include "Protocol.hpp"
#include <gmock/gmock.h>

struct TcpIpConfiguratorMock : public IProtocol
{
    TcpIpConfiguratorMock()
    : IProtocol(128)
    { }

    MOCK_METHOD(void, Read, (), (override));
    MOCK_METHOD(void, Send, (), (override));
    MOCK_METHOD(int, GetElapsedTimeBetweenRead, (), (const, override));
    MOCK_METHOD(bool, IsConnected, (), (const, override));
};

struct TcpIpConfiguratorStub : public IProtocol
{
    TcpIpConfiguratorStub()
    : IProtocol(128)
    { }

    void Read() override
    {
        GetReceptionBuffer() = "Check if buffer is set";
    }

    void Send() override
    {

    }

    bool IsConnected() const override
    {
        return false;
    }
    
};