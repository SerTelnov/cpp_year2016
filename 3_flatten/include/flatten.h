//
// Created by eugene on 11.10.2017
//

#pragma once


#include "../../2_promise_future/include/future.h"

template<typename T>
Future<T> Flatten(Future<T> const & that) {
    return that;
}

template<typename T>
Future<T> Flatten(Future<Future<T>> const & that) {
    return Flatten(that.Get());
}

template<template<typename, typename...> class C, typename T>
Future<C<T>> Flatten(C<Future<T>> const & that) {
    Future<C<T>> result;
    return result;
}
