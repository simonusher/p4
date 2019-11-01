#pragma once
#include "LocalOptimizer.h"

class NullOptimizer : public LocalOptimizer {
public:
	explicit NullOptimizer(Problem* problem)
		: LocalOptimizer(problem) {}

	void optimizeLocally(Solution& solution) override
	{

	}
};