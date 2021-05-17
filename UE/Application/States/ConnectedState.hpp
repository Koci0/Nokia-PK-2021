#pragma once

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
    void handleCallAccepted() final;
    void handleCallFailure(std::string &&) final;

    // IUserEventsHandler interface
    void handleSendCallRequest(common::PhoneNumber to) final;
    void handleCallRequestResignation() final;
};

}
