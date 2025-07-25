#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <queue>
#include <functional>

#if THREAD_DEBUG
#include <iostream>
#include "timing.hpp"
#endif

struct TaskQueue
{
    std::mutex mutex_;
    std::atomic<int> numTasks_ = 0;
    std::queue<std::function<void()>> tasks_;

    void pushTask(std::function<void()> task)
    {
        std::lock_guard<std::mutex> lock_guard{mutex_};
        tasks_.push(task);
        numTasks_++;
    }

     std::function<void()> getTask ()
    {
        std::lock_guard<std::mutex> lock_guard{mutex_};
        if (tasks_.empty()) return nullptr;
        std::function<void()> task = tasks_.front();
        tasks_.pop();
        return task;
    }

    void taskComplete()
    {
        numTasks_--;
    }

    void waitForComplete()
    {
        while (numTasks_ > 0)
        {
            std::this_thread::yield();
        }
    }
};

struct TaskWorker
{
    TaskQueue* pQueue_;
    std::thread thread_;
    int id_;
    bool isRunning_ = true;

    long long startTime = 0;
    long long endTime = 0;

    TaskWorker() = default;

    TaskWorker(TaskQueue* pQueue, int id):
        pQueue_(pQueue),
        thread_(std::thread([this](){run();})),
        id_(id)
    {
    }

    ~TaskWorker()
    {
    }

    void run()
    {
        std::function<void()> task = nullptr;
        while(isRunning_)
        {
            task = pQueue_->getTask();
            if (task == nullptr)
            {
                std::this_thread::yield();
            }
            else
            {
#if THREAD_DEBUG
                if (startTime == 0) startTime = getTime();
                task();
                endTime = getTime();
#else
                task();
#endif
                pQueue_->taskComplete();
            }
        }
    }

    void stop()
    {
        isRunning_ = false;
        thread_.join();
    }

#if THREAD_DEBUG
    void printTime()
    {
        std::cout << id_ << " [" << startTime << " , " << endTime << "] " << endTime - startTime << "\n";
        startTime = 0;
    }
#endif
};

class ThreadPool
{
    public:
        ThreadPool(int numThreads);
        ~ThreadPool();

        void dispatch(std::function<void()> func);
        void joinAll();

    private:
        int numWorkers_ = 0;
        TaskQueue taskQueue_;
        TaskWorker** workers_;
};