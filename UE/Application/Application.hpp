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
    void handleSmsReceived(common::PhoneNumber from, std::string &text) override;
    void handleCallRequest(common::PhoneNumber callingPhoneNumber) override;
    void handleCallRequestAccept() override;
    void handleCallRequestReject() override;
    void handleUnknownRecipient(common::PhoneNumber callingPhoneNumber) override;
    void handleCallAccepted() override;
    void handleCallFailure(std::string &&) override;

    // IUserEventsHandler interface
    void handleSendCallRequest(common::PhoneNumber to) override;
    void handleCallRequestResignation() override;

    void handleSmsSend(Sms &sms) override;
    void markSmsAsFailed() override;
private:
    Context context;
    common::PrefixedLogger logger;

};

}
