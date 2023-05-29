#pragma once

#include <iostream>
#include <functional>

namespace plh {

template<typename F>
class Callable {
public:

    explicit Callable(F&& handler) : _fn{new F( std::forward<F>(handler))} {}
    ~Callable() {delete _fn;}

    template<typename ...ARGS>
    decltype(auto) execute(ARGS&&... args) {
        auto ret = std::invoke(std::forward<F>(*_fn), std::forward<ARGS>(args)...);
        return ret;
    }
private:
    F* _fn = nullptr;
};

}
