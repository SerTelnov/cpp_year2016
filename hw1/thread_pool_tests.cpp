#include "thread_pool_tests.h"

void thread_pool_tests::tester_fn() {
	std::unique_lock<std::mutex> lock(locker);
	std::cout << "start\n";
	lock.unlock();
	std::cout << "sleep 1 sec\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	lock.lock();
	std::cout << "done\n";
}

thread_pool_tests::thread_pool_tests(size_t counter_threads)
	: tester(counter_threads),
	counter_tests(5)
{
	std::cout << "threadpool test starts:\n";
	run_tests();
	std::cout << "wait 2 sec!\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "OK";
}

thread_pool_tests::~thread_pool_tests() = default;


void thread_pool_tests::run_tests() {
	try {
		for (size_t i = 0; i != counter_tests; ++i) {
			tester.execute(std::bind(&thread_pool_tests::tester_fn, this));
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "threadpool test passed\n";
	}
	catch (std::runtime_error e) {
		say_error(e.what());
	}
}

void thread_pool_tests::say_error(const char * message) {
	std::cout << "tests failed:\n";
	std::cout << message << std::endl;
}
