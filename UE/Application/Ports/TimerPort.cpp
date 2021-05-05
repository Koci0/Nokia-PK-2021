#include "TimerPort.hpp"

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stoped");
    handler = nullptr;
}

void TimerPort::startTimer(Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    this->running = true;
    this->timer = std::async(&TimerPort::timerThread, this, duration);
}

void TimerPort::stopTimer()
{
    logger.logDebug("Stop timer");
    this->running = false;
    this->timer.get();
}

void TimerPort::timerThread(Duration duration)
{
    using namespace std::chrono_literals;
    for(int i = duration / 100ms; i > -1; i--)
    {
        std::this_thread::sleep_for(100ms);
        if (!this->running)
            return;
    }
    logger.logError("Timeout");
    handler->handleTimeout();
}

}
