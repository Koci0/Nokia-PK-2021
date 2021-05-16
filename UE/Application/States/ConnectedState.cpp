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

void ConnectedState::handleSmsReceived(common::PhoneNumber from, std::string &text)
{
    Sms sms(from,text);
    context.db.addOne(sms);
    context.user.showSmsNew();
}

}
