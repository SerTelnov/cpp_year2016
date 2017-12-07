#include <assert.h>
#include "thread_pool.h"

thread_pool::thread_pool(size_t counter_threads)
        : _workers(counter_threads),
          _tasks(),
          _running(true)
{
    assert(counter_threads != 0);

    for (std::thread & worker : _workers) {
        worker = std::thread(&thread_pool::action_fn, this);
    }
}

thread_pool::~thread_pool() {
    _running = false;
    _cond_variable.notify_all();

    for (std::thread & worker : _workers) {
        worker.join();
    }
}

void thread_pool::execute(std::function<void()> const & new_task) {
    std::unique_lock<std::mutex> lock(_mutex);
    _tasks.push(new_task);
    _cond_variable.notify_one();
}

void thread_pool::action_fn() {
    while (true) {
        std::function<void()> curr_fn;
        {
            std::unique_lock<std::mutex> lock(_mutex);

            while (_tasks.empty() && _running) {
                _cond_variable.wait(lock);
            }

            if (!_running)
                break;

            curr_fn = _tasks.front();
            _tasks.pop();
        }
        try {
            curr_fn();
        }
        catch (...) {}
    }
}

