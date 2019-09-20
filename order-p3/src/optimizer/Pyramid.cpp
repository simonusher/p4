//
// Created by Szymon on 24.11.2018.
//

#include "../../include/order-p3/optimizer/Pyramid.h"

Pyramid::Pyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory, LocalOptimizer* localOptimizer)
	: problem(problem), solutionFactory(solutionFactory), populationFactory(populationFactory), localOptimizer(localOptimizer) {
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
	localOptimizer->optimize(newSolution);
	tryAddSolutionToPyramid(newSolution);
}

bool Pyramid::tryAddSolutionToPyramid(Solution* solution) {
	return tryAddSolutionToPyramid(solution, 0);
}

bool Pyramid::tryAddSolutionToPyramid(Solution* solution, int level) {
	bool addedBaseSolution = addSolutionToPyramidIfUnique(solution, level);
	if (addedBaseSolution) {
		tryToAddImprovedSolutions(solution, level);
	}
	else {
		delete solution;
	}
	return addedBaseSolution;
}

void Pyramid::tryToAddImprovedSolutions(Solution* solution, int level) {
	Solution* currentlyImprovedSolution = new Solution(*solution);
	bool addedImprovedSolution = false;
	for (int lev = level; lev < populations.size(); lev++) {
		double previousFitness = currentlyImprovedSolution->getFitness();
		populations[lev]->improve(currentlyImprovedSolution);
		if (previousFitness < currentlyImprovedSolution->getFitness()) {
			addedImprovedSolution = addSolutionToPyramidIfUnique(currentlyImprovedSolution, lev + 1);
			if (addedImprovedSolution) {
				currentlyImprovedSolution = new Solution(*currentlyImprovedSolution);
				addedImprovedSolution = false;
			}
		}
	}
	if (!addedImprovedSolution) {
		delete currentlyImprovedSolution;
	}
}

bool Pyramid::addSolutionToPyramidIfUnique(Solution* solution, int level) {
	std::vector<int> phenotype(solution->getPhenotype());
    seenSolution = seen.find(phenotype);
    if(seenSolution != seen.end()){
        return false;
    } else {
        seen.insert(phenotype);
		ensurePyramidCapacity(level);
        populations[level]->addSolution(solution);
		checkIfBest(solution);
        return true;
    }
}

void Pyramid::ensurePyramidCapacity(int level) {
	if (populations.size() == level) {
		populations.push_back(populationFactory->newPopulation());
	}
}

void Pyramid::checkIfBest(Solution* solution) {
	if (solution->getFitness() > bestSolution->getFitness()) {
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
    return bestSolution->getFitness();
}
