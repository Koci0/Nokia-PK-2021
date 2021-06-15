#include "TalkingState.hpp"

#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState")
{
    context.user.showTalking();
    auto dropButtonCallback = [&]() {
        context.bts.sendCallDropped(context.callingPhoneNumber);
        context.setState<ConnectedState>();
        context.callingPhoneNumber.value = 0;
    };
    context.user.setupTalkingButtons(dropButtonCallback);
}

void TalkingState::handleCallUnknownRecipient()
{
    logger.logInfo("TalkingState::handleUnknownRecipient");
    context.setState<ConnectedState>();
    context.callingPhoneNumber.value = 0;
    context.user.showShortInfo("User is not connected.");
}

void TalkingState::handleCallFailure(std::string &&message)
{
    logger.logInfo("TalkingState: handleCallFailure");
    context.setState<ConnectedState>();
    context.callingPhoneNumber.value = 0;
    context.user.showShortInfo(std::move(message));
}

}
