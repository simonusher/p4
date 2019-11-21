#pragma once
#include "Pyramid.h"
#include <random>
#include <chrono>
#include "StopCondition.h"

namespace chrono = std::chrono;
using timePoint = std::chrono::steady_clock::time_point;
using stdClock = std::chrono::steady_clock;

struct BestSolutionFoundData {
	std::vector<int>* newBestPhenotype;
	std::vector<double>* newBestGenotype;
	double newSolutionFitness;
	int fitnessFunctionEvaluationsPassedWhenFound;
	long long elapsedTimeInSeconds;
};

class P3Optimizer {
public:
	
	static P3Optimizer* createOptimizerWithTimeConstraint(Problem* problem, const std::function<void(BestSolutionFoundData*)>
	                                                      & onNewBestSolutionFound, unsigned long long executionTimeInSeconds);
	static P3Optimizer* createOptimizerWithFfeConstraint(Problem* problem, const std::function<void(BestSolutionFoundData*)>& onNewBestSolutionFound, int ffeThreshold);
	void runIteration();
	bool finished();
	~P3Optimizer();
	BestSolutionFoundData* getLastFoundBestData();
private:
	P3Optimizer(Problem& problem, std::function<void(BestSolutionFoundData*)> onNewBestSolutionFound);
	void initializeTimeStopCondition(unsigned long long executionTimeInSeconds);
	void initializeFfeStopCondition(int fitnessFunctionEvaluationsThreshold);
	void updateBest(Solution* solution);
	BestSolutionFoundData getSolutionData(Solution* solution);
	long getElapsedTimeInSeconds();

	BestSolutionFoundData lastBestSolutionData;
	Problem& problem;
	std::mt19937* randomGenerator;
	Pyramid* pyramid;
	LocalOptimizer* localOptimizer;
	RandomKeyDecoder* decoder;
	RandomKeyEncoder* encoder;
	SolutionFactory* solutionFactory;
	PopulationFactory* populationFactory;
	SolutionMixer* solutionMixer;
	std::function<void(BestSolutionFoundData*)> onNewBestSolutionFound;
	StopCondition* stopCondtion;
	constexpr static double DEFAULT_RESCALING_PROBABILITY = 0.1;
	timePoint startTime;
};
