//
// Created by Szymon on 22.11.2018.
//

#include "../../include/order-p3/optimizer/Population.h"

Population::Population(Problem* problem, std::mt19937& randomGenerator) : problem(problem), randomGenerator(randomGenerator) {
    this->linkage = new Linkage(problem->getProblemSize(), randomGenerator);
}

Population::~Population() {
    delete linkage;
    for (Solution* solution : solutions) {
		delete solution;
    }
}

void Population::addSolution(Solution* solution) {
    solutions.push_back(solution);
    recalculateLinkage(solution);
}

void Population::recalculateLinkage(Solution* solution) const {
	this->linkage->recalculate(this->solutions.size(), solution);
}

Linkage *Population::getLinkage() const {
    return linkage;
}

void Population::improve(Solution* solution) {
    vector<vector<int>> clusters = linkage->getClusters();
    vector<int> cluster;
    bool different;
    for (int i = 0; i < clusters.size(); i++) {
        shuffleCheckingOrder();
        cluster = clusters[i];
        different = false;
        for (int j = 0; j < solutionCheckingOrder.size() && !different; j++) {
			different = solutionMixer->mix(solution, solutions[solutionCheckingOrder[j]], &cluster);
        }
    }
}

void Population::shuffleCheckingOrder() {
    if(solutionCheckingOrder.size() != solutions.size()){
        solutionCheckingOrder = vector<int>(solutions.size());
        std::iota(solutionCheckingOrder.begin(), solutionCheckingOrder.end(), 0);
    }
    std::shuffle(solutionCheckingOrder.begin(), solutionCheckingOrder.end(), randomGenerator);
}
