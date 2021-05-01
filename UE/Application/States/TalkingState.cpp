#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber withPhoneNumber)
    : BaseState(context, "TalkingState")
{
    context.user.showTalking();
    context.bts.sendCallAccept(withPhoneNumber);
}

}
