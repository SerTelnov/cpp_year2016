//
// Created by Sergey on 11.10.2017
//

#pragma once

#include <type_traits>
#include "../../2_promise_future/include/future.h"

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
Future<typename Impl<T>::inner_t> Flatten(Future<Future<T>> const & that) {
    Promise<typename Impl<T>::inner_t> promise;
    return promise.GetFuture();
}

template<template<typename, typename...> class C, typename T>
Future<C<T>> Flatten(C<Future<T>> const & that) {
    C<T> data(that.size());
    auto curr = data.begin();
    for (auto item = that.begin(); item != that.end(); ++item) {
        *curr = item->Get();
        ++curr;
    }
    Promise<C<T>> promise;
    promise.Set(data);
    return promise.GetFuture();
}

//template<template<typename, typename...> class C, typename T, typename... Targs>
//typename std::enable_if<&C<Future<T>, Targs...>::begin != nullptr, Future<C<T, Targs...>>>::type Flatten(C<Future<T>, Targs...> const & that) {
//    C<T> data(that.size());
//    auto curr = data.begin();
//    for (auto item = that.begin(); item != that.end(); ++item) {
//        *curr = item->Get();
//        ++curr;
//    }
//    Promise<C<T>> promise;
//    promise.Set(data);
//    return promise.GetFuture();
//}
