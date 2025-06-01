#include "thread_pool.hpp"

ThreadPool::ThreadPool(int numThreads)
{
    numWorkers_ = numThreads;
    workers_ = new TaskWorker*[numThreads];
    for (int i=0; i<numThreads; i++)
    {
        workers_[i] = new TaskWorker(&taskQueue_, i+1);
    }
}

ThreadPool::~ThreadPool()
{
    for (int i=0; i<numWorkers_; i++)
    {
        workers_[i]->stop();
        delete workers_[i];
    }
    delete[] workers_;
}

void ThreadPool::dispatch(std::function<void()> func)
{
    taskQueue_.pushTask(func);
}

void ThreadPool::joinAll()
{
#if THREAD_DEBUG
    long long allStart = getTime();
    taskQueue_.waitForComplete();
    long long allEnd = getTime();

    std::cout << "----- " << allEnd - allStart << "\n";
    for (int i=0; i<numWorkers_; i++)
    {
        workers_[i]->printTime();
    }
#else
    taskQueue_.waitForComplete();
#endif
}