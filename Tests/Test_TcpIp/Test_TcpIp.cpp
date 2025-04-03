#include <gtest/gtest.h>
#include "TcpIp.hpp"
#include "Common.hpp"
#include "TcpIpConfiguratorMock.hpp"

ServerAndTcpIpNotificationS serverAndTcpIpNotification[MAX_NUM_OF_CLIENTS];
// TcpIpReader_Suite

// TEST1: Creation of TcpIp object is succesfull
TEST(TcpIpReader_Suite, TcpIpObjectCreation)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    // Expect not equal null.
    ASSERT_NE(&tcpIpObj, nullptr);
}

// TEST2: Read over tcpIp. Expect the Read method is called
TEST(TcpIpReader_Suite, TcpIpReadMethodIsCalled)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    EXPECT_CALL(tcpIpConfMock, Read()).Times(1);

    tcpIpObj.Read();
}

// TEST3: Read over tcpIp. Expect the Read method sets the reception buffer
TEST(TcpIpReader_Suite, TcpIpReadProperReceptionBuffer)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    ON_CALL(tcpIpConfMock, Read()).WillByDefault(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "Check if buffer is set";
    }));

    tcpIpObj.Read();

    ASSERT_STREQ("Check if buffer is set", tcpIpObj.GetTcpIpConfiguration().GetReceptionBuffer().c_str());
}

// TEST4: Read over tcpIp. Expect that after Read The Message is being set (appended)
TEST(TcpIpReader_Suite, TcpIpReadAppendToReceivedMessage)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    ON_CALL(tcpIpConfMock, Read()).WillByDefault(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "Appended msg";
    }));

    tcpIpObj.Read();

    ASSERT_STREQ(tcpIpObj.GetReceivedMsg().c_str(), tcpIpObj.GetTcpIpConfiguration().GetReceptionBuffer().c_str());
}

// TEST5: Read over tcpIp. The read is divided in chunks, which comes within 1 sec
TEST(TcpIpReader_Suite, TcpIpReadInChunksWithinOneSec)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "First Part Of The Msg ";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(0));

    tcpIpObj.Read();

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "Second Part Of The Msg";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(0));

    tcpIpObj.Read();

    ASSERT_STREQ(tcpIpObj.GetReceivedMsg().c_str(), "First Part Of The Msg Second Part Of The Msg");
}

// TEST6: Read over tcpIp. The read is divided in chunks, which time between them is more than 1 sec
TEST(TcpIpReader_Suite, TcpIpReadInChunksWithMoreThanOneSec)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "First Part Of The Msg ";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(0));

    tcpIpObj.Read();

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "Second Part Of The Msg";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(2));

    tcpIpObj.Read();

    ASSERT_STREQ(tcpIpObj.GetReceivedMsg().c_str(), "Second Part Of The Msg");
}

// TEST7: Read over tcpIp. The message contains USB notification tag
TEST(TcpIpReader_Suite, TcpIpReadTriggerUsb)
{
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "A-whatever-A";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(0));

    tcpIpObj.Read();

    ASSERT_EQ(tcpIpObj.GetTriggerUsb(), true);
    ASSERT_STREQ((*g_TcpIpAndUsbMsgs.begin()).c_str(), "A-whatever-A");
    // remain last A in case if it will be starting new trigger message
    ASSERT_STREQ(tcpIpObj.GetReceivedMsg().c_str(), "A");
}

// TEST8: Read over tcpIp. The message contains USB notification tag
TEST(TcpIpReader_Suite, TcpIpReadTriggerUsbTwoMsgs)
{
    g_TcpIpAndUsbMsgs.clear();
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "A-whatever-A";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(0));

    tcpIpObj.Read();

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "SPACEA-second-A";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(0));

    tcpIpObj.Read();

    ASSERT_EQ(tcpIpObj.GetTriggerUsb(), true);
    ASSERT_STREQ((*g_TcpIpAndUsbMsgs.begin()).c_str(), "A-whatever-A");
    ASSERT_STREQ((*(g_TcpIpAndUsbMsgs.begin() + 1)).c_str(), "ASPA");
    ASSERT_STREQ((*(g_TcpIpAndUsbMsgs.begin() + 2)).c_str(), "ACEA");
    ASSERT_STREQ((*(g_TcpIpAndUsbMsgs.begin() + 3)).c_str(), "A-second-A");
    ASSERT_STREQ(tcpIpObj.GetReceivedMsg().c_str(), "A");
}

// TEST9: Read over tcpIp. Messages comes after some time
TEST(TcpIpReader_Suite, TcpIpReadTriggerUsbFirstMsgAfterSomeTime)
{
    g_TcpIpAndUsbMsgs.clear();
    TcpIpConfiguratorMock tcpIpConfMock;
    TcpIp tcpIpObj(tcpIpConfMock);

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "A-whatever-A";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(2));

    tcpIpObj.Read();

    ASSERT_EQ(tcpIpObj.GetTriggerUsb(), true);
    ASSERT_STREQ((*g_TcpIpAndUsbMsgs.begin()).c_str(), "A-whatever-A");

    EXPECT_CALL(tcpIpConfMock, Read()).WillOnce(::testing::Invoke([&tcpIpConfMock](){
        tcpIpConfMock.GetReceptionBuffer() = "A-secondWhatever-A";
    }));
    EXPECT_CALL(tcpIpConfMock, GetElapsedTimeBetweenRead()).Times(1).WillOnce(::testing::Return(3));

    tcpIpObj.Read();

    ASSERT_EQ(tcpIpObj.GetTriggerUsb(), true);
    ASSERT_STREQ((*g_TcpIpAndUsbMsgs.begin()).c_str(), "A-secondWhatever-A");
}
