#pragma once

#include <iostream>
#include <atomic>

namespace plh {
template <typename T>
class queue_free_lock {
private:
    struct node {
        std::shared_ptr<T> data = nullptr;
        node* _next = nullptr;
    };

    std::atomic<node*> _head;
    std::atomic<node*> _tail;

    node* pop_head() {
        node* const old_head = _head.load();
        if (old_head == _tail.load()) {
            return nullptr;
        }
        _head.store(old_head->_next);
        return old_head;
    }

public:
    queue_free_lock() : _head{new node}, _tail{_head.load()} {}

    ~queue_free_lock() {
        while (node* const old_head = _head.load()) {
            _head.store(old_head->_next);
            delete old_head;
        }
    }

    queue_free_lock(const queue_free_lock& ) = delete;
    queue_free_lock(queue_free_lock&& ) = delete;
    queue_free_lock& operator=(const queue_free_lock& ) = delete;

    std::shared_ptr<T> pop() {
        node* old_head = this->pop_head();
        if (!old_head) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(old_head->data);
        delete old_head;
        return res;
    }

    void push(const T& new_value) {
        std::shared_ptr<T> n_data(new T(new_value));
        node* p = new node;
        node* const old_tail = _tail.load();
        old_tail->data.swap(n_data);
        old_tail->_next = p;
        _tail.store(p);
    }

    bool isEmpty() {
        if (_head.load() == _tail.load()) {
            return true;
        }
        return false;
    }

    size_t size() {
        size_t s = 0;
        auto node_ptr = _head.load();
        if (node_ptr == _tail.load()) {
            return s;
        }

        while (node_ptr->data) {
            s++;
            node_ptr = node_ptr->_next;
        }
        return s;
    }
};
}
