//
// Created by Sergey on 08.11.2017.
//
#ifdef _GTEST

#include "map.h"
#include <gtest/gtest.h>

int f(int const value) {
    return value * value;
}

TEST(map, map_simple_Test) {
    Promise<int> promise;
    Future<int> future = promise.GetFuture();
    promise.Set(2);
    Future<int> result = Map(future, &f);

    ASSERT_EQ(result.Get(), 4);
}

#endif // _GTEST