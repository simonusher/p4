//
// Created by Szymon on 24.11.2018.
//

#include "../../include/order-p3/optimizer/Pyramid.h"

Pyramid::Pyramid(Problem* problem, SolutionFactory* solutionFactory, LocalOptimizer* localOptimizer)
	: problem(problem), solutionFactory(solutionFactory), localOptimizer(localOptimizer) {
	this->generator = std::mt19937(randomDevice());
	bestSolution = nullptr;
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
	bool added = addSolutionToPyramidIfUnique(solution, level);
	if (added) {
		for (int lev = level; lev < populations.size(); lev++) {
			double previousFitness = solution->getFitness();
			populations[lev]->improve(solution, *evaluator, generator);
			if (previousFitness < solution->getFitness()) {
				added = addSolutionToPyramidIfUnique(solution, lev + 1) || added;
			}
		}
	} else {
		delete solution;
	}
	return added;
}


bool Pyramid::addSolutionToPyramidIfUnique(Solution* solution, int level) {
	std::vector<int> phenotype(solution->getPhenotype());
    seenSolution = seen.find(phenotype);
    if(seenSolution != seen.end()){
        return false;
    } else {
        seen.insert(phenotype);
		ensurePyramidCapacity(level);
        populations[level]->addSolution(solution, generator);
		checkIfBest(solution);
        
        return true;
    }
}

void Pyramid::ensurePyramidCapacity(int level) {
	if (populations.size() == level) {
		populations.push_back(make_unique<Population>(numberOfGenes, geneDomain));
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
