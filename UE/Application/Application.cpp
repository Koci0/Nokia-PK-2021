#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         ISmsDb &db)
    : context{iLogger, bts, user, timer, db},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleSmsReceived(common::PhoneNumber from, std::string &text)
{
    context.state->handleSmsReceived(from, text);
}

void Application::handleSmsUnknownRecipient()
{
    context.state->handleSmsUnknownRecipient();
}

void Application::handleCallRequest(common::PhoneNumber callingPhoneNumber)
{
    context.state->handleCallRequest(callingPhoneNumber);
}

void Application::handleCallRequestAccept()
{
    context.state->handleCallRequestAccept();
}

void Application::handleCallRequestReject()
{
    context.state->handleCallRequestReject();
}

void Application::handleCallUnknownRecipient(common::PhoneNumber callingPhoneNumber)
{
    context.state->handleCallUnknownRecipient(callingPhoneNumber);
}

void Application::handleSendCallRequest(common::PhoneNumber to)
{
    context.state->handleSendCallRequest(to);
}

void Application::handleCallAccepted(common::PhoneNumber from)
{
    context.state->handleCallAccepted(from);
}

void Application::handleCallFailure(std::string &&message)
{
    context.state->handleCallFailure(std::move(message));
}

void Application::handleCallRequestResignation()
{
    context.state->handleCallRequestResignation();
}

void Application::handleSmsSend(Sms &sms)
{
    context.state->handleSmsSend(sms);
}
}
