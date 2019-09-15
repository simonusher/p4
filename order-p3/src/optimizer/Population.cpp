//
// Created by Szymon on 22.11.2018.
//

#include "../../include/order-p3/optimizer/Population.h"

Population::Population(int numberOfBits, vector<int> &geneDomain) {
    this->linkage = new Linkage(numberOfBits, geneDomain);
}

Population::~Population() {
    delete linkage;
}

void Population::addSolution(vector<int> &solution, std::mt19937& random) {
    solutions.push_back(solution);
    recalculateLinkage(solution, random);
}

void Population::recalculateLinkage(vector<int> &solution, std::mt19937& random) {
	this->linkage->recalculate(this->solutions.size(), solution, random);
}

Linkage *Population::getLinkage() const {
    return linkage;
}

void Population::improve(vector<int> &solution, double &fitness, CEvaluator& evaluator, std::mt19937& random) {
    vector<vector<int>> clusters = linkage->getClusters();
    vector<int> cluster;
    bool different;
    for (int i = 0; i < clusters.size(); i++) {
        shuffleCheckingOrder(random);
        cluster = clusters[i];
        different = false;
        for (int j = 0; j < solutionCheckingOrder.size() && !different; j++) {
            different = mix(solution, fitness, cluster, solutions[solutionCheckingOrder[j]], evaluator);
        }
    }
}

bool Population::mix(vector<int> &solution, double &fitness, vector<int> &cluster, vector<int> &source,
		CEvaluator& evaluator) {
	bool changed = false;
	for (const int& index : cluster) {
		changed |= (solution[index] != source[index]);
		// uses the "source" to store the original value of "solution"
		//vector<int>::swap(solution[index], source[index]);
		std::swap(solution[index], source[index]);
	}

	if (changed) {
		float new_fitness = evaluator.dEvaluate(solution);
		// NOTE: My previous work used strict improvement
		if (fitness <= new_fitness) {
			// improvement made, keep change to solution
			fitness = new_fitness;
			// copy pattern back into the source, leave solution changed
			for (const int& index : cluster) {
				source[index] = solution[index];
			}
		}
		else {
			// revert both solution and source
			for (const auto& index : cluster) {
				std::swap(solution[index], source[index]);
			}
		}
	}
	return changed;
}

void Population::shuffleCheckingOrder(std::mt19937& random) {
    if(solutionCheckingOrder.size() != solutions.size()){
        solutionCheckingOrder = vector<int>(solutions.size());
        std::iota(solutionCheckingOrder.begin(), solutionCheckingOrder.end(), 0);
    }
    std::shuffle(solutionCheckingOrder.begin(), solutionCheckingOrder.end(), random);
}
