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
    context.bts.sendCallReject(context.callingPhoneNumber);
    context.callingPhoneNumber.value = 0;
}

void ConnectedState::handleCallRequest(common::PhoneNumber callingPhoneNumber)
{
    logger.logInfo("ConnectedState::handleCallRequest");
    context.callingPhoneNumber = callingPhoneNumber;

    context.user.showCallRequest(context.callingPhoneNumber);
    auto acceptButtonCallback = [this]() {
        logger.logInfo("handleCallRequestAccept()");
        handleCallRequestAccept();
    };
    auto rejectButtonCallback = [this]() {
        logger.logInfo("handleCallRequestReject()");
        handleCallRequestReject();
    };
    context.user.setupIncomingCallButtons(acceptButtonCallback, rejectButtonCallback);

    using namespace std::chrono_literals;
    context.timer.startTimer(30s);
}

void ConnectedState::handleCallRequestAccept()
{
    logger.logInfo("ConnectedState::handleCallRequestAccept");
    context.timer.stopTimer();
    context.user.resetButtons();
    context.setState<TalkingState>(context.callingPhoneNumber);
}

void ConnectedState::handleCallRequestReject()
{
    logger.logInfo("ConnectedState::handleCalLRequestReject");
    context.timer.stopTimer();
    context.user.resetButtons();
    context.user.showConnected();
    context.bts.sendCallReject(context.callingPhoneNumber);
    context.callingPhoneNumber.value = 0;
}

}
