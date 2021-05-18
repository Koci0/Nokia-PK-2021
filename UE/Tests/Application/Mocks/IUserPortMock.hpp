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

    MOCK_METHOD(void, showSms, (int), (final));
    MOCK_METHOD(void, showSmsList, (), (final));
    MOCK_METHOD(void, showSmsReceived, (), (final));
    MOCK_METHOD(void, showSmsCompose, (), (final));
};

}
