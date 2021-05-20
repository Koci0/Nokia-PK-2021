#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"
#include "Sms.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSmsReceived(common::PhoneNumber from, std::string &text) override;
    void handleSmsUnknownRecipient() override;
    void handleCallRequest(common::PhoneNumber) override;
    void handleCallRequestAccept() override;
    void handleCallRequestReject() override;
    void handleCallUnknownRecipient(common::PhoneNumber) override;
    void handleCallAccepted(common::PhoneNumber from) override;
    void handleCallFailure(std::string &&) override;

    // IUserEventsHandler interface
    void handleSendCallRequest(common::PhoneNumber) override;
    void handleCallRequestResignation() override;
    void handleSmsSend(Sms& sms) override;
protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
