#include "thread_pool.h"
#include <iostream>

thread_pool::thread_pool()
	: thread_pool(2)
{ }

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
	_notify_workers.notify_all();

	for (std::thread & worker : _workers) {
		worker.join();
	}
}


void thread_pool::execute(std::function<void()> const & new_task) {
	std::unique_lock<std::mutex> lock(_locker);
	_tasks.push(new_task);
	_notify_workers.notify_one();
}

void thread_pool::action_fn() {
	while (true) {
		std::function<void()> curr_fn;
		{
			std::unique_lock<std::mutex> lock(_locker);

			while (_tasks.empty() && _running) {
				_notify_workers.wait(lock);
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
