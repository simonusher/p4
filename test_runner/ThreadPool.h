#pragma once
#include <queue>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

// This class manages a thread pool that will process requests
class ThreadPool {
public:
	ThreadPool() : done(false) {
		// This returns the number of threads supported by the system. If the
		// function can't figure out this information, it returns 0. 0 is not good,
		// so we create at least 1
		auto numberOfThreads = std::thread::hardware_concurrency();
		if (numberOfThreads == 0) {
			numberOfThreads = 1;
		}

		for (unsigned i = 0; i < numberOfThreads; ++i) {
			// The threads will execute the private member `doWork`. Note that we need
			// to pass a reference to the function (namespaced with the class name) as
			// the first argument, and the current object as second argument
			threads.push_back(std::thread(&ThreadPool::doWork, this));
		}
	}

	// The destructor joins all the threads so the program can exit gracefully.
	// This will be executed if there is any exception (e.g. creating the threads)
	~ThreadPool() {
		// So threads know it's time to shut down
		done = true;

		// Wake up all the threads, so they can finish and be joined
		workQueueConditionVariable.notify_all();
		for (auto& thread : threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

	void queueWork(int fd, std::string& request) {
		// Grab the mutex
		std::lock_guard<std::mutex> g(workQueueMutex);

		// Push the request to the queue
		workQueue.push(std::pair<int, std::string>(fd, request));

		// Notify one thread that there are requests to process
		workQueueConditionVariable.notify_one();
	}

private:
	// This condition variable is used for the threads to wait until there is work
	// to do
	std::condition_variable_any workQueueConditionVariable;

	// We store the threads in a vector, so we can later stop them gracefully
	std::vector<std::thread> threads;

	// Mutex to protect workQueue
	std::mutex workQueueMutex;

	// Queue of requests waiting to be processed
	std::queue<std::pair<int, std::string>> workQueue;

	// This will be set to true when the thread pool is shutting down. This tells
	// the threads to stop looping and finish
	bool done;

	// Function used by the threads to grab work from the queue
	void doWork() {
		// Loop while the queue is not destructing
		while (!done) {
			std::pair<int, std::string> request;

			// Create a scope, so we don't lock the queue for longer than necessary
			{
				std::unique_lock<std::mutex> g(workQueueMutex);
				workQueueConditionVariable.wait(g, [&] {
					// Only wake up if there are elements in the queue or the program is
					// shutting down
					return !workQueue.empty() || done;
					});

				request = workQueue.front();
				workQueue.pop();
			}

			processRequest(request);
		}
	}

	void processRequest(const std::pair<int, std::string>) {
		// This function will process the request and send the response back
	}
};