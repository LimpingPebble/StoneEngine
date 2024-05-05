#include "Utils/DispatchQueue.hpp"

#include <gtest/gtest.h>
#include <thread>

using namespace Stone;

TEST(DispatchQueueTest, QueueAsync) {
	DispatchQueue queue;

	std::atomic<int> count{0};

	// Submit tasks to the queue asynchronously
	for (int i = 0; i < 10; ++i) {
		queue.async(0, [&count] { ++count; });
	}

	EXPECT_EQ(count.load(), 0);

	// Run the queue
	queue.execute();

	// Verify that all tasks have been executed
	EXPECT_EQ(count.load(), 10);
}

TEST(DispatchQueueTest, QueueSync) {
	DispatchQueue queue;

	// Start a secondary thread to run the queue
	std::thread thread([&queue] { queue.run(); });

	std::atomic<int> count{0};

	// Submit tasks to the queue synchronously
	for (int i = 0; i < 10; ++i) {
		queue.async([&count] { ++count; });
	}

	// Stop the secondary thread
	queue.async(-1, [&queue] { queue.stop(); });

	++count;
	++count;

	thread.join();

	// Verify that all tasks have been executed
	EXPECT_EQ(count.load(), 12);

	// Verify that the secondary thread has stopped
	EXPECT_FALSE(thread.joinable());
}
