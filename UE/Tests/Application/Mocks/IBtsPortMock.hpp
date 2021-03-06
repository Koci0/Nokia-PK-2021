#pragma once

#include <gmock/gmock.h>
#include "Ports/IBtsPort.hpp"
#include "Sms.hpp"

namespace ue
{

class IBtsEventsHandlerMock : public IBtsEventsHandler
{
public:
    IBtsEventsHandlerMock();
    ~IBtsEventsHandlerMock() override;

    MOCK_METHOD(void, handleDisconnected, (), (final));
    MOCK_METHOD(void, handleSib, (common::BtsId), (final));
    MOCK_METHOD(void, handleAttachAccept, (), (final));
    MOCK_METHOD(void, handleAttachReject, (), (final));
    MOCK_METHOD(void, handleSms, (common::PhoneNumber, std::string &),(final));
    MOCK_METHOD(void, handleSmsUnknownRecipient, (), (final));
    MOCK_METHOD(void, handleCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallUnknownRecipient, (), (final));
    MOCK_METHOD(void, handleCallAccepted,(), (final));
    MOCK_METHOD(void, handleCallFailure,(std::string &&), (final));
    MOCK_METHOD(void, handleCallTalk, (std::string&), (final));
};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(void, sendCallAccept, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallDropped, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendSms, (ue::Sms&), (final));
    MOCK_METHOD(void, sendCallTalk, (std::string&, common::PhoneNumber),(final));
};

}
