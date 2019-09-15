#pragma once

#include "../evaluation/Evaluator.h"
#include <random>
#include <vector>
#include "Pyramid.h"

using namespace std;

class COptimizer {
public:
	COptimizer(CEvaluator &cEvaluator);
	~COptimizer();
	void vInitialize();
	void vRunIteration();

	vector<int> *pvGetCurrentBest() { return &v_current_best; }
	double getBestFitness() const { return d_current_best_fitness; }
	int getSeenIndividuals() const { return pyramid->getSeenIndividuals(); }

private:
	CEvaluator & c_evaluator;
	Pyramid *pyramid;
	double d_current_best_fitness;
	vector<int> v_current_best;
};//class COptimizer