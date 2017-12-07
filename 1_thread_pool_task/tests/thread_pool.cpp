//
// Created by eugene on 14.9.2017
//

#ifdef _GTEST

#include "thread_pool.h"
#include <gtest/gtest.h>

TEST(thread_pool, one_thread_one_task) {
    std::atomic<int> counter(0);

    {
        thread_pool pool(1);

        pool.execute([&counter]{ counter = 1; });
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ASSERT_EQ(counter, 1);
}

TEST(thread_pool, ten_threads) {
    std::atomic<int> counter(0);

    int32_t const cnt_threads = 10;
    {
        thread_pool pool(cnt_threads);

        for (int32_t i = 0; i < cnt_threads; ++i) {
            pool.execute([&counter]{ counter += 1; });
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ASSERT_EQ(counter, cnt_threads);
}

TEST(thread_pool, one_thread_several_tasks) {
    std::atomic<int> counter(0);

    int32_t const cnt_threads = 1;
    int32_t const cnt_tasks = 1;
    {
        thread_pool pool(cnt_threads);

        for (int32_t i = 0; i < cnt_tasks; ++i) {
            pool.execute([&counter]{ counter += 1; });
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ASSERT_EQ(counter, cnt_tasks);
}

TEST(thread_pool, parallel_simple_tast) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6};
    std::vector<int> values = arr;

    std::function<void(int&)> square_fun = [](int& x) {
        x = x * x;
    };

    thread_pool pool(2);
    pool.Parallel(arr.begin(), arr.end(), square_fun);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (size_t i = 0; i != arr.size(); ++i) {
        ASSERT_EQ(values[i] * values[i], arr[i]);
    }
}

TEST(thread_pool, parallel_random_test) {
    std::vector<int> arr(100000);
    for (auto & value: arr) {
        value = rand();
    }

    std::vector<int> values = arr;

    thread_pool pool(10);
    pool.Parallel(arr.begin(), arr.end(), [](int& x) {
        x = x + 10;
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (size_t i = 0; i != arr.size(); ++i) {
        ASSERT_EQ(values[i] + 10, arr[i]);
    }
}

#endif // _GTEST
