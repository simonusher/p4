//
// Created by Szymon on 24.11.2018.
//

#include <utility>
#include "../../include/order-p3/optimizer/Pyramid.h"

Pyramid::Pyramid(
	Problem& problem, 
	SolutionFactory& solutionFactory, 
	PopulationFactory& populationFactory, 
	LocalOptimizer& localOptimizer,
	std::function<void(Solution * solution)> onBestSolutionFound
) :
	iterationsRun(0),
	bestSolution(nullptr),
	problem(problem),
	solutionFactory(solutionFactory),
	populationFactory(populationFactory),
	localOptimizer(localOptimizer),
	onBestSolutionFound(std::move(onBestSolutionFound))
{
}

Pyramid::~Pyramid() {
	for (Population* population : populations) {
		delete population;
	}
	delete bestSolution;
}

void Pyramid::runSingleIteration() {
	Solution* newSolution = solutionFactory.nextSolution();
	newSolution->evaluate(problem);
	checkIfBest(newSolution);
	localOptimizer.optimizeLocally(newSolution);
	tryAddSolutionToPyramid(newSolution);
	iterationsRun++;
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
	addSolutionToPyramid(solution, level);
	return true;
}

void Pyramid::addSolutionToPyramid(Solution* solution, int level) {
	solution->reEncode();
	ensurePyramidCapacity(level);
	checkIfBest(solution);
	populations[level]->addSolution(solution);
}

void Pyramid::ensurePyramidCapacity(int level) {
	if (populations.size() == level) {
		populations.push_back(populationFactory.newPopulation());
	}
}

void Pyramid::checkIfBest(Solution* solution) {
	if (bestSolution != nullptr) {
		if(solution->getFitness() > bestSolution->getFitness()) {
			delete bestSolution;
			bestSolution = new Solution(*solution);
			if(onBestSolutionFound) {
				onBestSolutionFound(bestSolution);
			}
		}
	} else {
		bestSolution = new Solution(*solution);
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

int Pyramid::getIterationsRun() {
	return iterationsRun;
}

int Pyramid::getNumberOfPopulations() {
	return populations.size();
}
