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

class thread_pool {
public:
    typedef std::function<void()> task_t;

    thread_pool(size_t);
    ~thread_pool();

    void execute(task_t const & task);

private:
    void action_fn();

private:
    std::vector<std::thread> _workers;
    std::condition_variable _cond_variable;
    std::mutex _mutex;
    std::atomic_bool _running;

    std::queue<task_t> _tasks;
};
