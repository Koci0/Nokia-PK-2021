#include "UserPort.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "Sms.hpp"
#include "ISmsDb.hpp"
#include <string>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber, ISmsDb &db)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber),
      db(db)

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
    gui.setAcceptCallback([&](){
        if(menu.getCurrentItemIndex().second == 1){
            showSmsList();
        };
    });
}

void UserPort::showSmsList() {

    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    std::vector<Sms> ListSms = db.getAll();
    if(ListSms.empty()) {
        menu.addSelectionListItem("No SMS in DB", "");
    } else {
        for(auto sms : ListSms) {
            menu.addSelectionListItem("From: " + to_string(sms.from), sms.text);
        }
    }

    gui.setAcceptCallback([&](){
        showSms(menu.getCurrentItemIndex().second);;
    });
    gui.setRejectCallback([&](){
        showConnected();
    });
}

void UserPort::showSms(int id) {
    IUeGui::ITextMode& menu = gui.setViewTextMode();
    Sms* sms = db.getOne(id);
    menu.setText(sms->text);
    sms->read=true;
    gui.setRejectCallback([&](){
        showSmsList();
    });
}

void UserPort::showSmsReceived(){
    gui.showNewSms();
}

void UserPort::showCallRequest(common::PhoneNumber callingPhoneNumber)
{
    logger.logInfo("UserPort::showCalLRequest");
    IUeGui::ITextMode& alertMode = gui.setAlertMode();
    alertMode.setText("Incoming call from: " + to_string(callingPhoneNumber));
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
    showShortInfo("Peer User was disconnected from BTS");
}

void UserPort::showShortInfo(std::string &&message)
{
}

}
