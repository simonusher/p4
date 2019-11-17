//
// Created by Szymon on 22.11.2018.
//

#include "../../../include/order-p3/optimizer/population/Population.h"

Population::Population(Problem& problem, SolutionMixer& solutionMixer, std::mt19937& randomGenerator) :
	problem(problem),
	linkage(problem.getProblemSize(), randomGenerator),
	randomGenerator(randomGenerator),
	solutionMixer(solutionMixer)
{
}

Population::~Population() {
    for (Solution* solution : solutions) {
		delete solution;
    }
}

void Population::addSolution(Solution* solution) {
    solutions.push_back(solution);
	solutionCheckingOrder.push_back(solutions.size() - 1);
    recalculateLinkage(solution);
}

void Population::recalculateLinkage(Solution* solution) {
	linkage.update(solution, this->solutions.size());
}

void Population::improve(Solution* solution) {
	bool different;
	vector<int> options(solutionCheckingOrder);
	int unused;
	int index, working = 0;


	for (auto it = linkage.randomBegin(); it != linkage.randomEnd(); ++it) {
	// for (auto it = linkage->begin(); it != linkage->end(); ++it) {
		auto cluster = *it;
		unused = options.size() - 1;
		different = false;
		do {
			
			// Choose a donor
			index = std::uniform_int_distribution<int>(0, unused)(randomGenerator);
			working = options[index];
			// make certain that donor cannot be chosen again
			std::swap(options[unused], options[index]);
			unused -= 1;

			// Attempt the donation
			different = solutionMixer.mix(solution, solutions[working], &cluster);
			// Break loop if configured to stop_after_one or donate returned true
		} while (unused >= 0 and not different);
	}
}