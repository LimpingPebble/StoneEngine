// Copyright 2024 Stone-Engine

#include "Utils/DispatchQueue.hpp"

#include <cassert>

namespace Stone {

DispatchQueue &DispatchQueue::main() {
	static DispatchQueue mainQueue;
	return mainQueue;
}

DispatchQueue::DispatchQueue() : _running(false), _threadId(std::this_thread::get_id()) {
}

void DispatchQueue::execute() {
	std::unique_lock<std::mutex> lock(_mutex);
	_running = true;
	while (!_tasks.empty()) {
		Task task = _tasks.top();
		_tasks.pop();
		task.task();
	}
	_running = false;
}

void DispatchQueue::run() {
	_running = true;
	while (_running) {
		TaskType task;
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_condition.wait(lock, [this] { return !_tasks.empty(); });
			task = _tasks.top().task;
			_tasks.pop();
		}
		task();
	}
}

void DispatchQueue::stop() {
	_running = false;
	_condition.notify_all();
}

} // namespace Stone
