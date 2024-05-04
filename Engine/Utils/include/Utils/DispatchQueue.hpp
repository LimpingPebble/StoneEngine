// Copyright 2024 Stone-Engine

#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace Stone {

/**
 * @brief A class that represents a dispatch queue for executing tasks asynchronously or synchronously.
 */
class DispatchQueue {
public:
	using Task = std::function<void()>;

	DispatchQueue() = default;
	DispatchQueue(const DispatchQueue &) = delete;

	virtual ~DispatchQueue() = default;

	DispatchQueue &operator=(const DispatchQueue &) = delete;

	/**
	 * @brief Enqueues a task to be executed asynchronously.
	 * @param task The task to be executed.
	 */
	void async(Task task);

	/**
	 * @brief Enqueues a task to be executed synchronously.
	 * @param task The task to be executed.
	 */
	void sync(Task task);

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

private:
	std::queue<Task> _tasks;                ///< The queue of tasks.
	std::mutex _mutex;                      ///< The mutex for thread safety.
	std::condition_variable _condition;     ///< The condition variable for task synchronization.
	bool _running = false;                  ///< Flag indicating if the dispatch queue is running.
};

} // namespace Stone
