#pragma once

#include "QueueFreeLock.h"

#include <functional>
#include <iostream>

namespace plh {

template <typename F>
class Callable {
 public:
  explicit Callable(F&& executor) : _fn{std::move(executor)} {}
  explicit Callable(const F& executor) : _fn{executor} {}
  ~Callable() {}

  template <typename... ARGS>
  decltype(auto) execute(ARGS&&... args) {
    auto executor = std::bind(_fn, std::forward<ARGS>(args)...);
    return executor();
  }

 private:
  F _fn;
};

template<typename T>
class ExecutorChainIF {
  public:
   ExecutorChainIF() = default;
   ~ExecutorChainIF() = default;

   virtual ExecutorChainIF& then(Callable<T>) {
    return *this;
   }
};
}  // namespace plh
