#pragma once
#include "RandomKeyDecoder.h"
#include <random>

class MaskedDecoder : public RandomKeyDecoder {
public:
	MaskedDecoder(std::mt19937& randomGenerator, int numberOfGenes);
	MaskedDecoder(std::vector<int> orderingMask);
	std::vector<int> decode(const std::vector<double>& encodedGenotype) override;
private:
	std::vector<int> orderingMask;
};
