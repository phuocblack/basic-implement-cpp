// Just implement std::shared_ptr-like for knowlegde
#pragma once

#include <iostream>

namespace plh {
template <class _cptr>
class cshared_ptr {
 public:
  cshared_ptr() : _ptr{nullptr}, _count{nullptr} {
    std::cout << "[Ctr] Init cptr null..." << std::endl;
  }

  cshared_ptr(_cptr x) : _ptr{new _cptr(x)}, _count{new int(1)} {
    std::cout << "[Ctr] Init cptr..." << std::endl;
    std::cout << "[Ctr] Count add = " << *_count << std::endl;
  }

  ~cshared_ptr() {
    if (--(*_count) == 0) {
      std::cout << "[Detr] Count sub = " << (*_count) << std::endl;
      delete _ptr, _count;
      _ptr = nullptr;
      _count = nullptr;
      std::cout << "[Detr] Delete ptr...\n";
    } else {
      std::cout << "[Detr] Count sub = " << (*_count) << std::endl;
    }
  }

  // copy assignment
  cshared_ptr& operator=(const cshared_ptr& ptr) {
    this->_ptr = ptr._ptr;
    _count = ptr._count;
    ++(*_count);
    std::cout << "[Assignment operator] Count add = " << *_count << std::endl;
    return *this;
  }

  _cptr& operator*() const noexcept { return *_ptr; }

  _cptr* operator->() const noexcept { return _ptr; }

  cshared_ptr(const cshared_ptr& ptr) {
    this->_ptr = ptr._ptr;
    _count = ptr._count;
    ++(*_count);
    std::cout << "[Copy ctr] Count add = " << *_count << std::endl;
  }

  _cptr* get_ptr() const noexcept { return _ptr; }

 public:
  _cptr* _ptr = nullptr;
  int* _count = nullptr;
};
}  // namespace plh
