//
// Created by Sergey on 22.11.2017.
//

#include <gtest/gtest.h>
#include "tuple.h"
#include "../../2_promise_future/include/promise.h"

#ifdef _GTEST

TEST(tuple, tuple_flatten) {
    int x = 2;
    Promise<int> pi;
    pi.Set(2);
    Future<int> fi = pi.GetFuture();

    Promise<double> pd;
    pd.Set(2.0);
    Future<double> fd = pd.GetFuture();

    static_assert(std::is_same<int, Impl<Future<int> &>::inner_t>::value);

    std::tuple<int, Future<int> &, Future<double> &> t(x, fi, fd);

    Future<std::tuple<int, int, double>> fut = Flatten(t);

    std::tuple<int, int, double> expected {2, 2, 2.0};

    fut.Wait();

    ASSERT_TRUE(fut.IsReady());

    ASSERT_EQ(fut.Get(), expected);
}

#endif // _GTEST