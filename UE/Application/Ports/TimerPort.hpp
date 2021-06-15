#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <future>

namespace ue
{

class TimerPort : public ITimerPort
{
public:
    explicit TimerPort(common::ILogger& logger);

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void stopTimer() override;

private:
    void timerThread(Duration duration);
    bool running;
    std::future<void> timer;
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
};

}
