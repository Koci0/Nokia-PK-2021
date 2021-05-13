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
                ISmsDb& smsDB);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleCallRequest(common::PhoneNumber callingPhoneNumber) override;
    void handleCallRequestAccept() override;
    void handleCallRequestReject() override;
    void handleUnknownRecipient(common::PhoneNumber callingPhoneNumber) override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
