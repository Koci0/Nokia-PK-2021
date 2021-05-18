#pragma once

#include <functional>

#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleCallRequestAccept() = 0;
    virtual void handleCallRequestReject() = 0;
    virtual void handleSendCallRequest(common::PhoneNumber to) = 0;
    virtual void handleCallRequestResignation() = 0;
    virtual void handleSmsSend(Sms& sms) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
  
    virtual void setupCallReceiver() = 0;
    using InternalMethod = std::function<void(IUserPort*)>;
    virtual void showShortInfo(std::string &&, InternalMethod = &IUserPort::showConnected) = 0;
    virtual void callRequestResignation() = 0;

    virtual void showSmsNew() = 0;
    virtual void showSmsList() = 0;
    virtual void showSms(int id) = 0;

    virtual void showCallRequest(common::PhoneNumber) = 0;
    virtual void resetButtons() = 0;
    virtual void setupIncomingCallButtons(std::function<void()>, std::function<void()>) = 0;
    virtual void showTalking() = 0;
    virtual void showPeerUserDisconnected() = 0;

    virtual void showSmsCompose() = 0;
};

}
