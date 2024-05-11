// Copyright 2024 Stone-Engine

#include "Utils/DispatchQueue.hpp"

namespace Stone {

void DispatchQueue::async(Task task) {
	std::unique_lock<std::mutex> lock(_mutex);
	_tasks.push(task);
	if (_tasks.size() == 1) {
		_condition.notify_one();
	}
}

void DispatchQueue::sync(Task task) {
	std::unique_lock<std::mutex> lock(_mutex);
	_tasks.push(task);
	_condition.wait(lock, [this] { return _tasks.empty(); });
}

void DispatchQueue::execute() {
	std::unique_lock<std::mutex> lock(_mutex);
	_running = true;
	while (!_tasks.empty()) {
		Task task = _tasks.front();
		_tasks.pop();
		task();
	}
	_running = false;
}

void DispatchQueue::run() {
	_running = true;
	while (_running) {
		Task task;
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_condition.wait(lock, [this] { return !_tasks.empty(); });
			task = _tasks.front();
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
