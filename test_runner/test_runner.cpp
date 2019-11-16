#include <iostream>
#include "ThreadPool.h"
#include <fstream>
#include <sstream>
#include <filesystem>

void readTasksFromConfigFile(const std::string& fileName, std::vector<ExperimentTask*>& tasks) {
	std::ifstream fileStream(fileName);
	std::string line;
	while(std::getline(fileStream, line)) {
		std::stringstream lineStream(line);
		int flowshopIndex;
		bool useRescaling;
		bool useReencoding;
		int numberOfRuns;
		int budget;
		lineStream >> flowshopIndex >> useRescaling >> useReencoding >> numberOfRuns >> budget;
		tasks.push_back(new ExperimentTask(flowshopIndex, useRescaling, useReencoding, numberOfRuns, budget,
			"experiments/" + std::to_string(flowshopIndex) + "/"));
	}
}

void initializeDirectoryHierarchy(std::vector<ExperimentTask*>& tasks) {
	if(!std::filesystem::exists("experiments")) {
		std::filesystem::create_directory("experiments");
	}
	for (ExperimentTask* task : tasks) {
		std::string directoryPath = "experiments/" + std::to_string(task->getFlowshopIndex());
		if (!std::filesystem::exists(directoryPath)) {
			std::filesystem::create_directory(directoryPath);
		}
	}
}

int main()
{
	ThreadPool threadPool;
	std::vector<ExperimentTask*> tasks;
	readTasksFromConfigFile("experiments.txt", tasks);
	initializeDirectoryHierarchy(tasks);
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
