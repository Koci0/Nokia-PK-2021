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

void ConnectedState::handleTimeout()
{
    logger.logInfo("ConnectedState::handleTimeout");
    context.user.resetButtons();
    context.user.showConnected();
    context.bts.sendCallReject(context.foreignPhoneNumber);
}

void ConnectedState::handleCallRequest(common::PhoneNumber fromPhoneNumber)
{
    logger.logInfo("ConnectedState::handleCallRequest");
    context.user.showCallRequest(fromPhoneNumber);

    auto acceptButtonCallback = [this, fromPhoneNumber]() {
        logger.logInfo("handleCallRequestAccept()");
        handleCallRequestAccept(fromPhoneNumber);
    };
    auto rejectButtonCallback = [this, fromPhoneNumber]() {
        logger.logInfo("handleCallRequestReject()");
        handleCallRequestReject(fromPhoneNumber);
    };
    context.user.setupIncomingCallButtons(acceptButtonCallback, rejectButtonCallback);

    context.foreignPhoneNumber = fromPhoneNumber;
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

void ConnectedState::handleCallRequestReject(common::PhoneNumber fromPhoneNumber)
{
    logger.logInfo("ConnectedState::handleCalLRequestReject");
    context.timer.stopTimer();
    context.user.resetButtons();
    context.user.showConnected();
    context.bts.sendCallReject(fromPhoneNumber);
}

}
