#pragma once

#include <functional>
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void setupButtons(std::function<void()> acceptCallback, std::function<void()> rejectCallback) = 0;
    virtual void showIncomingCall(common::PhoneNumber fromPhoneNumber) = 0;
    virtual void showCallMode() = 0;
};

}
