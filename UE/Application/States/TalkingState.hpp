#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber withPhoneNumber);

    // IBtsEventsHandler interface
public:
};

}