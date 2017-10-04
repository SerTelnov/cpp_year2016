#pragma once

#include "thread_pool.h"
#include <iostream>
#include <list>

class thread_pool_tests {
public:
	thread_pool_tests(size_t);
	~thread_pool_tests();

	void run_tests();

private:
	void say_error(const char *);
	void tester_fn();

private:
	thread_pool tester;
	size_t counter_tests;
	std::mutex locker;
};
