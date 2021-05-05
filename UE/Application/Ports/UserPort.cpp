#include "UserPort.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
}

void UserPort::showCallRequest(common::PhoneNumber fromPhoneNumber)
{
    logger.logInfo("UserPort::showCalLRequest");
    IUeGui::ITextMode& alertMode = gui.setAlertMode();
    alertMode.setText("Incoming call from: " + to_string(fromPhoneNumber));
}

void UserPort::resetButtons()
{
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
}

void UserPort::setupIncomingCallButtons(std::function<void()> acceptButtonCallback, std::function<void()> rejectButtonCallback)
{
    logger.logInfo("UserPort::setupIncomingCallButtons");
    gui.setAcceptCallback(acceptButtonCallback);
    gui.setRejectCallback(rejectButtonCallback);
}

void UserPort::showTalking()
{
    logger.logInfo("UserPort::showTalking");
    gui.setCallMode();
}

void UserPort::showPeerUserDisconnected()
{
    logger.logInfo("UserPort::showPeerUserDisconnected");
    this->showAlert("Peer User was disconnected from BTS");
}

void UserPort::showAlert(std::string message)
{
    logger.logInfo("UserPort::showAlert: ", message);
    IUeGui::ITextMode &alertMode = gui.setAlertMode();
    alertMode.setText(message);
}

}
