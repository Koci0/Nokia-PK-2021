#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

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

void ConnectedState::handleSendCallRequest(common::PhoneNumber to)
{
    context.bts.sendCallRequest(to);
    using namespace std::chrono_literals;
    context.timer.startTimer(60s);
}

void ConnectedState::handleCallAccepted()
{
    // TODO change mode to talking state
    logger.logInfo("ConnectedState: handleCallAccepted");
    context.timer.stopTimer();
}

void ConnectedState::handleCallFailure(std::string message)
{
    logger.logInfo("ConnectedState: handleCallFailure");
    context.user.showShortInfo(message);
    context.timer.stopTimer();
}

    void ConnectedState::handleTimeout()
    {
        logger.logInfo("Timeout");
        context.user.showShortInfo("Timeout.");
}

}
