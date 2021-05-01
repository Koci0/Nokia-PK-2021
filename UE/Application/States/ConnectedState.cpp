#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleCallRequest(common::PhoneNumber fromPhoneNumber)
{
    logger.logInfo("ConnectedState::handleCallRequest");
    context.user.showCallRequest(fromPhoneNumber);

    auto acceptButtonCallback = [this, fromPhoneNumber]() {
        logger.logInfo("handleCallRequestAccept()");
        handleCallRequestAccept(fromPhoneNumber);
    };
    auto rejectButtonCallback = [this]() {
        logger.logInfo("handleCallRequestReject()");
        // TODO
    };
    context.user.setupIncomingCallButtons(acceptButtonCallback, rejectButtonCallback);

    using namespace std::chrono_literals;
    context.timer.startTimer(30s);
}

void ConnectedState::handleCallRequestAccept(common::PhoneNumber fromPhoneNumber)
{
    logger.logInfo("ConnectedState::handleCallRequestAccept");
    context.timer.stopTimer();
    context.user.resetButtons();
    context.setState<TalkingState>(fromPhoneNumber);
}

}
