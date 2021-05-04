#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleSendCallRequest(common::PhoneNumber to) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void setupCallReceiver() = 0;
    virtual void showShortInfo(std::string) = 0;
};

}
