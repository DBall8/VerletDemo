#include "timing.hpp"
#include <chrono>
#include <iostream>

static bool timingStarted = false;
static std::chrono::high_resolution_clock::time_point timingStart;

static std::chrono::high_resolution_clock::time_point startupTime;

void initTiming()
{
    startupTime = std::chrono::high_resolution_clock::now();
}

void startTiming()
{
    if (timingStarted) std::cout << "Timing overlap!!\n";
    timingStart = std::chrono::high_resolution_clock::now();
    timingStarted = true;
}

long long endTiming(char* label)
{
    if (!timingStarted) std::cout << "Timing unstarted!!\n";
    std::chrono::high_resolution_clock::time_point timingEnd = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds result = std::chrono::duration_cast<std::chrono::microseconds>(timingEnd - timingStart);

    if (label != nullptr) std::cout << label << " ";
    std::cout << result.count() << std::endl;
    timingStarted = false;
    return result.count();
}

long long getTime()
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds result = std::chrono::duration_cast<std::chrono::microseconds>(now - startupTime);
    return result.count();
}