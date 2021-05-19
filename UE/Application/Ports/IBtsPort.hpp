#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleDisconnected() = 0;
    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleSmsReceived(common::PhoneNumber, std::string&) = 0;
    virtual void handleCallRequest(common::PhoneNumber) = 0;
    virtual void handleUnknownRecipient(common::PhoneNumber) = 0;
    virtual void handleCallAccepted() = 0;
    virtual void handleCallFailure(std::string &&) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendCallAccept(common::PhoneNumber) = 0;
    virtual void sendCallRequest(common::PhoneNumber) = 0;
    virtual void sendCallDropped(common::PhoneNumber) = 0;
};

}
