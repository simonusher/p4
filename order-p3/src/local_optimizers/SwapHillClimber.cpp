#include "../../include/order-p3/local_optimizers/SwapHillClimber.h"

SwapHillClimber::SwapHillClimber(Problem* problem, RandomKeyEncoder* encoder) : HillClimber(problem), encoder(encoder){
	initialize();
}

void SwapHillClimber::hillClimb(Solution& solution) {
	resetHelperFields();
	initializeSolutionData(solution);
	optimize(solution);
}

void SwapHillClimber::resetHelperFields() {
	improvementMadeLastIteration = true;
	anyImprovementMade = false;
	triedIndexPairs.clear();
}

void SwapHillClimber::initializeSolutionData(const Solution& solution) {
	currentPhenotype = solution.getPhenotype();
	currentFitness = solution.getFitness();
}

void SwapHillClimber::optimize(Solution& solution) {
	while (improvementMadeLastIteration) {
		runOptimizationIteration();
	}

	if (anyImprovementMade) {
		saveImprovedSolution(solution);
	}
}

void SwapHillClimber::saveImprovedSolution(Solution& solution) {
	solution.setPhenotype(currentPhenotype, *encoder);
	solution.evaluate(*problem);
}

void SwapHillClimber::runOptimizationIteration() {
	improvementMadeLastIteration = false;
	std::shuffle(possibleIndexPairs.begin(), possibleIndexPairs.end(), randomGenerator);
	trySwappingPossiblePairs();
}

void SwapHillClimber::trySwappingPossiblePairs() {
	for (std::pair<int, int>& indexPair : possibleIndexPairs) {
		std::swap(currentPhenotype[indexPair.first], currentPhenotype[indexPair.second]);
		double newFitness = problem->evaluate(currentPhenotype);
		if (currentFitness < newFitness) {
			noteImprovement(newFitness);
		}
		else {
			std::swap(currentPhenotype[indexPair.first], currentPhenotype[indexPair.second]);
		}
		triedIndexPairs.insert(indexPair);
	}
}

void SwapHillClimber::noteImprovement(double newFitness) {
	currentFitness = newFitness;
	improvementMadeLastIteration = true;
	anyImprovementMade = true;
	triedIndexPairs.clear();
}


void SwapHillClimber::initialize() {
	generateAllIndexPairs();
	initializeRandomGenerator();
	resetHelperFields();
}

void SwapHillClimber::generateAllIndexPairs() {
	const size_t numberOfGenes = problem->getProblemSize();
	possibleIndexPairs.clear();
	for (size_t i = 0; i < numberOfGenes - 1; i++) {
		for (size_t j = i + 1; j < numberOfGenes; j++) {
			possibleIndexPairs.emplace_back(i, j);
		}
	}
}

void SwapHillClimber::initializeRandomGenerator() {
	std::random_device randomDevice;
	randomGenerator.seed(randomDevice());
}