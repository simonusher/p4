//
// Created by Szymon on 24.11.2018.
//

#include "../../include/order-p3/optimizer/Pyramid.h"

Pyramid::Pyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory, LocalOptimizer* localOptimizer, int maxPopulations,
	bool removeDuplicates,
	bool usePreprocessedLinkage)
	: LocalOptimizer(problem), problem(problem), solutionFactory(solutionFactory), populationFactory(populationFactory), localOptimizer(localOptimizer),
		removeDuplicates(removeDuplicates), iterationsRun(0), maxPopulations(maxPopulations)
{
	bestSolution = nullptr;
	if(usePreprocessedLinkage) {
		OptimizedLinkage::usePreprocessedLinkage = true;
		preprocessProblem();
	} else {
		OptimizedLinkage::usePreprocessedLinkage = false;
	}
}

Pyramid::~Pyramid() {
	for (Population* population : populations) {
		delete population;
	}
	delete bestSolution;
}

void Pyramid::runSingleIteration() {
	Solution* newSolution = solutionFactory->nextRandomSolution();
	newSolution->evaluate(*problem);
	checkIfBest(newSolution);
	localOptimizer->optimizeLocally(newSolution);
	tryAddSolutionToPyramid(newSolution);
	iterationsRun++;
	if(populations.size() > maxPopulations) {
		int toDelete = populations.size() - maxPopulations;
		for(int i = 0; i < toDelete; i++) {
			delete populations[i];
		}
		populations.erase(populations.begin(), populations.begin() + toDelete);
	}
}

void Pyramid::optimizeLocally(Solution& solution) {
	Solution* copy = new Solution(solution);
	localOptimizer->optimizeLocally(copy);
	Solution* bestAddedSolution = tryAddSolutionToPyramid(copy);
	if(bestAddedSolution != nullptr) {
		solution = *bestAddedSolution;
	}
}

void Pyramid::preprocessProblem() {
	vector<vector<double>> initialLinkage(problem->getProblemSize(), vector<double>(problem->getProblemSize(), 0));
	vector<int> phenotype(problem->getProblemSize());
	std::iota(phenotype.begin(), phenotype.end(), 0);

	int firstMiddleIndex = problem->getProblemSize() / 2;
	int secondMiddleIndex = firstMiddleIndex + 1;
	double maxDelta = -1;
	
	for(int i = 0; i < problem->getProblemSize(); i++) {
		for(int j = i + 1; j < problem->getProblemSize(); j++) {
			std::swap(phenotype[i], phenotype[firstMiddleIndex]);
			std::swap(phenotype[j], phenotype[secondMiddleIndex]);
			double fitness = problem->evaluate(phenotype);
			std::swap(phenotype[firstMiddleIndex], phenotype[secondMiddleIndex]);
			double swappedFitness = problem->evaluate(phenotype);
			double delta = swappedFitness - fitness;
			delta = delta >= 0 ? delta : -delta;
			if(delta > maxDelta) {
				maxDelta = delta;
			}
			if (fitness != swappedFitness) {
				// initialLinkage[i][j] = initialLinkage[j][i] = delta;
				initialLinkage[i][j] = initialLinkage[j][i] = 1;
			}
			std::swap(phenotype[firstMiddleIndex], phenotype[secondMiddleIndex]);
			std::swap(phenotype[j], phenotype[secondMiddleIndex]);
			std::swap(phenotype[i], phenotype[firstMiddleIndex]);
		}
	}
	// for (int i = 0; i < problem->getProblemSize(); i++) {
	// 	for (int j = i + 1; j < problem->getProblemSize(); j++) {
	// 		initialLinkage[i][j] = initialLinkage[j][i] = initialLinkage[i][j] / maxDelta;
	// 	}
	// }
	OptimizedLinkage::preprocessedLinkage = initialLinkage;
}

void Pyramid::reEncode() {
	for (Population* population : populations) {
		population->reEncode();
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

void Pyramid::improveUsingBest(Solution* solution) {
	if(bestSolution) {
		Solution* currentlyImprovedSolution = new Solution(*solution);
		bool addedImprovedSolution = false;
		for (int lev = 0; lev < populations.size(); lev++) {
			double previousFitness = currentlyImprovedSolution->getFitness();
			populations[lev]->improveUsingDonor(currentlyImprovedSolution, bestSolution);
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
}

Solution* Pyramid::tryToAddImprovedSolutions(Solution* solution, int level) {
	Solution* currentlyImprovedSolution = new Solution(*solution);
	Solution* lastAddedSolution = nullptr;
	bool addedImprovedSolution = false;
	// bool anyImprovement = false;
	for (int lev = level; lev < populations.size(); lev++) {
		double previousFitness = currentlyImprovedSolution->getFitness();
		populations[lev]->improve(currentlyImprovedSolution);
		if (previousFitness < currentlyImprovedSolution->getFitness()) {
			// anyImprovement = true;
			addedImprovedSolution = addSolutionToPyramidIfUnique(currentlyImprovedSolution, lev + 1);
			if (addedImprovedSolution) {
				lastAddedSolution = currentlyImprovedSolution;
				currentlyImprovedSolution = new Solution(*currentlyImprovedSolution);
				addedImprovedSolution = false;
			}
		}
	}
	// if(!anyImprovement) {
	// 	improveUsingBest(solution);
	// }
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
	solution->reEncode();
	ensurePyramidCapacity(level);
	checkIfBest(solution);
	populations[level]->addSolution(solution);
}

void Pyramid::ensurePyramidCapacity(int level) {
	if (populations.size() == level) {
		populations.push_back(populationFactory->newPopulation());
	}
}

void Pyramid::checkIfBest(Solution* solution) {
	if (bestSolution != nullptr) {
		if(solution->getFitness() > bestSolution->getFitness()) {
			delete bestSolution;
			bestSolution = new Solution(*solution);
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

double Pyramid::mean() {
	int populationSizesSum = 0;
	double fitnessSum = 0;
	for (Population* population : populations) {
		population->addMeanInformation(populationSizesSum, fitnessSum);
	}
	return fitnessSum / static_cast<double>(populationSizesSum);
}

int Pyramid::getNumberOfPopulations() {
	return populations.size();
}
