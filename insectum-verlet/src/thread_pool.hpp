#pragma once
#include <thread>
#include <atomic>

class ThreadPool
{
    public:
        ThreadPool();
        ~ThreadPool();

        void dispatch(std::function<void()> func);
        void joinAll();

    private:
        int maxThreads_;
        int numThreads_;
        std::atomic<int> numThreadsRunning_;
};