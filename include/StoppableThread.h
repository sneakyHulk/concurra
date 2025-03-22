#pragma once

#include <thread>
#include <functional>

class StoppableThread : public std::thread {
	std::function<void()> stop_function;

   public:
	template <typename... ThreadArgs>
	explicit StoppableThread(std::function<void()>&& stop_function, ThreadArgs&&... thread_args) : stop_function(std::forward<decltype(stop_function)>(stop_function)), std::thread(std::forward<decltype(thread_args)>(thread_args)...) {}

	void request_stop();
	~StoppableThread();
};

