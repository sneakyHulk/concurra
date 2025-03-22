#include "StoppableThread.h"

void StoppableThread::request_stop() { stop_function(); }
StoppableThread::~StoppableThread() {
	request_stop();

	if (joinable()) join();
}
