#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    // IBtsEventsHandler interface
public:
    void handleDisconnected() final;
    void handleCallAccepted() final;

    // IUserEventsHandler interface
    void handleSendCallRequest(common::PhoneNumber to) final;
};

}
