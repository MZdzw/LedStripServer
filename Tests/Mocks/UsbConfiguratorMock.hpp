#pragma once
#include "Protocol.hpp"
#include <gmock/gmock.h>

struct UsbConfiguratorMock : public IProtocol
{
    UsbConfiguratorMock()
    : IProtocol(256)
    { }

    MOCK_METHOD(void, Read, (), (override));
    MOCK_METHOD(void, Send, (), (override));
    MOCK_METHOD(int, GetElapsedTimeBetweenRead, (), (const, override));
    MOCK_METHOD(bool, IsConnected, (), (const, override));
    MOCK_METHOD(void, ChangeMode, (ModeE), (override));
};
