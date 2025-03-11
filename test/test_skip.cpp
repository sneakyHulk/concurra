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
		std::cout << data + 1 << " process_once.push(" << data << ")" << std::endl;
		return data + 1;
	}
	int process(int const& data) override {
		std::cout << data + 1 << " process.push(" << data << ")" << std::endl;

		static unsigned int counter = 0;
		if (++counter % 2) throw std::invalid_argument("bad argument");

		return data + 1;
	}
};

class Run1 : public Runner<int> {
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
	process1.asynchronously_connect(run1);

	auto t1 = push1();
	auto t2 = process1();
	auto t3 = run1();

	std::this_thread::sleep_for(100ms);
}