#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/ISmsDbMock.hpp"
#include "Sms.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Messages/BtsId.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;
using namespace std::chrono_literals;

class ApplicationTestSuite : public Test
{
protected:
    const common::BtsId BTS_ID{42};
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<ISmsDbMock> smsDbMock;

    Expectation expectShowNotConnected = EXPECT_CALL(userPortMock, showNotConnected());
    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                smsDbMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{};

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite();
    void doConnecting();
};

ApplicationConnectingTestSuite::ApplicationConnectingTestSuite()
{
    doConnecting();
}

void ApplicationConnectingTestSuite::doConnecting()
{
    EXPECT_CALL(userPortMock, showConnecting());
    EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
    EXPECT_CALL(timerPortMock, startTimer(500ms));
    objectUnderTest.handleSib(BTS_ID);
}

TEST_F(ApplicationNotConnectedTestSuite, shallSendAttachRequestOnSib)
{
    // everything in constructor
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite();
    void doConnected();
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite()
{
    doConnected();
}

void ApplicationConnectedTestSuite::doConnected()
{
    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachAccept();
}

TEST_F(ApplicationConnectedTestSuite, shallShowConnectedOnAttachAccept)
{
    // see test-suite constructor
}

TEST_F(ApplicationConnectedTestSuite, shallShowNotConnectedOnDisconnectFromBts)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();
}

TEST_F(ApplicationConnectedTestSuite, shallReattach)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();

    doConnecting();
    doConnected();
}

TEST_F(ApplicationConnectedTestSuite, shallShowIncomingCallOnCallRequest)
{
    EXPECT_CALL(userPortMock, showCallRequest(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, setupIncomingCallButtons(_, _));
    EXPECT_CALL(timerPortMock, startTimer(_));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallDropCallOnCallRequestReject)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, resetButtons());
    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(btsPortMock, sendCallDropped(_));
    objectUnderTest.handleCallRequestReject();
}

TEST_F(ApplicationConnectedTestSuite, shallDropCallOnCallRequestTimeout)
{
    EXPECT_CALL(userPortMock, resetButtons());
    EXPECT_CALL(userPortMock, showShortInfo(_, _));
    EXPECT_CALL(btsPortMock, sendCallDropped(_));
    objectUnderTest.handleTimeout();
}

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    ApplicationTalkingTestSuite();
    void doTalking();
};

ApplicationTalkingTestSuite::ApplicationTalkingTestSuite()
{
    doTalking();
}

void ApplicationTalkingTestSuite::doTalking()
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallAccept(_));
    EXPECT_CALL(userPortMock, resetButtons());
    EXPECT_CALL(userPortMock, setupTalkingButtons(_));
    EXPECT_CALL(userPortMock, showTalking(_));
    EXPECT_CALL(timerPortMock, startTimer(_));
    objectUnderTest.handleCallRequestAccept();
}

TEST_F(ApplicationTalkingTestSuite, shallAcceptIncomingCallOnAcceptButtonPress)
{
    // see test-suite constructor
}

TEST_F(ApplicationTalkingTestSuite, shallReturnToMainMenuModeOnUnknownRecipient)
{
    EXPECT_CALL(userPortMock, showShortInfo(_, _));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallUnknownRecipient();
}

TEST_F(ApplicationTalkingTestSuite, shallReturnToMainMenuModeOnCallFailure)
{
    EXPECT_CALL(userPortMock, showShortInfo(_, _));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallFailure("");
}

TEST_F(ApplicationTalkingTestSuite, shallRestartTimerWhenMessageSend)
{
    std::string test = "test";
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(timerPortMock, startTimer(_));
    EXPECT_CALL(btsPortMock, sendCallTalk(test, _));
    EXPECT_CALL(userPortMock, showTalking(_));
    objectUnderTest.handleSendCallTalk(test);
}

TEST_F(ApplicationTalkingTestSuite, shallRestartTimerWhenMessageRecevied)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(timerPortMock, startTimer(_));
    EXPECT_CALL(userPortMock, showTalking(_));
    std::string test = "test";
    objectUnderTest.handleCallTalk(test);
}

}
