#pragma once
#include "../problem/Problem.h"
#include "../optimizer/solution/Solution.h"

class LocalOptimizer {
public:
	LocalOptimizer(Problem* problem);
	virtual ~LocalOptimizer() = default;
	
	virtual void optimize(Solution& solution) = 0;
	virtual void optimize(Solution* solution);
protected:
	Problem* problem;
};
