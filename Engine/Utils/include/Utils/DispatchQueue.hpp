// Copyright 2024 Stone-Engine

#pragma once

#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace Stone {

/**
 * @brief A class that represents a dispatch queue for executing tasks asynchronously or synchronously.
 */
class DispatchQueue {
public:
	static DispatchQueue &main();

	DispatchQueue();
	DispatchQueue(const DispatchQueue &) = delete;

	virtual ~DispatchQueue() = default;

	DispatchQueue &operator=(const DispatchQueue &) = delete;

	/**
	 * @brief Enqueues a task to be executed asynchronously.
	 * @param task The task to be executed.
	 */
	template <typename... Args>
	void async(Args... arg) {
		std::unique_lock<std::mutex> lock(_mutex);
		_tasks.emplace(arg...);
		if (_tasks.size() == 1) {
			_condition.notify_one();
		}
	}

	/**
	 * @brief Enqueues a task to be executed synchronously.
	 * @param task The task to be executed.
	 */
	template <typename... Args>
	void sync(Args... arg) {
		assert(std::this_thread::get_id() != _threadId);
		std::unique_lock<std::mutex> lock(_mutex);
		_tasks.emplace(arg...);
		_condition.wait(lock, [this] { return _tasks.empty(); });
	}

	/**
	 * @brief Executes the tasks in the queue.
	 */
	void execute();

	/**
	 * @brief Runs the dispatch queue in a loop.
	 */
	void run();

	/**
	 * @brief Stops the dispatch queue loop.
	 */
	void stop();

	using TaskType = std::function<void()>;

private:
	struct Task {
		int priority;
		TaskType task;

		explicit Task(TaskType task) : priority(0), task(std::move(task)) {
		}

		Task(int priority, TaskType task) : priority(priority), task(std::move(task)) {
		}

		bool operator<(const Task &other) const {
			return priority < other.priority;
		}
	};

	std::priority_queue<Task> _tasks;	///< The queue of tasks.
	std::mutex _mutex;					///< The mutex for thread safety.
	std::condition_variable _condition; ///< The condition variable for task synchronization.
	bool _running = false;				///< Flag indicating if the dispatch queue is running.
	std::thread::id _threadId;			///< The ID of the thread that created the dispatch queue.
};

} // namespace Stone
