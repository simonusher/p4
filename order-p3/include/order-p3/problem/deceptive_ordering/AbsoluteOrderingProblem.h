#pragma once
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include "../../util/VectorHasher.h"
#include "../Problem.h"

class AbsoluteOrderingProblem : public Problem {
public:
	AbsoluteOrderingProblem(int numberOfFunctions);
	double evaluate(std::vector<int>& solution) override;
	double evaluate(std::vector<double>& solution) ;

	double evaluateAbsolute(const std::vector<int>& solution);
	double evaluateRelative(const std::vector<int>& solution);


	int getProblemSize() override;
	int getFitnessFunctionEvaluations() override;
private:
	std::vector<int> problemCoding;
	int numberOfFunctions;
	double maximumFitness;
	int fitnessFunctionEvaluations;
	void initializeFunctionValues();
	std::unordered_map<std::vector<int>, double, VectorHasher> functionValues;
	std::unordered_map<std::vector<int>, double, VectorHasher> relativeFunctionValues;
};
