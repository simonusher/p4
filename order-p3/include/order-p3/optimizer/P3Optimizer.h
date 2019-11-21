#pragma once
#include "Pyramid.h"
#include <random>

class P3Optimizer {
public:
	struct BestSolutionFoundData {
		const std::vector<int>& newBestPhenotype;
		const std::vector<double>& newBestGenotype;
		const double newSolutionFitness;
		const int fitnessFunctionEvaluationsPassedWhenFound;
	};
	~P3Optimizer();
private:
	P3Optimizer(Problem& problem, std::function<void(const BestSolutionFoundData&)> onNewBestSolutionFound);
	void updateBest(Solution* solution);
	
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
	constexpr static double DEFAULT_RESCALING_PROBABILITY = 0.1;
};
