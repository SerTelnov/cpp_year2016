//
// Created by eugene on 11.10.2017
//
#ifdef _GTEST

#include "flatten.h"
#include "../../2_promise_future/include/promise.h"
#include <thread>
#include <gtest/gtest.h>


TEST(flatten, flatten_simple_Test) {
    Future<int> first;
    Future<int> second = Flatten(first);
}

TEST(flatten, two_deep) {
    const int x = 10;
    Promise<int> promise1;
    promise1.Set(x);
    Future<int> future1 = promise1.GetFuture();

    Promise<Future<int>> promise2;
    promise2.Set(future1);
    Future<Future<int>> f = promise2.GetFuture();
    auto future = Flatten(f);
}

TEST(flatten, three_deep) {

    const int x = 10;

    Promise<int> promise1;
    promise1.Set(x);
    Future<int> future1 = promise1.GetFuture();

    Promise<Future<int>> promise2;
    promise2.Set(future1);
    Future<Future<int>> future2 = promise2.GetFuture();

    Promise<Future<Future<int>>> promise3;
    promise3.Set(future2);
    Future<Future<Future<int>>> future3 = promise3.GetFuture();

    Future<int> future = Flatten(future3);
}

TEST(flatten, collections) {
    std::vector<Promise<int>> promises(10);
    std::vector<Future<int>> futures(10);
    for (size_t i = 0; i != promises.size(); i++) {
        promises[i].Set(rand());
        futures[i] = promises[i].GetFuture();
    }
    Future<std::vector<int>> result = Flatten(futures);
}

#endif // _GTEST
