#include "../../include/order-p3/local_optimizers/OptimalInversionHillClimber.h"
#include <iostream>

OptimalInversionHillClimber::OptimalInversionHillClimber(Problem& problem): LocalOptimizer(problem) {	}

void OptimalInversionHillClimber::optimizeLocally(Solution& solution) {
	std::vector<int>* solutionPhenotype = solution.getPhenotypePtr();
	for(int i = 2; i < solutionPhenotype->size() - 1 ; i++) {
		for(int j = 0; j <= solutionPhenotype->size() - i; j++) {
			double oldFitness = solution.getFitness();
			std::reverse(solutionPhenotype->begin() + j, solutionPhenotype->begin() + j + i);
			double newFitness = solution.evaluate(problem);
			if(newFitness <= oldFitness) {
				std::reverse(solutionPhenotype->begin() + j, solutionPhenotype->begin() + j + i);
			}
		}
	}
	solution.recalculateGenotype();
}
