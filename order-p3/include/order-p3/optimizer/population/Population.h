//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include <algorithm>
#include <numeric>
#include "../../problem/Problem.h"
#include "../solution/SolutionMixer.h"
#include "../Linkage.h"


class Population {
public:
    Population(int problemSize, SolutionMixer& solutionMixer, std::mt19937& randomGenerator);
    ~Population();
    void addSolution(Solution* solution);
    void improve(Solution* solution);
private:
    void recalculateLinkage(Solution* solution);
    std::vector<Solution*> solutions;
    Linkage linkage;
    std::vector<int> solutionCheckingOrder;
	std::mt19937& randomGenerator;
	SolutionMixer& solutionMixer;
};
