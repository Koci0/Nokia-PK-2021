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

void ConnectedState::handleSmsReceived(common::PhoneNumber from, std::string &text)
{
    Sms sms(from,text);
    context.db.addOne(sms);
    context.user.showSmsNew();
}

void ConnectedState::handleTimeout()
{
    logger.logInfo("ConnectedState::handleTimeout");
    context.user.resetButtons();
    context.user.showShortInfo("Timeout.");
    context.bts.sendCallDropped(context.callingPhoneNumber);
    context.callingPhoneNumber.value = common::PhoneNumber::INVALID_VALUE;
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
    context.bts.sendCallDropped(context.callingPhoneNumber);
    context.callingPhoneNumber.value = 0;
}

void ConnectedState::handleSendCallRequest(common::PhoneNumber to)
{
    context.callingPhoneNumber = to;
    context.bts.sendCallRequest(context.callingPhoneNumber);
    using namespace std::chrono_literals;
    context.timer.startTimer(60s);
}

void ConnectedState::handleCallAccepted()
{
    // TODO change mode to talking state
    logger.logInfo("ConnectedState: handleCallAccepted");
    context.timer.stopTimer();
}

void ConnectedState::handleCallFailure(std::string &&message)
{
    logger.logInfo("ConnectedState: handleCallFailure");
    context.user.showShortInfo(std::move(message));
    context.timer.stopTimer();
}

void ConnectedState::handleUnknownRecipient(common::PhoneNumber)
{
    handleCallFailure("User is not connected.");
}

void ConnectedState::handleCallRequestResignation()
{
    logger.logInfo("ConnectedState: handleCallRequestResignation");
    context.timer.stopTimer();
    context.bts.sendCallDropped(context.callingPhoneNumber);
    context.user.showShortInfo("You dropped a call.");
void ConnectedState::handleSmsSend(Sms &sms)
{
    context.bts.handleMessageSend(sms);
}

}
