#include "WorkerThread.h"

WorkerThread::WorkerThread(Problem* problem, unsigned long long executionTimeInSeconds): optimizer(nullptr) {
	optimizer = P3Optimizer::createOptimizerWithTimeConstraint(problem,
	                                                           [&](BestSolutionData* bestSolutionData) {
		                                                           emit newBestFound(bestSolutionData);
	                                                           }, executionTimeInSeconds,
	                                                           [&](const IterationData& iterationData) {
		                                                           emit iterationPassed(iterationData);
	                                                           });
}

WorkerThread::~WorkerThread() {
	delete optimizer;
}

void WorkerThread::run() {
	while (!isInterruptionRequested() && !optimizer->finished()) {
		optimizer->runIteration();
	}
	emit lastBestSolution(optimizer->getLastFoundBestData());
}
