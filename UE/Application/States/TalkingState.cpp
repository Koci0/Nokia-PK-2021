#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber toPhoneNumber)
    : BaseState(context, "TalkingState")
{
    logger.logInfo("TalkingState");
    context.user.setupButtons(nullptr, nullptr);
    context.user.showCallMode();
}

void TalkingState::handleCallAccepted(common::PhoneNumber toPhoneNumber)
{
    logger.logInfo("TalkingState::handleCallAccepted");
    context.bts.sendCallAccepted(toPhoneNumber);
}

}


