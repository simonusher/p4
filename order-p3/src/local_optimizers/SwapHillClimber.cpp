#include "../../include/order-p3/local_optimizers/SwapHillClimber.h"

SwapHillClimber::SwapHillClimber(Problem* problem, RandomKeyEncoder* encoder) : LocalOptimizer(problem), encoder(encoder){
	initialize();
}

void SwapHillClimber::initialize() {
	generateAllIndexPairs();
	initializeRandomGenerator();
	resetHelperFields();
}

void SwapHillClimber::optimize(Solution& solution) {
	resetHelperFields();
	initializeSolutionData(solution);
	optimize(solution);
}

void SwapHillClimber::resetHelperFields() {
	improvementMadeLastIteration = true;
	anyImprovementMade = false;
	triedIndexPairs.clear();
}

void SwapHillClimber::initializeSolutionData(Solution& solution) {
	solutionPhenotypePtr = solution.getPhenotypePtr();
	currentFitness = solution.getFitness();
}

void SwapHillClimber::hillClimb(Solution& solution) {
	while (improvementMadeLastIteration) {
		runOptimizationIteration();
	}

	if (anyImprovementMade) {
		saveImprovedSolution(solution);
	}
}

void SwapHillClimber::saveImprovedSolution(Solution& solution) {
	solution.recalculateGenotype(*encoder);
	solution.setFitness(currentFitness);
}

void SwapHillClimber::runOptimizationIteration() {
	improvementMadeLastIteration = false;
	std::shuffle(possibleIndexPairs.begin(), possibleIndexPairs.end(), randomGenerator);
	trySwappingPossiblePairs();
}

void SwapHillClimber::trySwappingPossiblePairs() {
	for (std::pair<int, int>& indexPair : possibleIndexPairs) {
		std::swap((*solutionPhenotypePtr)[indexPair.first], (*solutionPhenotypePtr)[indexPair.second]);
		double newFitness = problem->evaluate(*solutionPhenotypePtr);
		if (currentFitness < newFitness) {
			noteImprovement(newFitness);
		}
		else {
			std::swap((*solutionPhenotypePtr)[indexPair.first], (*solutionPhenotypePtr)[indexPair.second]);
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