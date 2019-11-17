#pragma once
#include "SolutionFactory.h"

class SolutionFactoryImpl : public SolutionFactory {
public:
	SolutionFactoryImpl(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	Solution* nextSolution() override;

private:
	RandomKeyEncoder& encoder;
	RandomKeyDecoder& decoder;
};
