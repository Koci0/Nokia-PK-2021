#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber withPhoneNumber);

    // ITimerEventsHandler interface
    void handleTimeout() final;

    // IBtsEventsHandler interface
    void handleCallUnknownRecipient(common::PhoneNumber callingPhoneNumber) final;
    void handleCallTalk(std::string& text) final;

    // IUserEventsHandler interface
    void handleSendCallTalk(std::string& text) final;
private:
    common::PhoneNumber withPhoneNumber;

};

}
