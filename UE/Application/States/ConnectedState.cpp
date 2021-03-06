#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    logger.logInfo("ConnectedState::ConnectedState");
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleSms(common::PhoneNumber from, std::string &text)
{
    Sms sms(from, text);
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
    context.bts.sendCallAccept(context.callingPhoneNumber);
    context.setState<TalkingState>();
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
    logger.logInfo("ConnectedState::handleSendCallRequest: ", to);
    context.callingPhoneNumber = to;
    using namespace std::chrono_literals;
    context.timer.startTimer(60s);
    context.bts.sendCallRequest(context.callingPhoneNumber);
}

void ConnectedState::handleCallAccepted()
{
    logger.logInfo("ConnectedState: handleCallAccepted");
    context.timer.stopTimer();
    context.setState<TalkingState>();
}

void ConnectedState::handleCallFailure(std::string &&message)
{
    logger.logInfo("ConnectedState::handleCallFailure");
    context.user.showShortInfo(std::move(message));
    context.timer.stopTimer();
}

void ConnectedState::handleCallUnknownRecipient()
{
    logger.logInfo("ConnectedState::handleCallUnknownRecipient");
    handleCallFailure("User is not connected.");
}

void ConnectedState::handleCallRequestResignation()
{
    logger.logInfo("ConnectedState: handleCallRequestResignation");
    context.timer.stopTimer();
    context.bts.sendCallDropped(context.callingPhoneNumber);
    context.user.showShortInfo("You dropped a call.");
}

void ConnectedState::handleSendSms(Sms &sms)
{
    context.bts.sendSms(sms);
}

void ConnectedState::handleSmsUnknownRecipient()
{
    Sms* sms = context.db.getLast();
    sms->isFailed = true;
    logger.logInfo("ConnectedState: handleSmsUnknownRecipient - sms marked as failed");
}

}
