//
// Created by Sergey on 11.9.2017
//

#pragma once

#include <functional>
#include <cstdint>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

template <typename T, typename F>
void execute_map_function(T begin, T end, F const &f) {
    while (begin != end) {
        f(*begin);
        ++begin;
    }
}

class thread_pool {
public:
    typedef std::function<void()> task_t;

    thread_pool(size_t);
    ~thread_pool();

    void execute(task_t const & task);

    template <typename T, typename F>
    void Parallel(T begin, T end, F const & func) {
        size_t pack_size = std::distance(begin, end) / (_workers.size() + 1);
        for (size_t i = 0; i != _workers.size(); ++i) {
            T curr_end = begin;
            for (size_t j = 0; j != pack_size; ++j) {
                ++curr_end;
            }
            execute([begin, curr_end, &func] {
                execute_map_function(begin, curr_end, func);
            });
            begin = curr_end;
        }
        execute_map_function(begin, end, func);
    }

private:
    void action_fn();

private:
    std::vector<std::thread> _workers;
    std::condition_variable _cond_variable;
    std::mutex _mutex;
    std::atomic_bool _running;

    std::queue<task_t> _tasks;
};
