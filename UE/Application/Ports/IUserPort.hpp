#pragma once

#include "Sms.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleSmsSend(Sms& sms) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;

    virtual void showSmsReceived() = 0;
    virtual void showSmsList() = 0;
    virtual void showSms(int id) = 0;
    virtual void showSmsCompose() = 0;
};

}
