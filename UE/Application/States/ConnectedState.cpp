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
}

void ConnectedState::handleCallAccepted()
{
    // TODO change mode to talking state
    logger.logInfo("ConnectedState: handleCallAccepted");
}

void ConnectedState::handleCallFailure(std::string message)
{
    logger.logInfo("ConnectedState: handleCallFailure");
    context.user.showShortInfo(message);
}

}
