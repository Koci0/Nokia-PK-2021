#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/IDialMode.hpp"

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
    menu.addSelectionListItem("Call", "");

    gui.setAcceptCallback([&](){
        if(menu.getCurrentItemIndex().second == 2){
            setupCallReceiver();
        };
    });
}

void UserPort::setupCallReceiver()
{
    logger.logInfo("setup call receiver");
    auto& mode = gui.setDialMode();
    gui.setAcceptCallback([&](){
        logger.logInfo("to: ", mode.getPhoneNumber());
        this->handler->handleSendCallRequest(mode.getPhoneNumber());
    });
}

}
