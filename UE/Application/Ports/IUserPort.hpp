#pragma once

#include "Messages/PhoneNumber.hpp"

#include <functional>

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleSendCallRequest(common::PhoneNumber to) = 0;
    virtual void handleCallRequestResignation() = 0;
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
};

}
