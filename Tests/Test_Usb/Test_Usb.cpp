#include <gtest/gtest.h>
#include "Usb.hpp"
#include "Common.hpp"
#include "UsbConfiguratorMock.hpp"

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
