#pragma once
#include "../problem/Problem.h"
#include "../optimizer/solution/Solution.h"

class HillClimber {
public:
	HillClimber(Problem* problem);
	virtual ~HillClimber() = default;
	
	virtual void hillClimb(Solution& solution) = 0;
	virtual void hillClimb(Solution* solution);
protected:
	Problem* problem;
};
