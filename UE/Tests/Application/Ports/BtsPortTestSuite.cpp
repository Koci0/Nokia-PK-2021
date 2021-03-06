#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/BtsPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Messages/IncomingMessage.hpp"

namespace ue
{
using namespace ::testing;

class BtsPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::PhoneNumber TO_PHONE_NUMBER{113};
    const common::BtsId BTS_ID{13121981ll};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsEventsHandlerMock> handlerMock;
    StrictMock<common::ITransportMock> transportMock;
    common::ITransport::MessageCallback messageCallback;
    common::ITransport::DisconnectedCallback disconnectCallback;

    BtsPort objectUnderTest{loggerMock, transportMock, PHONE_NUMBER};

    BtsPortTestSuite()
    {
        EXPECT_CALL(transportMock, registerMessageCallback(_))
                .WillOnce(SaveArg<0>(&messageCallback));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(_))
                .WillOnce(SaveArg<0>(&disconnectCallback));
        objectUnderTest.start(handlerMock);
    }
    ~BtsPortTestSuite()
    {

        EXPECT_CALL(transportMock, registerMessageCallback(IsNull()));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(IsNull()));
        objectUnderTest.stop();
    }
};

TEST_F(BtsPortTestSuite, shallRegisterHandlersBetweenStartStop)
{
}

TEST_F(BtsPortTestSuite, shallIgnoreWrongMessage)
{
    common::OutgoingMessage wrongMsg{};
    wrongMsg.writeBtsId(BTS_ID);
    messageCallback(wrongMsg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleDisconnected)
{
    EXPECT_CALL(handlerMock, handleDisconnected());
    disconnectCallback();
}

TEST_F(BtsPortTestSuite, shallHandleSib)
{
    EXPECT_CALL(handlerMock, handleSib(BTS_ID));
    common::OutgoingMessage msg{common::MessageId::Sib,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeBtsId(BTS_ID);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachAccept)
{
    EXPECT_CALL(handlerMock, handleAttachAccept());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(true);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachReject)
{
    EXPECT_CALL(handlerMock, handleAttachReject());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendAttachRequest)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });
    objectUnderTest.sendAttachRequest(BTS_ID);
    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::AttachRequest, reader.readMessageId()) );
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{}, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(BTS_ID, reader.readBtsId()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallHandleSms)
{
    EXPECT_CALL(handlerMock, handleSms);
    common::OutgoingMessage msg{common::MessageId::Sms,
                            common::PhoneNumber{},
                            PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}
  
TEST_F(BtsPortTestSuite, shallSendCallReject)
{
    common::BinaryMessage msg;
    common::PhoneNumber toPhoneNumber{1};
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });
    objectUnderTest.sendCallDropped(toPhoneNumber);
    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallDropped, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(toPhoneNumber, reader.readPhoneNumber()));
}

TEST_F(BtsPortTestSuite, shallHandleUnknownRecipient)
{
    EXPECT_CALL(handlerMock, handleCallUnknownRecipient());
    common::OutgoingMessage msg{common::MessageId::UnknownRecipient,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallRequestOnDifferentPhoneNumber)
{
    common::OutgoingMessage msg = {common::MessageId::AttachRequest,
                                   PHONE_NUMBER,
                                   TO_PHONE_NUMBER};
    EXPECT_CALL(transportMock, sendMessage(_));
    objectUnderTest.sendCallRequest(TO_PHONE_NUMBER);
}

TEST_F(BtsPortTestSuite, shallNotSendCallRequestOnSamePhoneNumber)
{
    EXPECT_CALL(handlerMock, handleCallFailure(_));
    objectUnderTest.sendCallRequest(PHONE_NUMBER);
}

TEST_F(BtsPortTestSuite, shallHandleCallAccepted)
{
    EXPECT_CALL(handlerMock, handleCallAccepted());
    common::OutgoingMessage msg{common::MessageId::CallAccepted,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleCallDropped)
{
    EXPECT_CALL(handlerMock, handleCallFailure(_));
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleOnUnknownRecipient)
{
    EXPECT_CALL(handlerMock, handleCallUnknownRecipient());
    common::OutgoingMessage msg{common::MessageId::UnknownRecipient,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallDropped)
{
    EXPECT_CALL(transportMock, sendMessage(_));
    common::OutgoingMessage msg = {common::MessageId::CallDropped,
                                   PHONE_NUMBER,
                                   PHONE_NUMBER};
    objectUnderTest.sendCallDropped(PHONE_NUMBER);
}

TEST_F(BtsPortTestSuite, shallSendCallTalk)
{
    EXPECT_CALL(transportMock, sendMessage(_));
    common::OutgoingMessage msg = {common::MessageId::CallTalk,
                                   PHONE_NUMBER,
                                   PHONE_NUMBER};
    std::string text = "text";
    objectUnderTest.sendCallTalk(text,PHONE_NUMBER);
}

TEST_F(BtsPortTestSuite, shallHandleCallTalk)
{
    EXPECT_CALL(handlerMock, handleCallTalk(_));
    common::OutgoingMessage msg = {common::MessageId::CallTalk,
                                   PHONE_NUMBER,
                                   PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}



}
