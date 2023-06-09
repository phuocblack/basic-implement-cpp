#pragma once
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include "QueueFreeLock.h"

using namespace std::chrono_literals;

namespace plh {
using Executor = std::function<void()>;
class PoolExecutorIF {
 public:
  PoolExecutorIF() = default;
  virtual ~PoolExecutorIF(){};

  PoolExecutorIF(const PoolExecutorIF&) = delete;
  PoolExecutorIF(PoolExecutorIF&&) = delete;
  PoolExecutorIF& operator=(const PoolExecutorIF&) = delete;
  PoolExecutorIF& operator=(PoolExecutorIF&&) = delete;

  virtual void startPool(size_t) = 0;
  virtual bool isRunning() const = 0;
  virtual void executeTask(Executor f) = 0;
};

class PoolExecutor : public PoolExecutorIF {
 public:
  PoolExecutor() {}

  ~PoolExecutor() { waitAndStop(); }

  void startPool(size_t n = 1) override {
    _nthreads = n;
    _stop.store(false);
    std::cout << "[Pool] Start thread pool!" << std::endl;
    for (int i = 0; i < _nthreads; ++i) {
      auto t = std::thread([this] { living(); });
      workers.push_back(std::move(t));
    }
  }

  bool isRunning() const override { return !_stop.load(); }

  void waitAndStop() {
    std::cout << "[Pool] End thread pool!" << std::endl;
    {
      std::unique_lock<std::mutex> lock(_mu);
      _stop.store(true);
    }
    _cv.notify_all();
    for (auto& thread : workers) {
      thread.join();
    }
  }

  void executeTask(Executor f) override { this->executeTasks(f); }

  template <typename F, typename... ARGS>
  std::future<std::result_of_t<F && (ARGS && ...)>> executeTasks(
      F&& callee, ARGS&&... args) {
    using return_type = std::result_of_t<F && (ARGS && ...)>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(callee), std::forward<ARGS>(args)...));
    auto ret = task->get_future();
    {
      std::unique_lock lk(_mu);
      if (_stop.load()) throw std::runtime_error("Thread pool has been stop!");

      _tasks.push([task]() -> void { (*task)(); });
    }
    _cv.notify_one();
    return ret;
  }

 private:
  void living() {
    while (true) {
      std::shared_ptr<std::function<void()>> exec;
      {
        std::unique_lock<std::mutex> lk(_mu);
        _cv.wait(lk, [this] { return _stop.load() || !_tasks.isEmpty(); });

        if (_stop.load() && _tasks.isEmpty()) return;
        exec = std::move(_tasks.pop());
      }
      std::cout << "[" << std::this_thread::get_id() << "]"
                << "Executing task..." << std::endl;
      if (exec) (*exec)();
      std::cout << "[" << std::this_thread::get_id() << "]"
                << "Executing task done" << std::endl;
    }
  }

 private:
  size_t _nthreads = 1;
  std::mutex _mu;
  std::vector<std::thread> workers;
  std::atomic<bool> _stop = true;
  std::condition_variable _cv;
  queue_free_lock<std::function<void()>> _tasks;
};
}  // namespace plh
