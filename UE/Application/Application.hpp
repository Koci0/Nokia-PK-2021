#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"
#include "ISmsDb.hpp"

namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer,
                ISmsDb& db);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSms(common::PhoneNumber from, std::string &text) override;
    void handleSmsUnknownRecipient() override;
    void handleCallRequest(common::PhoneNumber callingPhoneNumber) override;
    void handleCallRequestAccept() override;
    void handleCallRequestReject() override;
    void handleCallUnknownRecipient(common::PhoneNumber callingPhoneNumber) override;
    void handleCallAccepted(common::PhoneNumber from) override;
    void handleCallFailure(std::string &&) override;
    void handleCallTalk(std::string& text) override;

    // IUserEventsHandler interface
    void handleSendCallRequest(common::PhoneNumber to) override;
    void handleCallRequestResignation() override;

    void handleSendSms(Sms &sms) override;
    void handleSendCallTalk(std::string& text) override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
