#pragma once
#include "Solution.h"

class SolutionMixer {
public:
	virtual ~SolutionMixer() = default;
	virtual bool mix(Solution* destination, Solution* source, std::vector<int>* cluster) = 0;
};
