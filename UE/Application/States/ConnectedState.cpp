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

void ConnectedState::handleSmsSend(Sms &sms)
{
    context.bts.handleMessageSend(sms);
}

void ConnectedState::markSmsAsFailed()
{
    Sms* sms = context.db.getLast();
    sms->isFailed = true;
    logger.logInfo("last sms marked as failed");
}

}
