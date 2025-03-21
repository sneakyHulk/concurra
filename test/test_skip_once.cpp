#include <chrono>
#include <iostream>
#include <thread>

#include "Processor.h"
#include "Pusher.h"
#include "Runner.h"

using namespace std::chrono_literals;

class Push1 : public Pusher<int> {
	int i = 0;

   public:
	Push1() { std::cout << "Push1()" << std::endl; }
	int push() final {
		std::cout << ++i << " push()" << std::endl;
		return i;
	}
	int push_once() override {
		std::cout << ++i << " push_once()" << std::endl;
		return i;
	}
};

class Process1 : public Processor<int, int> {
   public:
	Process1() { std::cout << "Process1()" << std::endl; }

	int process_once(int const& data) override {
		return process(data);
	}
	int process(int const& data) override {
		if (stop_token.stop_requested()) {
			return data + 1;
		}

		std::cout << "still waiting" << std::endl;
		throw std::logic_error("still waiting");
	}
};

class Run1 : public RunnerSynchronous<int> {
   public:
	Run1() { std::cout << "Run1()" << std::endl; }

	void run_once(int const& data) override { std::cout << " run_once(" << data << ")" << std::endl; }
	void run(int const& data) override { std::cout << " run(" << data << ")" << std::endl; }
};

int main() {
	Run1 run1;

	Push1 push1;
	Process1 process1;

	push1.asynchronously_connect(process1);
	process1.synchronously_connect(run1);

	auto t1 = push1();
	auto t2 = process1();

	std::this_thread::sleep_for(100ms);
}