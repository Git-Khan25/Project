// ============================================================
//  Timer.cpp
// ============================================================
#include "Timer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

Timer::Timer(int seconds)
    : totalSeconds(seconds), active(false), secondsLeft(seconds) {}

Timer::~Timer() { stop(); }

// Background thread: ticks every second, updates console line
void Timer::runDisplay() {
    while (active && secondsLeft > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!active) break;
        --secondsLeft;

        int left = secondsLeft.load();
        std::string col = (left <= 5) ? Utils::Color::RED
                                      : Utils::Color::YELLOW;
        std::cout << "\r  " << col << Utils::Color::BOLD
                  << "  Time left: " << std::setw(2) << left << "s   "
                  << Utils::Color::RESET << std::flush;
    }
    if (active && secondsLeft <= 0) {
        std::cout << "\r  "
                  << Utils::Color::BG_RED << Utils::Color::BOLD
                  << " TIME'S UP!                    "
                  << Utils::Color::RESET << "\n\n" << std::flush;
        active = false;
    }
}

void Timer::start() {
    secondsLeft = totalSeconds;
    active      = true;
    std::cout << "  " << Utils::Color::YELLOW << Utils::Color::BOLD
              << "  Time left: " << std::setw(2) << totalSeconds << "s   "
              << Utils::Color::RESET << std::flush;
    displayThread = std::thread(&Timer::runDisplay, this);
}

void Timer::stop() {
    active = false;
    if (displayThread.joinable())
        displayThread.join();
}

bool Timer::isExpired()       const { return secondsLeft <= 0; }
int  Timer::getSecondsLeft()  const { return secondsLeft;      }
int  Timer::getTotalSeconds() const { return totalSeconds;     }
