#include <chrono>

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

int main() {
	PusherClass<TEST> pusher(2);
	RunnerClass<PRINTER> printer;
	RunnerSynchronousClass<PRINTER> printer2;

	pusher.asynchronously_connect(printer);
	pusher.synchronously_connect(printer2);

	auto array_thread = pusher();
	auto printer_thread = printer();

	std::this_thread::sleep_for(1s);
}