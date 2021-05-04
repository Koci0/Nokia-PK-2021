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
    void handleTimeout() final;
    void handleCallRequest(common::PhoneNumber) final;
    void handleCallRequestAccept(common::PhoneNumber fromPhoneNumber) final;
    void handleCallRequestReject(common::PhoneNumber fromPhoneNumber) final;
};

}
