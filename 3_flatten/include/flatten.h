//
// Created by Sergey on 11.10.2017
//

#pragma once

#include "../../2_promise_future/include/future.h"
#include <thread>

template<typename T>
Future<T> Flatten(Future<T> const & that) {
    return that;
}

template <typename T>
struct Impl;

template <typename T>
struct Impl<Future<T>> {
    using inner_t = T;
};

template <typename T>
struct Impl<const Future<T> &> {
    using inner_t = typename Impl<T>::inner_t;
};

template <typename T>
struct Impl<Future<Future<T>>> {
    using inner_t = typename Impl<Future<T>>::inner_t;
};

template <typename T>
typename Impl<Future<T>>::inner_t FlattenImpl(Future<T> const & that) {
    return that.Get();
}

template <typename T>
typename Impl<Future<T>>::inner_t FlattenImpl(Future<Future<T>> const & that) {
    return FlattenImpl(that.Get());
}

template <typename T>
Future<typename Impl<Future<T>>::inner_t> Flatten(Future<Future<T>> const & that) {
    Promise<typename Impl<Future<T>>::inner_t> promise;

    std::thread value_getter = std::thread([&that](Promise<typename Impl<Future<T>>::inner_t> promise1) {
        typename Impl<Future<T>>::inner_t value = FlattenImpl(that);
        promise1.Set(value);
    }, std::move(promise));

    value_getter.detach();

    return promise.GetFuture();
}

template<template<typename, typename...> class C, typename T>
Future<C<T>> Flatten(C<Future<T>> const & that) {
    Promise<C<T>> promise;

    C<T> data(that.size());
    auto curr = data.begin();
    for (auto item = that.begin(); item != that.end(); ++item) {
        *curr = item->Get();
        ++curr;
    }
    promise.Set(data);

    return promise.GetFuture();
}
