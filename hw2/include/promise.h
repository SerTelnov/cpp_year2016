//
// Created by Sergey on 02.10.2017.
//

#ifndef CPP_COURSE_CLION_PROMISE_H
#define CPP_COURSE_CLION_PROMISE_H

#include "Future.h"
#include "shared_state.h"

template<typename T>
class Promise {
public:
    Promise()
            : _state(new shared_state<T>()),
              _future_exist(false)
    {
        _state->_has_promise = true;
    }

    Promise(Promise && that) noexcept
        : _state(that._state)
    { }

    Promise & operator= (Promise && that) noexcept {
        *this = std::move(that);
        return *this;
    }

    Promise(Promise const &) = delete;
    Promise & operator=(Promise const &) = delete;

    ~Promise() {
        _state->_has_promise = false;
    }

    void Set(const T &);
    void Set(const T &&);

    void SetException(const std::exception_ptr &);

    Future<T> GetFuture();

private:
    void check_state();

private:
    std::shared_ptr<shared_state<T>> _state;
    std::atomic_bool _future_exist;
};

template<typename T>
inline void Promise<T>::Set(const T & value) {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_is_ready) {
        throw std::logic_error("recall Set");
    }
    _state->value = value;

    _state->_is_ready = true;
    _state->notify_one();
}

template<typename T>
inline void Promise<T>::Set(const T && value) {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_is_ready) {
        throw std::logic_error("recall Set");
    }
    _state->value = std::move(value);

    _state->_is_ready = true;
    _state->notify_one();
}

template<typename T>
inline void Promise<T>::SetException(const std::exception_ptr & e) {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_was_error) {
        throw std::logic_error("recall Set exception");
    }
    _state->set_error(e);
}

template<typename T>
inline Future<T> Promise<T>::GetFuture() {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_future_exist) {
        throw std::logic_error("re-creation Future");
    }
    _future_exist = true;
    return Future<T>(_state);
}

template<typename T>
inline void Promise<T>::check_state() {
    if (!_state) {
        throw std::runtime_error("Promise is invalidate");
    }
}

template<>
class Promise<void> {
public:
    Promise()
            : _state(new shared_state<void>()),
              _future_exist(false)
    { }

    Promise(Promise && that) noexcept
            : _state(that._state)
    { }

    Promise & operator= (Promise && that) noexcept {
        *this = std::move(that);
        return *this;
    }

    Promise(Promise &) = delete;
    Promise & operator=(Promise &) = delete;

    ~Promise()
    { }

    void Set();
    void SetException(const std::exception_ptr &);
    Future<void> GetFuture();

private:
    void check_state() {
        if (!_state) {
            throw std::runtime_error("Promise is invalidate");
        }
    }

private:
    std::shared_ptr<shared_state<void>> _state;
    std::atomic_bool _future_exist;
};

inline void Promise<void>::Set() {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_is_ready) {
        throw std::logic_error("recall Set");
    }

    _state->_is_ready = true;
    _state->notify_one();
}

inline Future<void> Promise<void>::GetFuture() {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_future_exist) {
        throw std::logic_error("re-creation Future");
    }
    _future_exist = true;
    return Future<void> (_state);
}

inline void Promise<void>::SetException(const std::exception_ptr &e) {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_was_error) {
        throw std::logic_error("recall Set exception");
    }
    _state->set_error(e);
}

template<typename T>
class Promise<T &> {
public:
    Promise()
            : _state(new shared_state<T &>()),
              _future_exist(false)
    {
        _state->_has_promise = true;
    }

    Promise(Promise && that) noexcept
            : _state(that._state)
    { }

    Promise & operator=(Promise && that) noexcept {
        *this = std::move(that);
        return *this;
    }

    Promise(Promise &) = delete;
    Promise & operator=(Promise &) = delete;

    ~Promise() {
        _state->_has_promise = false;
    }

    void Set(T &);

    void SetException(const std::exception_ptr &);

    Future<T &> GetFuture();

private:
    void check_state();

private:
    std::shared_ptr<shared_state<T &>> _state;
    std::atomic_bool _future_exist;
};

template <typename T>
void Promise<T &>::Set(T & value) {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_is_ready) {
        throw std::logic_error("recall Set");
    }
    _state->value = & value;

    _state->_is_ready = true;
    _state->notify_one();
}

template<typename T>
inline void Promise<T &>::SetException(const std::exception_ptr &e) {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_was_error) {
        throw std::logic_error("recall Set exception");
    }
    _state->set_error(e);
}

template <typename T>
Future<T &> Promise<T &>::GetFuture() {
    check_state();
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_future_exist) {
        throw std::logic_error("re-creation Future");
    }
    _future_exist = true;
    return Future<T &>(_state);
}

template<typename T>
inline void Promise<T &>::check_state() {
    if (!_state) {
        throw std::runtime_error("Promise is invalidate");
    }
}

#endif //CPP_COURSE_CLION_PROMISE_H
