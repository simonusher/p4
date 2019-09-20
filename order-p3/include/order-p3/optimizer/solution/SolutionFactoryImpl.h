#pragma once
#include "SolutionFactory.h"

class SolutionFactoryImpl : public SolutionFactory {
public:
	SolutionFactoryImpl(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	Solution* nextRandomSolution() override;

private:
	RandomKeyEncoder& encoder;
	RandomKeyDecoder& decoder;
};
