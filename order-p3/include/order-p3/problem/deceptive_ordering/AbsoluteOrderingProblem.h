#pragma once
#include "../Problem.h"
#include <unordered_map>
#include "../../util/VectorHasher.h"

class AbsoluteOrderingProblem : public Problem {
public:
	AbsoluteOrderingProblem(int numberOfFunctions);
	double evaluate(std::vector<int>& solution) override;
	int getProblemSize() override;
	int getFitnessFunctionEvaluations() override;
private:
	int numberOfFunctions;
	double maximumFitness; 
	int fitnessFunctionEvaluations;
	void initializeFunctionValues();
	std::unordered_map<std::vector<int>, double, VectorHasher> functionValues;
};
