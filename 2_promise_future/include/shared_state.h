//
// Created by Sergey on 02.10.2017.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

template <typename> class Promise;
template <typename> class Future;

class basic_shared_state {
public:
    basic_shared_state()
            : _is_ready{false},
              _was_error{false},
              _has_promise{false}
    { }

    void notify_one() { cv.notify_one(); }

    std::exception_ptr get_error() { return _error; }

    void set_error(const std::exception_ptr & e) {
        _error = e;
        _was_error = true;
    }

    std::atomic_bool _is_ready;
    std::atomic_bool _was_error;
    std::atomic_bool _has_promise;
    std::exception_ptr _error;

    std::mutex locker;
    std::condition_variable cv;
};

template<typename T>
class shared_state : public basic_shared_state {
private:
    shared_state()
            : basic_shared_state()
    { }

    friend class Promise<T>;
    friend class Future<T>;
private:
    T value;
};

template<>
class shared_state<void> : public basic_shared_state {
private:
    shared_state()
            : basic_shared_state()
    { }

    friend class Promise<void>;
    friend class Future<void>;
};

template<typename T>
class shared_state<T &> : public basic_shared_state {
private:
    shared_state() noexcept
            : basic_shared_state()
    { }

    friend class Promise<T &>;
    friend class Future<T &>;

private:
    T * value;
};
