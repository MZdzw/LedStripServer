#include <gtest/gtest.h>
#include "Usb.hpp"
#include "Common.hpp"
#include "UsbConfiguratorMock.hpp"

TcpIpAndUsbNotificationS tcpIpAndUsbNotification;
// UsbReader_Suite

// TEST1: Creation of TcpIp object is succesfull
TEST(UsbReader_Suite, UsbObjectCreation)
{
    UsbConfiguratorMock usbConfMock;
    Usb usbObj(usbConfMock);

    // Expect not equal null.
    ASSERT_NE(&usbConfMock, nullptr);
}

// TEST2: Read over Usb and check conversion from bytes to string
TEST(UsbReader_Suite, UsbReadMethodAndConversionFromBytesToString)
{
    UsbConfiguratorMock usbConfMock;
    Usb usbObj(usbConfMock);

    EXPECT_CALL(usbConfMock, Read()).WillOnce(::testing::Invoke([&usbConfMock](){
        // In the UsbConfigurator we should append null terminator character at the end
        char bytes[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, '\0'}; // ASCII in bytes
        usbConfMock.GetReceivedBytes() = 12;
        // capacity should be 256 (check in the mock implementation)
        usbConfMock.GetReceptionBuffer() = std::string(bytes, usbConfMock.GetReceptionBuffer().capacity());
    }));
    EXPECT_CALL(usbConfMock, GetElapsedTimeBetweenRead()).Times(1);

    usbObj.Read();

    // Check conversion
    ASSERT_STREQ("01234567891011", usbObj.GetReceivedMsg().c_str());
}

// TEST3: Send over Usb and check if it's proper USB message
TEST(UsbReader_Suite, UsbSendUsbAndCheckIfItIsUsbMsg)
{
    ClearQueue(g_TcpIpAndUsbMsgs);
    UsbConfiguratorMock usbConfMock;
    Usb usbObj(usbConfMock);

    EXPECT_CALL(usbConfMock, Send()).Times(1);
    g_TcpIpAndUsbMsgs.push("A-0-1-A");

    usbObj.Send();

    std::vector<char> msgVal;
    for (int i = 0; i < 6; i++)
        msgVal.push_back(0xAA);
    msgVal.push_back(0);
    msgVal.push_back(1);
    for (int i = 0; i < 6; i++)
        msgVal.push_back(0xAA);

    std::string expectedMsg = std::string(msgVal.begin(), msgVal.end());
    // Check conversion
    ASSERT_EQ(expectedMsg, usbObj.GetSendMsg());

    g_TcpIpAndUsbMsgs.push("A-0-A");   // bad length

    usbObj.Send();

    ASSERT_EQ(expectedMsg, usbObj.GetSendMsg());    // should be the same message
    // NOTICE: the mock should not be called
}
