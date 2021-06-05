#include "TalkingState.hpp"

#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber withPhoneNumber)
    : BaseState(context, "TalkingState"), withPhoneNumber{withPhoneNumber}
{
    context.user.resetButtons();
    std::string text = "";
    context.user.showTalking(text);
    context.bts.sendCallAccept(withPhoneNumber);
}

void TalkingState::handleCallUnknownRecipient(common::PhoneNumber callingPhoneNumber)
{
    logger.logInfo("TalkingState::handleUnkownRecipient");
    context.user.showPeerUserDisconnected();
    // TODO: behave as if call was dropped
}

void TalkingState::handleTalkTextSend(std::string& text)
{
    context.bts.handleTalkMessageSend(text,withPhoneNumber);
    logger.logInfo("TalkingState: Sending text: ", text);
    context.user.showTalking(text);
}

void TalkingState::handleTalkTextReceived(std::string& text)
{
    logger.logInfo("virtual void handleTalkTextReceived(std::string&) = 0", text);
    context.user.showTalking(text);
}

}
