#include "TalkingState.hpp"

#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber withPhoneNumber)
    : BaseState(context, "TalkingState"), withPhoneNumber{withPhoneNumber}
{
    context.user.resetButtons();
    std::string empty = "";
    context.user.showTalking(empty);
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

void TalkingState::handleSendCallTalk(std::string& text)
{
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
    context.bts.sendCallTalk(text,withPhoneNumber);
    logger.logInfo("TalkingState::handleSendCallTalk");
    context.user.showTalking(text);
}

void TalkingState::handleCallTalk(std::string& text)
{
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
    logger.logInfo("TalkingState::handleCallTalk");
    context.user.showTalking(text);
}

void TalkingState::handleTimeout()
{
    logger.logInfo("TalkingState: No activity, dropping call");
    context.user.showPeerUserDisconnected();
    // TODO: drop call
}

}
