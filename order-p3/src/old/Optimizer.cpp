#include "../../include/order-p3/old/Optimizer.h"

#include <cfloat>
#include <iostream>
#include <windows.h>

using namespace std;

COptimizer::COptimizer(CEvaluator &cEvaluator)
	: c_evaluator(cEvaluator)
{
	d_current_best_fitness = 0;
	vector<int> domain{ 0, 1 };
	this->pyramid = new Pyramid(domain, cEvaluator);
}//COptimizer::COptimizer(CEvaluator &cEvaluator)

COptimizer::~COptimizer() {
	delete pyramid;
}
//COptimizer::COptimizer(CEvaluator &cEvaluator)

void COptimizer::vInitialize()
{
	d_current_best_fitness = -DBL_MAX;
	v_current_best.clear();
}//void COptimizer::vInitialize()

void COptimizer::vRunIteration()
{
	this->pyramid->runSingleIteration();
	double newFitness = pyramid->getBestFitness();
	if (newFitness > d_current_best_fitness) {
		d_current_best_fitness = newFitness;
		v_current_best = pyramid->getBestSolution();
	}
}//void COptimizer::vRunIteration()