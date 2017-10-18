//
// Created by Sergey on 02.10.2017.
//

#pragma once

#include <memory>
#include "shared_state.h"

template <typename> class Promise;

template<typename T>
class Future {
public:
    explicit Future()
            : _state(new shared_state<T>())
    { }

    Future(Future && that) noexcept
            : _state(that._state)
    { }

    Future & operator=(Future && that) noexcept {
        if (this != &that) {
            _state = that._state;
        }
        return *this;
    }

    Future(Future const & that)
            : _state(that._state)
    { }

    Future & operator=(Future const & that) {
        if (this != &that) {
            _state = that._state;
        }
        return *this;
    }

    ~Future() = default;

    T Get() const;
    void Wait() const;
    bool IsReady() const;

    friend class Promise<T>;
private:
    explicit Future(std::shared_ptr<shared_state<T>> state)
    : _state(state)
            { }
private:
    std::shared_ptr<shared_state<T>> _state;
};

template<typename T>
inline T Future<T>::Get() const {
    if (_state->_was_error) {
        throw _state->get_error();
    }
    if (!IsReady()) {
        if (!_state->_has_promise) {
            throw std::logic_error("Future is invalidate");
        }
        Wait();
    }
    return _state->value;
}

template<typename T>
inline void Future<T>::Wait() const {
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_is_ready)
        return;
    _state->cv.wait(lock);
    _state->_is_ready = true;
}

template<typename T>
inline bool Future<T>::IsReady() const {
    return _state->_is_ready;
}

template<>
class Future<void> {
public:
    Future(Future && that) noexcept
            : _state(that._state)
    { }
    Future & operator=(Future && that) noexcept {
        *this = std::move(that);
        return *this;
    }

    Future(Future const &) = delete;
    Future & operator=(Future const &) = delete;

    ~Future() = default;

    void Get() const {
        if (_state->_was_error) {
            throw _state->get_error();
        }
        if (!IsReady()) {
            if (!_state->_has_promise) {
                throw std::logic_error("Future is invalidate");
            }
            Wait();
        }
    }
    void Wait() const {
        std::unique_lock<std::mutex> lock(_state->locker);
        if (_state->_is_ready)
            return;
        _state->cv.wait(lock);
        _state->_is_ready = true;
    }
    bool IsReady() const {
        return _state->_is_ready;
    }

    friend class Promise<void>;
private:
    explicit Future(std::shared_ptr<shared_state<void>> state)
    : _state(state)
            { }
private:
    std::shared_ptr<shared_state<void>> _state;
};

template<typename T>
class Future<T &> {
public:

    Future(Future && that) noexcept
            : _state(that._state)
    { }
    Future & operator=(Future && that) noexcept {
        *this = std::move(that);
        return *this;
    }
    Future(Future const &) = delete;
    Future & operator=(Future const &) = delete;

    ~Future() = default;

    T & Get() const;
    void Wait() const;
    bool IsReady() const;

    friend class Promise<T &>;
private:
    explicit Future(std::shared_ptr<shared_state<T &>> state)
    : _state(state)
            { }
private:
    std::shared_ptr<shared_state<T &>> _state;
};

template<typename T>
inline T & Future<T &>::Get() const {
    if (_state->_was_error) {
        throw _state->get_error();
    }
    if (!IsReady()) {
        if (!_state->_has_promise) {
            throw std::logic_error("Future is invalidate");
        }
        Wait();
    }
    return *_state->value;
}

template<typename T>
inline void Future<T &>::Wait() const {
    std::unique_lock<std::mutex> lock(_state->locker);
    if (_state->_is_ready)
        return;
    _state->cv.wait(lock);
    _state->_is_ready = true;
}

template<typename T>
inline bool Future<T &>::IsReady() const {
    return _state->_is_ready;
}

