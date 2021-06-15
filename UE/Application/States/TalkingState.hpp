#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context);

    // IBtsEventsHandler interface
    void handleCallUnknownRecipient() final;
    void handleCallFailure(std::string &&) final;
};

}
