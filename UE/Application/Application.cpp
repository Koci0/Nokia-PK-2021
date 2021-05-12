#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer)
    : context{iLogger, bts, user, timer},
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
    context.state->handleCallRequest(common::PhoneNumber{1});
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleCallRequest(common::PhoneNumber fromPhoneNumber)
{
    context.state->handleCallRequest(fromPhoneNumber);
}

void Application::handleCallRequestAccept()
{
    context.state->handleCallRequestAccept();
}

void Application::handleCallRequestReject()
{
    context.state->handleCallRequestReject();
}

void Application::handleUnknownRecipient(common::PhoneNumber callingPhoneNumber)
{
    context.state->handleUnknownRecipient(callingPhoneNumber);
}

}
