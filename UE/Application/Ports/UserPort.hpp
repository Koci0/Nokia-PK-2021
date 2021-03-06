#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "ISmsDb.hpp"

namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber, ISmsDb &db);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showCallRequest(common::PhoneNumber callingPhoneNumber) override;
    void resetButtons() override;
    void setupIncomingCallButtons(std::function<void()>, std::function<void()>) override;
    void showTalking(std::string& text) override;
    void setupTalkingButtons(std::function<void()>) override;
    void setupCallReceiver() override;
    void showShortInfo(std::string &&, InternalMethod = &IUserPort::showConnected) override;
    void callRequestResignation() override;

    void showSmsNew() override;
    void showSmsNotNew() override;
    void showSmsList() override;
    void showSms(int id) override;
    void showSmsCompose() override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    ISmsDb &db;
};

}
