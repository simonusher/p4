#pragma once
#include "solution/Solution.h"
#include "solution/SolutionFactory.h"
#include "../local_optimizers/LocalOptimizer.h"

class HcOptimizer {
public:
	HcOptimizer(Problem* problem, SolutionFactory* solutionFactory, LocalOptimizer* optimizer);
	~HcOptimizer();
	void runOneStep();
	double getBestFitness();
private:
	Solution* bestSolution;
	LocalOptimizer* optimizer;
	SolutionFactory* factory;
	Problem* problem;
};

inline HcOptimizer::HcOptimizer(Problem* problem, SolutionFactory* solutionFactory, LocalOptimizer* optimizer): problem(problem), factory(solutionFactory), optimizer(optimizer), bestSolution(nullptr){
}

inline HcOptimizer::~HcOptimizer() {
	delete bestSolution;
}

inline void HcOptimizer::runOneStep() {
	Solution* solution = factory->nextRandomSolution();
	solution->evaluate(*problem);
	optimizer->optimize(solution);
	if(bestSolution == nullptr || bestSolution->getFitness() < solution->getFitness()) {
		delete bestSolution;
		bestSolution = solution;
	} else {
		delete solution;
	}
}

inline double HcOptimizer::getBestFitness() {
	return bestSolution == nullptr ? 0 : bestSolution->getFitness();
}
