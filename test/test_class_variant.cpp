#include <chrono>

#include "ProcessorClass.h"
#include "PusherClass.h"
#include "RunnerClass.h"
#include "RunnerSynchronousClass.h"

using namespace std::chrono_literals;

class TEST {
	int i;

   public:
	TEST(int i) : i(i) {}

	int push() { return ++i; }
};

class PRINTER {
   public:
	void run(int const& i) { std::cout << i << std::endl; }
};

class ADD {
   public:
	ADD() {}
	int process(int const& i) { return i + 1; }
};

int main() {
	PusherClass<TEST> pusher(2);
	ProcessorClass<ADD> adder;
	RunnerClass<PRINTER> printer;
	RunnerSynchronousClass<PRINTER> printer2;

	pusher.asynchronously_connect(printer);
	pusher.asynchronously_connect(adder);
	adder.synchronously_connect(printer2);

	auto array_thread = pusher();
	auto adder_thread = adder();
	auto printer_thread = printer();

	std::this_thread::sleep_for(1s);
}