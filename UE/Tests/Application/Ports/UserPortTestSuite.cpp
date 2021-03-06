#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Mocks/ISmsDbMock.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;

    StrictMock<IDialModeMock> dialViewModeMock;
    StrictMock<ITextModeMock> alertViewModeMock;

    StrictMock<ISmsDbMock> smsDbMock;
    StrictMock<ITextModeMock> textModeMock;
    StrictMock<ICallModeMock> callModeMock;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER, smsDbMock};

    UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        objectUnderTest.start(handlerMock);
    }
    ~UserPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMenuOnConnected)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    objectUnderTest.showConnected();
}

TEST_F(UserPortTestSuite, shallShowIncomingCallOnCallRequest)
{
    EXPECT_CALL(guiMock, setAlertMode()).WillOnce(ReturnRef(textModeMock));
    EXPECT_CALL(textModeMock, setText(_));
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    EXPECT_CALL(guiMock, setRejectCallback(_));
    objectUnderTest.showCallRequest(PHONE_NUMBER);
    objectUnderTest.setupIncomingCallButtons(nullptr, nullptr);
}

TEST_F(UserPortTestSuite, shallShowDialActionModeOnSetupCallReceiver)
{
    EXPECT_CALL(guiMock, setDialMode()).WillOnce(ReturnRef(dialViewModeMock));
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    EXPECT_CALL(guiMock, setRejectCallback(_));
    objectUnderTest.setupCallReceiver();
}

TEST_F(UserPortTestSuite, shallInformUserInAlertMode)
{
    EXPECT_CALL(guiMock, setAlertMode()).WillOnce(ReturnRef(alertViewModeMock));
    EXPECT_CALL(alertViewModeMock, setText(_));
    EXPECT_CALL(guiMock, setRejectCallback(_));
    objectUnderTest.showShortInfo("_");
}

TEST_F(UserPortTestSuite, shallHandleCallRequestResignation)
{
    EXPECT_CALL(handlerMock, handleCallRequestResignation());
    objectUnderTest.callRequestResignation();
}

TEST_F(UserPortTestSuite, shallShowCallModeOnTalking)
{
    EXPECT_CALL(guiMock,setCallMode()).WillOnce(ReturnRef(callModeMock));
    std::string empty = "";
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    objectUnderTest.showTalking(empty);
}

}
