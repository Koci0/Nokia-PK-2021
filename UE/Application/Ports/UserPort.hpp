#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showCallRequest(common::PhoneNumber fromPhoneNumber) override;
    void resetButtons() override;
    void setupIncomingCallButtons(std::function<void()>, std::function<void()>) override;
    void showTalking() override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    common::PhoneNumber foreignPhoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}
