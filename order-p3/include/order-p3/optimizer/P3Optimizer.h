#pragma once
#include "Pyramid.h"
#include <random>
#include <chrono>
#include "StopCondition.h"

namespace chrono = std::chrono;
using timePoint = std::chrono::steady_clock::time_point;
using stdClock = std::chrono::steady_clock;

struct FinalSolutionData {
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double solutionFitness;
	int fitnessFunctionEvaluationsPassedWhenFound;
	double elapsedTimeInSeconds;
	int iterationNumber;
};

struct IterationData {
	double bestFitness;
	int iterationNumber;
};

struct BestSolutionData {
	double solutionFitness;
	int fitnessFunctionEvaluationsPassedWhenFound;
	double elapsedTimeInSeconds;
	int iterationNumber;
};

class P3Optimizer {
public:
	
	static P3Optimizer* createOptimizerWithTimeConstraint(Problem* problem, const std::function<void(BestSolutionData*)>
	                                                      & onNewBestSolutionFound, unsigned long long executionTimeInSeconds,
														  const std::function<void(const IterationData&)>& onIterationPassed = nullptr);
	
	static P3Optimizer* createOptimizerWithFfeConstraint(Problem* problem, const std::function<void(BestSolutionData*)>& onNewBestSolutionFound, int ffeThreshold,
														 const std::function<void(const IterationData&)>& onIterationPassed = nullptr);
	void runIteration();
	bool finished();
	~P3Optimizer();
	FinalSolutionData getLastFoundBestData();
private:
	P3Optimizer(Problem& problem, std::function<void(BestSolutionData*)> onNewBestSolutionFound, 
		std::function<void(const IterationData&)> onIterationPassed = nullptr);
	void initializeTimeStopCondition(unsigned long long executionTimeInSeconds);
	void initializeFfeStopCondition(int fitnessFunctionEvaluationsThreshold);
	void updateBest(Solution* solution);
	BestSolutionData getSolutionData(Solution* solution);
	double getElapsedTimeInSeconds();

	int iterationsPassed;
	BestSolutionData lastBestSolutionData;
	Problem& problem;
	std::mt19937* randomGenerator;
	Pyramid* pyramid;
	LocalOptimizer* localOptimizer;
	RandomKeyDecoder* decoder;
	RandomKeyEncoder* encoder;
	SolutionFactory* solutionFactory;
	PopulationFactory* populationFactory;
	SolutionMixer* solutionMixer;
	std::function<void(BestSolutionData*)> onNewBestSolutionFound;
	std::function<void(const IterationData&)> onIterationPassed;
	StopCondition* stopCondtion;
	constexpr static double DEFAULT_RESCALING_PROBABILITY = 0.1;
	timePoint startTime;
};
