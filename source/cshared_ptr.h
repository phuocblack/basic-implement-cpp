#ifndef CSHARED_PTR_H
#define CSHARED_PTR_H

#include <iostream>

namespace plh {
template <class _cptr>
class cshared_ptr {
public:
    cshared_ptr(_cptr x) : _ptr{new _cptr(x)}, _count{new int(1)} {
        std::cout << "Init cptr..." << std::endl;
        std::cout << "[Ctr] Count add = " << *_count << std::endl;
    }

    ~cshared_ptr() {
        if (--(*_count) == 0) {
            delete _ptr, _count;
            std::cout << "Delete ptr...\n";
        } else {
            std::cout << "Count sub = " << *_count << std::endl;
        }
    }

    // copy assignment
    cshared_ptr& operator=(const cshared_ptr& ptr) {
        this->_ptr = ptr._ptr;
        _count = ptr._count;
        ++(*_count);
        std::cout << "[Assignment ope] Count add = " << *_count << std::endl;
    }


    cshared_ptr(const cshared_ptr& ptr) {
        this->_ptr = ptr._ptr;
        _count = ptr._count;
        ++(*_count);
        std::cout << "[Copy ctr] Count add = " << *_count << std::endl;
    }


    _cptr* get_ptr() const {
        return _ptr;
    }

public:
    _cptr* _ptr = nullptr;
    int* _count = nullptr;
};
}

#endif // CSHARED_PTR_H
