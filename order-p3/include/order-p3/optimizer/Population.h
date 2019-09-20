//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include "Linkage.h"
#include <algorithm>
#include <numeric>
#include "../evaluation/Evaluator.h"
#include "solution/SolutionMixer.h"


class Population {
public:
    Population(Problem* problem, std::mt19937& randomGenerator);
    ~Population();
    void addSolution(Solution* solution);
    void improve(Solution* solution);
    Linkage *getLinkage() const;
private:
    void recalculateLinkage(Solution* solution) const;
    void shuffleCheckingOrder();
	vector<Solution*> solutions;
	Problem* problem;
    Linkage *linkage;
    vector<int> solutionCheckingOrder;
	std::mt19937& randomGenerator;
	SolutionMixer* solutionMixer;
};
