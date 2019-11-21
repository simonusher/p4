#pragma once
#include "Pyramid.h"
#include <random>
#include <chrono>
#include "StopCondition.h"

namespace chrono = std::chrono;
using timePoint = std::chrono::steady_clock::time_point;
using stdClock = std::chrono::steady_clock;

class P3Optimizer {
public:
	struct BestSolutionFoundData {
		const std::vector<int>& newBestPhenotype;
		const std::vector<double>& newBestGenotype;
		const double newSolutionFitness;
		const int fitnessFunctionEvaluationsPassedWhenFound;
		const long long elapsedTimeInSeconds;
	};
	static P3Optimizer* createOptimizer(Problem* problem, std::function<void(const BestSolutionFoundData&)> onNewBestSolutionFound);
	void runForTime(unsigned long long timeInSeconds);
	void runForFitnessFunctionEvaluations(int fitnessFunctionEvaluationsThreshold);
	void run();
	void stop();
	~P3Optimizer();
private:
	P3Optimizer(Problem& problem, std::function<void(const BestSolutionFoundData&)> onNewBestSolutionFound);
	void updateBest(Solution* solution);
	long getElapsedTimeInSeconds();
	
	Problem& problem;
	std::mt19937* randomGenerator;
	Pyramid* pyramid;
	LocalOptimizer* localOptimizer;
	RandomKeyDecoder* decoder;
	RandomKeyEncoder* encoder;
	SolutionFactory* solutionFactory;
	PopulationFactory* populationFactory;
	SolutionMixer* solutionMixer;
	std::function<void(const BestSolutionFoundData&)> onNewBestSolutionFound;
	StopCondition* stopCondtion;
	bool interrupted;
	constexpr static double DEFAULT_RESCALING_PROBABILITY = 0.1;
	timePoint startTime;
};
