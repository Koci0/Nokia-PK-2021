#pragma once

#include <functional>

#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleCallRequestAccept(common::PhoneNumber fromPhoneNumber) = 0;
    virtual void handleCallRequestReject(common::PhoneNumber fromPhoneNumber) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showCallRequest(common::PhoneNumber) = 0;
    virtual void resetButtons() = 0;
    virtual void setupIncomingCallButtons(std::function<void()>, std::function<void()>) = 0;
    virtual void showTalking() = 0;
    virtual void showPeerUserDisconnected() = 0;
    virtual void showShortInfo(std::string &&message) = 0;
};

}
