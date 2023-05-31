#pragma once

#include "ThreadPool.h"

namespace plh {
class TimerExecutor {
    static singleShotAfter(unsigned long timeout = 0, TimerExecutor&& exec, PoolExecutorIF* component = nullptr) {
        std::chrono::milliseconds dur(duration);
        TimerExecutor::singleShotAfter(dur, exec, component);
    }

    static singleShotAfter(std::chrono::seconds timeout = 0s, TimerExecutor&& exec, PoolExecutorIF* component = nullptr) {

    }
};
}
