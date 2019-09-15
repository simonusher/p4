#pragma once
#include "../Solution.h"
#include "../problem/Problem.h"

class HillClimber {
public:
	HillClimber(Problem* problem);
	virtual ~HillClimber() = default;
	
	virtual void hillClimb(Solution& solution) = 0;
	virtual void hillClimb(Solution* solution);
protected:
	Problem* problem;
};
