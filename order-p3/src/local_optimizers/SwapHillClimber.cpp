#include "../../include/order-p3/local_optimizers/SwapHillClimber.h"
#include <iostream>

SwapHillClimber::SwapHillClimber(Problem& problem, std::mt19937& randomGenerator) :
	LocalOptimizer(problem),
	randomGenerator(randomGenerator)
{
	initialize();
}

SwapHillClimber::~SwapHillClimber() {
	for (std::pair<int, int>* possibleIndexPair : possibleIndexPairs) {
		delete possibleIndexPair;
	}
}

void SwapHillClimber::initialize() {
	generateAllIndexPairs();
	initializeRandomGenerator();
	resetHelperFields();
}

void SwapHillClimber::optimizeLocally(Solution& solution) {
	resetHelperFields();
	initializeSolutionData(solution);
	hillClimb(solution);
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
	solution.recalculateGenotype();
	solution.recalculatePhenotype();
	solution.setFitness(currentFitness);
}

void SwapHillClimber::runOptimizationIteration() {
	improvementMadeLastIteration = false;
	std::shuffle(possibleIndexPairs.begin(), possibleIndexPairs.end(), randomGenerator);
	trySwappingPossiblePairs();
}

void SwapHillClimber::trySwappingPossiblePairs() {
	for (int i = 0; i < possibleIndexPairs.size(); i++) {
		std::pair<int, int>* indexPair = possibleIndexPairs.at(i);
		if(triedIndexPairs.count(indexPair) == 0) {
			std::swap((*solutionPhenotypePtr)[(*indexPair).first], (*solutionPhenotypePtr)[(*indexPair).second]);
			double newFitness = problem.evaluate(*solutionPhenotypePtr);
			if (currentFitness < newFitness) {
				noteImprovement(newFitness);
			}
			else {
				std::swap((*solutionPhenotypePtr)[(*indexPair).first], (*solutionPhenotypePtr)[(*indexPair).second]);
			}
			triedIndexPairs.insert(indexPair);
		}
		
	}
}

void SwapHillClimber::noteImprovement(double newFitness) {
	currentFitness = newFitness;
	improvementMadeLastIteration = true;
	anyImprovementMade = true;
	triedIndexPairs.clear();
}

void SwapHillClimber::generateAllIndexPairs() {
	const size_t numberOfGenes = problem.getProblemSize();
	possibleIndexPairs.clear();
	for (size_t i = 0; i < numberOfGenes - 1; i++) {
		for (size_t j = i + 1; j < numberOfGenes; j++) {
			possibleIndexPairs.emplace_back(new std::pair<int, int>(i, j));
		}
	}
}

void SwapHillClimber::initializeRandomGenerator() {
	std::random_device randomDevice;
	randomGenerator.seed(randomDevice());
}