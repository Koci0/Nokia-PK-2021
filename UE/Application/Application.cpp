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

    // TODO: Temporary request simulation
    common::PhoneNumber tempPhoneNumber {255};
    this->handleCallRequest(tempPhoneNumber);
    // TODO: overall - remove logInfo
}

    void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleCallRequest(common::PhoneNumber fromPhoneNumber)
{
    logger.logInfo("handleCallRequest: ", fromPhoneNumber);
    context.state->handleCallRequest(fromPhoneNumber);
}

void Application::handleCallAccepted(common::PhoneNumber toPhoneNumber)
{
    logger.logInfo("Application::handleCallAccepted: ", toPhoneNumber);
    context.state->handleCallAccepted(toPhoneNumber);
}

}
