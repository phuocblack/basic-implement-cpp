#pragma once

#include <unordered_map>

#include "ThreadPool.h"

namespace plh {
using MapEvent =
    std::unordered_map<std::string, std::unordered_map<int, Executor>>;
using sID = int;
using sEventId = std::string;

class EventObserverIF {
 public:
  EventObserverIF() = default;
  ~EventObserverIF() = default;

  virtual sID registerEvent(const sEventId& eventID, Executor&&) = 0;
  virtual void unregisterEvent(const sEventId& eventID, sID callbackID) = 0;
  virtual void signaling(const sEventId& eventID) = 0;
};

class EventObserver : public EventObserverIF {
 public:
  EventObserver(std::shared_ptr<PoolExecutorIF> com) {
    if (com)
      _com = com;
    else
      _com = std::make_shared<PoolExecutor>();
    if (!_com->isRunning()) {  // make sure threads pool are started
      _com->startPool(1);
    }
  }

  ~EventObserver() { _umap.clear(); }

  sID registerEvent(const sEventId& eventID, Executor&& f) override {
    auto nId = -1;
    try {
      auto& mEvent = _umap[eventID];
      do {
        nId = _fID.fetch_add(1, std::memory_order_relaxed);
        if (mEvent.find(nId) == mEvent.end()) {
          mEvent[nId] = std::move(f);
          break;
        }
      } while (true);
    } catch (const std::exception& e) {
      // logging
      return -1;
    }

    return nId;
  }

  void unregisterEvent(const sEventId& eventID, sID callbackID) override {
    try {
      if (_umap.find(eventID) != _umap.end()) {
        auto& mEvent = _umap[eventID];
        mEvent.erase(callbackID);
      }
    } catch (const std::exception& e) {
      // logging
    }
  }

  void signaling(const sEventId& eventID) override {
    try {
      if (_umap.find(eventID) != _umap.end()) {
        const auto& mEvent = _umap[eventID];
        for (auto& [sid, callback] : mEvent) {
          if (!_com->isRunning())
            throw std::runtime_error("Thread pool is stopped!");
          _com->executeTask(callback);
        }
      }
    } catch (const std::exception& e) {
      // logging
    }
  }

 private:
  std::shared_ptr<PoolExecutorIF> _com = nullptr;
  MapEvent _umap;
  std::atomic<int> _fID = 0;
};
}  // namespace plh
