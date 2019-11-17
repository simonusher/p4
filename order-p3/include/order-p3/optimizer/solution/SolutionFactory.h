#pragma once
#include "Solution.h"

class SolutionFactory {
public:
	virtual ~SolutionFactory() = default;
	virtual Solution* nextSolution() = 0;
};
