#pragma once
#include "LocalOptimizer.h"

class OptimalInversionHillClimber : public LocalOptimizer {
public:
	OptimalInversionHillClimber(Problem& problem);

	void optimizeLocally(Solution& solution) override;
};
