//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include <algorithm>
#include <numeric>
#include "solution/SolutionMixer.h"
#include "OptimizedLinkage.h"


class Population {
public:
    Population(Problem* problem, SolutionMixer* solutionMixer, std::mt19937& randomGenerator);
    ~Population();
    void addSolution(Solution* solution);
    void improve(Solution* solution);
    void improveUsingDonor(Solution* solution, Solution* donor);
	void reEncode();
private:
    void recalculateLinkage(Solution* solution) const;
	vector<Solution*> solutions;
	Problem* problem;
    OptimizedLinkage *linkage;
    vector<int> solutionCheckingOrder;
	std::mt19937& randomGenerator;
	SolutionMixer* solutionMixer;
};
