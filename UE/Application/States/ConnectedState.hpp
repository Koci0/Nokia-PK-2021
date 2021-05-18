#pragma once

#include "Sms.hpp"
#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    // ITimerEventsHandler interface
public:
    void handleTimeout() final;

    // IBtsEventsHandler interface
public:
    void handleDisconnected() final;
    void handleSmsReceived(common::PhoneNumber from, std::string &text) final;
    void handleCallRequest(common::PhoneNumber) final;
    void handleCallRequestAccept() final;
    void handleCallRequestReject() final;
    void handleCallAccepted() final;
    void handleCallFailure(std::string &&) final;
    void handleUnknownRecipient(common::PhoneNumber callingPhoneNumber) final;

    // IUserEventsHandler interface
    void handleSendCallRequest(common::PhoneNumber to) final;
    void handleCallRequestResignation() final;
    void handleSmsSend(Sms& sms) final;
    void markSmsAsFailed() final;
};

}
