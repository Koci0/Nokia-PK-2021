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
    logger.logInfo("ConnectedState::handleCallRequest()");
    auto accept = [this, fromPhoneNumber]() {
        logger.logInfo("handleCallRequestAccept()");
        context.timer.stopTimer();
        context.setState<TalkingState>(fromPhoneNumber);
    };
    auto reject = [this]() {
        logger.logInfo("handleCallRequestReject()");
        // TODO
    };
    context.user.setupButtons(accept, reject);
    context.user.showIncomingCall(fromPhoneNumber);
    using namespace std::chrono_literals;
    context.timer.startTimer(30s);
}

}
