#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleDisconnected()
{
    logger.logError("Uexpected: handleDisconnected");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleSmsReceived(common::PhoneNumber, std::string&)
{
    logger.logError("Unexpected: handleSmsReceived");
}

void BaseState::handleSmsUnknownRecipient()
{
    logger.logError("Unexpected: handleSmsUnknownRecipient");
}

void BaseState::handleCallRequest(common::PhoneNumber)
{
    logger.logError("Unexpected: handleCallRequest");
}

void BaseState::handleCallRequestAccept()
{
    logger.logError("Unexpected: handleCallRequestAcept");
}

void BaseState::handleCallRequestReject()
{
    logger.logError("Unexpected: handleCallRequestReject");
}

void BaseState::handleCallUnknownRecipient(common::PhoneNumber)
{
    logger.logError("Unexpected: handleUnknownRecipient");
}

void BaseState::handleSendCallRequest(common::PhoneNumber)
{
    logger.logError("Unexpected: handleSendCallRequest");
}

void BaseState::handleCallAccepted(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallAccepted");
}

void BaseState::handleCallFailure(std::string &&)
{
    logger.logError("Unexpected: handleCallFailure");
}

void BaseState::handleCallRequestResignation()
{
    logger.logError("Unexpected: handleCallRequestResignation");
}

void BaseState::handleSmsSend(Sms &sms)
{
    logger.logError("Unexpected: handleSmsSend");
}
}
