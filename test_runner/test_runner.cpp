#include <iostream>
#include "ThreadPool.h"

int main()
{
	ThreadPool threadPool;
	std::vector<ExperimentTask*> tasks;
	int budget = 10000000;
	for(int i = 1; i < 9; i++) {
		tasks.push_back(new ExperimentTask(i, true, true,
			[&](Problem* problem, Pyramid* pyramid) { return problem->getFitnessFunctionEvaluations() >= budget;  }));
	}
	threadPool.queueWork(tasks);

	bool finished = false;
	while(!finished) {
		std::unique_lock<std::mutex> g(threadPool.finishedMutex);
		threadPool.finishedConditionVariable.wait(g, [&] {
			return threadPool.finished;
			});
		finished = true;
	}
}
