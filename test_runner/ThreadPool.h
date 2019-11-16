#pragma once
#include <queue>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include "ExperimentTask.h"

class ThreadPool {
public:
	ThreadPool() : done(false) {
		auto numberOfThreads = std::thread::hardware_concurrency();
		if (numberOfThreads == 0) {
			numberOfThreads = 1;
		}

		for (unsigned i = 0; i < numberOfThreads; ++i) {
			threads.emplace_back(&ThreadPool::doWork, this);
		}
		finished = false;
	}

	~ThreadPool() {
		done = true;

		workQueueConditionVariable.notify_all();
		for (auto& thread : threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

	void queueWork(ExperimentTask* task) {
		std::lock_guard<std::mutex> g(workQueueMutex);
		workQueue.push_back(task);
		workQueueConditionVariable.notify_one();
	}

	void queueWork(std::vector<ExperimentTask*> tasks) {
		workQueue.insert(workQueue.end(), tasks.begin(), tasks.end());
		workQueueConditionVariable.notify_all();
	}

	std::mutex finishedMutex;
	std::condition_variable_any finishedConditionVariable;
	bool finished;
private:
	std::condition_variable_any workQueueConditionVariable;
	std::mutex workQueueMutex;

	std::vector<std::thread> threads;
	std::deque<ExperimentTask*> workQueue;
	bool done;
	void doWork() {
		while (!done) {
			ExperimentTask* task;
			{
				std::unique_lock<std::mutex> g(workQueueMutex);
				workQueueConditionVariable.wait(g, [&] {
					return !workQueue.empty() || done;
					});

				task = workQueue.front();
				workQueue.pop_front();
				if(workQueue.empty()) {
					std::unique_lock<std::mutex> g2(finishedMutex);
					finishedConditionVariable.notify_all();
					finished = true;
				}
			}
			task->execute();
		}
	}
};