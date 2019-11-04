//
// Created by Szymon on 24.11.2018.
//

#include "../../include/order-p3/optimizer/Pyramid.h"

Pyramid::Pyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory, LocalOptimizer* localOptimizer, bool removeDuplicates)
	: LocalOptimizer(problem), problem(problem), solutionFactory(solutionFactory), populationFactory(populationFactory), localOptimizer(localOptimizer),
		removeDuplicates(removeDuplicates)
{
	bestSolution = nullptr;
}

Pyramid::~Pyramid() {
	for (Population* population : populations) {
		delete population;
	}
}

void Pyramid::runSingleIteration() {
	Solution* newSolution = solutionFactory->nextRandomSolution();
	newSolution->evaluate(*problem);
	localOptimizer->optimizeLocally(newSolution);
	tryAddSolutionToPyramid(newSolution);
}

void Pyramid::optimizeLocally(Solution& solution) {
	Solution* copy = new Solution(solution);
	localOptimizer->optimizeLocally(copy);
	Solution* bestAddedSolution = tryAddSolutionToPyramid(copy);
	if(bestAddedSolution != nullptr) {
		solution = *bestAddedSolution;
	}
}

Solution* Pyramid::tryAddSolutionToPyramid(Solution* solution) {
	return tryAddSolutionToPyramid(solution, 0);
}

Solution* Pyramid::tryAddSolutionToPyramid(Solution* solution, int level) {
	bool addedBaseSolution = addSolutionToPyramidIfUnique(solution, level);
	Solution* bestAddedSolution = tryToAddImprovedSolutions(solution, level);
	if(!addedBaseSolution) {
		delete solution;
	}
	if(bestAddedSolution == nullptr && addedBaseSolution)
	{
		return solution;
	} else
	{
		return bestAddedSolution;
	}
}

Solution* Pyramid::tryToAddImprovedSolutions(Solution* solution, int level) {
	Solution* currentlyImprovedSolution = new Solution(*solution);
	Solution* lastAddedSolution = nullptr;
	bool addedImprovedSolution = false;
	for (int lev = level; lev < populations.size(); lev++) {
		double previousFitness = currentlyImprovedSolution->getFitness();
		populations[lev]->improve(currentlyImprovedSolution);
		if (previousFitness < currentlyImprovedSolution->getFitness()) {
			addedImprovedSolution = addSolutionToPyramidIfUnique(currentlyImprovedSolution, lev + 1);
			if (addedImprovedSolution) {
				lastAddedSolution = currentlyImprovedSolution;
				currentlyImprovedSolution = new Solution(*currentlyImprovedSolution);
				addedImprovedSolution = false;
			}
		}
	}
	if (!addedImprovedSolution) {
		delete currentlyImprovedSolution;
	}
	return lastAddedSolution;
}

bool Pyramid::addSolutionToPyramidIfUnique(Solution* solution, int level) {
	if(removeDuplicates) {
		solution->recalculatePhenotype();
		std::vector<int> phenotype(solution->getPhenotype());
		seenSolution = seen.find(phenotype);
		if (seenSolution != seen.end()) {
			return false;
		}
		seen.insert(phenotype);
	}
	addSolutionToPyramid(solution, level);
	return true;
}

void Pyramid::addSolutionToPyramid(Solution* solution, int level) {
	ensurePyramidCapacity(level);
	populations[level]->addSolution(solution);
	checkIfBest(solution);
}

void Pyramid::ensurePyramidCapacity(int level) {
	if (populations.size() == level) {
		populations.push_back(populationFactory->newPopulation());
	}
}

void Pyramid::checkIfBest(Solution* solution) {
	if (bestSolution == nullptr || solution->getFitness() > bestSolution->getFitness()) {
		bestSolution = solution;
	}
}

vector<int> Pyramid::getBestSolutionPhenotype() const {
    return bestSolution->getPhenotype();
}

Solution* Pyramid::getBestSolution() const {
	return bestSolution;
}

double Pyramid::getBestFitness() const {
	if(bestSolution != nullptr) {
		return bestSolution->getFitness();
	} else {
		return 0;
	}
}