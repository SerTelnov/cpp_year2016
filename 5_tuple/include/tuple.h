//
// Created by Sergey on 22.11.2017.
//

#ifndef CPP_COURSE_TUPLE_H
#define CPP_COURSE_TUPLE_H

#include <tuple>
#include "../../2_promise_future/include/future.h"
#include <thread>
#include <utility>

template <typename T>
struct Impl {
    using inner_t = T;
};

template <typename T>
struct Impl<Future<T>> {
    using inner_t = T;
};

template <typename T>
struct Impl<Future<T> &> {
    using inner_t = typename Impl<T>::inner_t;
};

template <typename T>
struct Impl<Future<Future<T>>> {
    using inner_t = typename Impl<Future<T>>::inner_t;
};

template <typename T>
T Get_Impl(T &x) {
    return x;
}

template <typename T>
inline T Get_Impl(Future<T> &x) {
    return x.Get();
}

template <size_t... Is, typename... Ts>
auto Get(std::index_sequence<Is...>, std::tuple<Ts...> &t) {
    return std::make_tuple(Get_Impl(std::get<Is>(t))...);
}

template <size_t N, typename... Ts>
auto Get(std::tuple<Ts...> &t) {
    return Get(std::make_index_sequence<N>{}, t);
}

template <typename... Ts>
Future<typename std::tuple<typename Impl<Ts>::inner_t...>>
Flatten(std::tuple<Ts...> &tuple) {

    Promise<typename std::tuple<typename Impl<Ts>::inner_t...>> promise;

    std::thread values_setter([&tuple](Promise<typename std::tuple<typename Impl<Ts>::inner_t...>> promise1) {

        promise1.Set(Get<sizeof...(Ts)>(tuple));

    }, std::move(promise));

    values_setter.detach();

    return promise.GetFuture();
}

#endif //CPP_COURSE_TUPLE_H
