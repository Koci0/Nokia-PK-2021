#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context);

    // ITimerEventsHandler interface
    void handleTimeout() final;

    // IBtsEventsHandler interface
    void handleCallTalk(std::string& text) final;

    // IUserEventsHandler interface
    void handleSendCallTalk(std::string& text) final;

private:
    void handleCallUnknownRecipient() final;
    void handleCallFailure(std::string &&) final;
};

}
