#pragma once
// ============================================================
//  Timer.h  –  Per-question countdown (std::chrono)
//
//  A background thread decrements the counter every second
//  and prints the live countdown on the same console line.
//  The main thread calls stop() once input arrives.
// ============================================================
#include <chrono>
#include <atomic>
#include <thread>

class Timer {
private:
    int               totalSeconds;
    std::atomic<bool> active;
    std::atomic<int>  secondsLeft;
    std::thread       displayThread;

    void runDisplay();   // runs in background thread

public:
    explicit Timer(int seconds = 15);
    ~Timer();

    void start();
    void stop();

    bool isExpired()       const;
    int  getSecondsLeft()  const;
    int  getTotalSeconds() const;
};
