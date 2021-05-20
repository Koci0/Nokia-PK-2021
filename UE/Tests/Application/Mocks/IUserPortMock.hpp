#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"
#include "Sms.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleCallRequestAccept, (), (final));
    MOCK_METHOD(void, handleCallRequestReject, (), (final));
    MOCK_METHOD(void, handleSendCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallRequestResignation, (), (final));
    MOCK_METHOD(void, handleSmsSend, (ue::Sms&), (final));
};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    
    MOCK_METHOD(void, setupCallReceiver, (), (final));
    MOCK_METHOD(void, showShortInfo, (std::string &&, InternalMethod), (final));
    MOCK_METHOD(void, callRequestResignation, (), (final));
    
    MOCK_METHOD(void, showSms, (int), (final));
    MOCK_METHOD(void, showSmsList, (), (final));
    MOCK_METHOD(void, showSmsNew, (), (final));
    MOCK_METHOD(void, showSmsNotNew, (), (final));

    MOCK_METHOD(void, showCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, resetButtons, (), (final));
    MOCK_METHOD(void, setupIncomingCallButtons, (std::function<void()>, std::function<void()>), (final));
    MOCK_METHOD(void, showTalking, (), (final));
    MOCK_METHOD(void, showPeerUserDisconnected, (), (final));
    MOCK_METHOD(void, showSmsCompose, (), (final));
};

}
