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
    void handleCallRequest(common::PhoneNumber) final;
    void handleCallRequestAccept(common::PhoneNumber fromPhoneNumber) final;
};

}
