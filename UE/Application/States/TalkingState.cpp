#include "TalkingState.hpp"

#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber withPhoneNumber)
    : BaseState(context, "TalkingState")
{
    context.user.showTalking();
    context.user.setupIncomingCallButtons(nullptr, nullptr);
    context.bts.sendCallAccept(withPhoneNumber);
}

void TalkingState::handleUnknownRecipient(common::PhoneNumber callingPhoneNumber)
{
    logger.logInfo("TalkingState::handleUnkownRecipient");
    context.user.showPeerUserDisconnected();
    // TODO: behave as if call was dropped
}

}
