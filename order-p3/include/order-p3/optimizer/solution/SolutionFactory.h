#pragma once
#include "Solution.h"

class SolutionFactory {
public:
	SolutionFactory(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	Solution* nextSolution();

private:
	RandomKeyEncoder& encoder;
	RandomKeyDecoder& decoder;
};
