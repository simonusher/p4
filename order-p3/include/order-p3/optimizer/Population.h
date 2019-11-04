//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include "NewLinkage.h"
#include "Linkage.h"
#include <algorithm>
#include <numeric>
#include "../evaluation/Evaluator.h"
#include "solution/SolutionMixer.h"
#include "OptimizedLinkage.h"


class Population {
public:
    Population(Problem* problem, SolutionMixer* solutionMixer, std::mt19937& randomGenerator);
    ~Population();
    void addSolution(Solution* solution);
    void improve(Solution* solution);
private:
    void recalculateLinkage(Solution* solution) const;
    void shuffleCheckingOrder();
	vector<Solution*> solutions;
	Problem* problem;
    // NewLinkage *linkage;
    OptimizedLinkage *linkage;
    vector<int> solutionCheckingOrder;
	std::mt19937& randomGenerator;
	SolutionMixer* solutionMixer;
};
