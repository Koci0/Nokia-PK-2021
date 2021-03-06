#pragma once

#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"
#include <memory>
#include "ISmsDb.hpp"

namespace ue
{

struct Context
{
    common::ILogger& logger;
    IBtsPort& bts;
    IUserPort& user;
    ITimerPort& timer;
    ISmsDb& db;
    std::unique_ptr<IEventsHandler> state{};
    common::PhoneNumber callingPhoneNumber{};

    template <typename State, typename ...Arg>
    void setState(Arg&& ...arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};

}
