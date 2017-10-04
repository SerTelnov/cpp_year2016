//
// Created by Sergey on 02.10.2017.
//

#ifndef CPP_COURSE_CLION_SHARED_STATE_H
#define CPP_COURSE_CLION_SHARED_STATE_H

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

    bool is_ready() { return _is_ready; }
    void set_ready() { _is_ready = true; }

    std::exception_ptr get_error() { return _error; }
    bool was_error() { return _was_error; }

    void set_error(const std::exception_ptr & e) {
        _error = e;
        _was_error = true;
    }

    void set_has_promise() { _has_promise = true; }
    void destroy_promise() { _has_promise = false; }
    bool has_promise() { return _has_promise; }

protected:
    std::atomic_bool _is_ready;
    std::atomic_bool _was_error;
    std::atomic_bool _has_promise;
    std::exception_ptr _error;

public:
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

#endif //CPP_COURSE_CLION_SHARED_STATE_H
