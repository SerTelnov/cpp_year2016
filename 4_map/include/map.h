//
// Created by Sergey on 08.11.2017.
//

#ifndef CPP_COURSE_MAP_H
#define CPP_COURSE_MAP_H

#include "../../2_promise_future/include/promise.h"

//template <typename T, typename F>
//using Type = std::result_of<F(T)>::type;

template <typename T, typename F>
Future<typename std::result_of<F(T)>::type> Map(Future<T> & init_future, const F & func) {

    Promise<typename std::result_of<F(T)>::type> promise;
    Future<typename std::result_of<F(T)>::type> future = promise.GetFuture();

    init_future.SetFunc([&func, &promise] (T const &value) {
        promise.Set(func(value));
    });

    return future;
}

#endif //CPP_COURSE_MAP_H
