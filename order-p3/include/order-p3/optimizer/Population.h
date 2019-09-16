//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include "Linkage.h"
#include <algorithm>
#include <numeric>
#include "../evaluation/Evaluator.h"


class Population {
public:
    Population(int numberOfBits, vector<int>&geneDomain);
    ~Population();
    void addSolution(vector<int> &solution);
    void improve(vector<int> &solution, double &fitness, CEvaluator& evaluator);
    Linkage *getLinkage() const;
private:
    void recalculateLinkage(vector<int> &solution);
    bool mix(vector<int> &solution, double &fitness, vector<int>& cluster, vector<int> &source, CEvaluator& evaluator);
    void shuffleCheckingOrder(std::mt19937& random);
    vector<vector<int>> solutions;
    Linkage *linkage;
    vector<int> solutionCheckingOrder;
	std::mt19937* randomGenerator;
};
