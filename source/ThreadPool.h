#pragma once
#include "QueueFreeLock.h"
#include "ChainFunctor.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <condition_variable>
#include <vector>
#include <future>
#include <atomic>

using namespace std::chrono_literals;

namespace plh {
class PoolExecutor {
public:
    PoolExecutor(int n) {
        std::cout << "[Pool] Start thread pool!" << std::endl;
        for (int i = 0; i < n; ++i) {
            workers.push_back(std::thread(runner));
        }
    }

    ~PoolExecutor() {}
    PoolExecutor(const PoolExecutor&) = delete;
    PoolExecutor(PoolExecutor&&) = delete;

private:
    void runner() {

        std::unique_lock<std::mutex> lk(_mu);

        while(!_stop.load()) {

        }
    }

private:
    std::mutex _mu;
    std::vector<std::thread> workers;
    std::atomic<bool> _stop = false;
    queue_free_lock<std::function<void()>> _tasks;
};
}
