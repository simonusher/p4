#pragma once
#include "../Problem.h"
#include "../../util/VectorHasher.h"
#include <unordered_map>
#include <numeric>
#include <algorithm>

class RelativeOrderingProblem : public Problem {
public:
	RelativeOrderingProblem(int numberOfFunctions);
	double evaluate(std::vector<int>& solution) override;
private:
	int numberOfFunctions;
	double maximumFitness;
	const static std::unordered_map<std::vector<int>, double, VectorHasher> functionValues;
};
