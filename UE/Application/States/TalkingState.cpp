#include "TalkingState.hpp"

#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState")
{
    logger.logInfo("TalkingState::TalkingState");
    context.user.resetButtons();
    auto dropButtonCallback = [&]() {
        context.timer.stopTimer();
        context.bts.sendCallDropped(context.callingPhoneNumber);
        context.callingPhoneNumber.value = 0;
        context.setState<ConnectedState>();
    };
    context.user.setupTalkingButtons(dropButtonCallback);
    std::string empty = ""; // TODO: std::optional
    context.user.showTalking(empty);
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
}

void TalkingState::handleCallUnknownRecipient()
{
    logger.logInfo("TalkingState::handleUnknownRecipient");
    context.callingPhoneNumber.value = 0;
    context.user.showShortInfo("User is not connected.");
    context.setState<ConnectedState>();
}

void TalkingState::handleSendCallTalk(std::string& text)
{
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
    context.bts.sendCallTalk(text, context.callingPhoneNumber);
    logger.logInfo("TalkingState::handleSendCallTalk");
    context.user.showTalking(text);
}

void TalkingState::handleCallFailure(std::string &&message)
{
    logger.logInfo("TalkingState::handleCallFailure");
    context.callingPhoneNumber.value = 0;
    context.timer.stopTimer();
    context.setState<ConnectedState>();
    context.user.showShortInfo(std::move(message));
}

void TalkingState::handleCallTalk(std::string& text)
{
    logger.logInfo("TalkingState::handleCallTalk");
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
    logger.logInfo("TalkingState::handleCallTalk");
    context.user.showTalking(text);
}

void TalkingState::handleTimeout()
{
    logger.logInfo("TalkingState: No activity, dropping call");
    context.bts.sendCallDropped(context.callingPhoneNumber);
    context.callingPhoneNumber.value = 0;
    context.setState<ConnectedState>();
    context.user.showShortInfo(std::move("Timeout."));
}

}
