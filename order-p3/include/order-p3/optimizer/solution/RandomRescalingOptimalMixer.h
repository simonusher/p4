#pragma once
#include "OptimalMixer.h"

class RandomRescalingOptimalMixer: public OptimalMixer {

public:
	RandomRescalingOptimalMixer(Problem* problem, double rescalingProbability, double lowerGeneValueBound,
		double upperGeneValueBound, std::mt19937& randomGenerator);

protected:
	bool mixGenotypes() override;

	bool mixWithRescaling();
	bool swapWithRescaled();
	void handleFitnessChanges();
	void initializeRescalingIntervals();
	void rescaleClusterGeneValues();
	double rescalingProbability;
	double lowerGeneValueBound;
	double upperGeneValueBound;
	double intervalSize;
	int numberOfIntervals;
	std::vector<double> rescaledClusterValues;
	std::vector<double> rescalingIntervalsLowerBounds;
	std::uniform_int_distribution<int> intervalIndexDistribution;
	std::bernoulli_distribution shouldRescaleDistribution;
	std::mt19937& randomGenerator;
};