//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include "Linkage.h"
#include <algorithm>
#include <numeric>
#include "Evaluator.h"


class Population {
public:
    Population(int numberOfBits, vector<int>&geneDomain);
    ~Population();
    void addSolution(vector<int> &solution, std::mt19937& random);
    void improve(vector<int> &solution, double &fitness, CEvaluator& evaluator, std::mt19937& random);
    Linkage *getLinkage() const;
private:
    bool mix(vector<int> &solution, double &fitness, vector<int>& cluster, vector<int> &source, CEvaluator& evaluator);
    vector<vector<int>> solutions;
    Linkage *linkage;
    void shuffleCheckingOrder(std::mt19937& random);
    vector<int> solutionCheckingOrder;

    void recalculateLinkage(vector<int> &solution, std::mt19937& random);
};