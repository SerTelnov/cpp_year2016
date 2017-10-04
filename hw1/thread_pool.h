#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <assert.h>

class thread_pool {
public:
	thread_pool();
	thread_pool(size_t);
	~thread_pool();

	void execute(std::function<void()> const &);

private:
	void action_fn();

private:

	std::vector<std::thread> _workers;
	std::queue<std::function<void()>> _tasks;
	std::atomic<bool> _running;
	std::mutex _locker;
	std::condition_variable _notify_workers;
};
