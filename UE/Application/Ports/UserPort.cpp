#include "UserPort.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
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
    menu.addSelectionListItem("Call", "");

    gui.setAcceptCallback([&](){
        if (menu.getCurrentItemIndex().second == 0) {
                    showSmsCompose();
        } else if (menu.getCurrentItemIndex().second == 1) {
            showSmsList();
        } else if (menu.getCurrentItemIndex().second == 2) {
            setupCallReceiver();
        }
    });
}

void UserPort::setupCallReceiver()
{
    logger.logInfo("setup call receiver");
    auto& mode = gui.setDialMode();
    gui.setAcceptCallback([&]{
        logger.logInfo("to: ", mode.getPhoneNumber());
        showShortInfo("Calling...", &IUserPort::callRequestResignation);
        this->handler->handleSendCallRequest(mode.getPhoneNumber());
    });
}

void UserPort::showShortInfo(std::string &&message, InternalMethod onRejectFunction)
{
    // TODO Add timeout hiding message after few second even where the button is not pressed.
    logger.logDebug("showShortInfo - message:", message);
    auto& mode = gui.setAlertMode();
    mode.setText(std::move(message));
    gui.setRejectCallback([this, onRejectFunction]{
        onRejectFunction(this);
    });
}

void UserPort::callRequestResignation()
{
    logger.logInfo("user resignation");
    this->handler->handleCallRequestResignation();
}

void UserPort::showSmsList() {

    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    std::vector<Sms> ListSms = db.getAll();
    if(ListSms.empty()) {
        menu.addSelectionListItem("No SMS in DB", "");
    } else {
        for(auto sms : ListSms) {
            if(sms.from == phoneNumber) {
                menu.addSelectionListItem(sms.isFailed?"Failed: " + to_string(sms.to):"Send to: " + to_string(sms.to), sms.text);
            } else {
            menu.addSelectionListItem("From: " + to_string(sms.from), sms.text);
            }
        }
        gui.setAcceptCallback([&](){
            showSms(menu.getCurrentItemIndex().second);
        });
    }
    gui.setRejectCallback([&](){
        showConnected();
    });
}

void UserPort::showSms(int id) {
    IUeGui::ITextMode& menu = gui.setViewTextMode();
    Sms* sms = db.getOne(id);
    menu.setText(sms->text);
    sms->read=true;
    bool readAll = true;
    for(auto sms : db.getAll()) {
        if(sms.read == false){
            readAll = false;
            break;
        }
    }
    if (readAll == true) {
        showSmsNotNew();
    }
    gui.setRejectCallback([&](){
        showSmsList();
    });
}

void UserPort::showSmsNew(){
    gui.showNewSms();
}

void UserPort::showSmsNotNew(){
    gui.showNotNewSms();
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

void UserPort::setupTalkingButtons(std::function<void()> dropButtonCallback) {
    logger.logInfo("UserPort::setupTalkingButtons");
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(dropButtonCallback);
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

void UserPort::showSmsCompose() {
    IUeGui::ISmsComposeMode& smsGui = gui.setSmsComposeMode();
    smsGui.clearSmsText();
    gui.setRejectCallback([&](){
        showConnected();
    });
    gui.setAcceptCallback([&](){
        Sms sms{phoneNumber, smsGui.getSmsText()};
        sms.to = smsGui.getPhoneNumber();
        if(sms.from == sms.to)
            return;

        db.addOne(sms);
        handler->handleSmsSend(sms);
        showConnected();
    });
}

}
