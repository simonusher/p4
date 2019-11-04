#include "../../../include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"



bool RandomRescalingOptimalMixer::mixWithRescaling() {
	rescaleClusterGeneValues();
	bool anyGeneChanged = swapWithRescaled();
	if (anyGeneChanged) {
		handleFitnessChanges();
	}
	return anyGeneChanged;
}

bool RandomRescalingOptimalMixer::swapWithRescaled() {
	bool solutionsDiffer = false;
	for (int i = 0; i < rescaledClusterValues.size(); i++) {
		solutionsDiffer |= ((*destinationGenotype)[cluster->at(i)] != rescaledClusterValues[i]);
		std::swap((*destinationGenotype)[cluster->at(i)], rescaledClusterValues[i]);
	}
	return solutionsDiffer;
}

void RandomRescalingOptimalMixer::handleFitnessChanges() {
	double oldFitness = destinationSolution->getFitness();
	std::vector<int> oldPhenotype(destinationSolution->getPhenotype());
	double newFitness = destinationSolution->evaluate(*problem);

	if (newFitness < oldFitness) {
		swapWithRescaled();
		destinationSolution->setPhenotype(oldPhenotype);
		destinationSolution->setFitness(oldFitness);
	}
	else {
		destinationSolution->reEncode();
	}
}

RandomRescalingOptimalMixer::RandomRescalingOptimalMixer(Problem* problem, double rescalingProbability,
	double lowerGeneValueBound, double upperGeneValueBound, std::mt19937& randomGenerator)
	: OptimalMixer(problem),
	rescalingProbability(rescalingProbability),
	lowerGeneValueBound(lowerGeneValueBound),
	upperGeneValueBound(upperGeneValueBound),
	numberOfIntervals(problem->getProblemSize()),
	randomGenerator(randomGenerator) {
	initializeRescalingIntervals();
}

bool RandomRescalingOptimalMixer::mixGenotypes() {
	bool shouldRescale = shouldRescaleDistribution(randomGenerator);
	if (shouldRescale) {
		return mixWithRescaling();
	}
	else {
		return OptimalMixer::mixGenotypes();
	}
}

void RandomRescalingOptimalMixer::initializeRescalingIntervals() {
	intervalSize = (upperGeneValueBound - lowerGeneValueBound) / static_cast<double>(numberOfIntervals);
	double lowerBound = lowerGeneValueBound;
	for (int i = 0; i < numberOfIntervals; i++) {
		rescalingIntervalsLowerBounds.emplace_back(lowerBound);
		lowerBound += intervalSize;
	}
	intervalIndexDistribution = std::uniform_int_distribution<int>(0, rescalingIntervalsLowerBounds.size() - 1);
	shouldRescaleDistribution = std::bernoulli_distribution(rescalingProbability);
}

void RandomRescalingOptimalMixer::rescaleClusterGeneValues() {
	double intervalLowerBound = rescalingIntervalsLowerBounds[intervalIndexDistribution(randomGenerator)];
	rescaledClusterValues = std::vector<double>(cluster->size());
	for (int i = 0; i < cluster->size(); i++) {
		rescaledClusterValues[i] = sourceGenotype->at(cluster->at(i)) * intervalSize + intervalLowerBound;
	}
}