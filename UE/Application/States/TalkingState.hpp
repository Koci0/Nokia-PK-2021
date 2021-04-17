#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState: public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber toPhoneNumber);

    void handleCallAccepted(common::PhoneNumber toPhoneNumber) override;
};

}
