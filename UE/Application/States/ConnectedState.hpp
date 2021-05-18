#pragma once

#include "Sms.hpp"
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

    // IUserEventsHandler interface
    void handleSmsSend(Sms& sms) final;
    void markSmsAsFailed() final;
};

}
