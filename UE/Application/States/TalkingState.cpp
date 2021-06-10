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
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
}

void TalkingState::handleCallUnknownRecipient(common::PhoneNumber callingPhoneNumber)
{
    logger.logInfo("TalkingState::handleUnkownRecipient");
    context.user.showPeerUserDisconnected();
    // TODO: behave as if call was dropped
}

void TalkingState::handleTalkTextSend(std::string& text)
{
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
    context.bts.handleTalkMessageSend(text,withPhoneNumber);
    logger.logInfo("TalkingState::handleTalkTextSend");
    context.user.showTalking(text);
}

void TalkingState::handleTalkTextReceived(std::string& text)
{
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
    logger.logInfo("TalkingState::handleTalkTextReceived");
    context.user.showTalking(text);
}

void TalkingState::handleTimeout()
{
    logger.logInfo("TalkingState: No activity, dropping call");
    context.user.showPeerUserDisconnected();
    // TODO: drop call
}

}
