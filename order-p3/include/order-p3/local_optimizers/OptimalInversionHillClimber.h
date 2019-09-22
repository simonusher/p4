#pragma once
#include "LocalOptimizer.h"

class OptimalInversionHillClimber : public LocalOptimizer {

public:
	OptimalInversionHillClimber(Problem* problem, RandomKeyEncoder* encoder);

	void optimize(Solution& solution) override;
private:
	RandomKeyEncoder* encoder;
};
