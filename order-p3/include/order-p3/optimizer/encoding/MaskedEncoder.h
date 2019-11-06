#pragma once
#include "RandomKeyEncoder.h"

class MaskedEncoder : public RandomKeyEncoder {
public:
	MaskedEncoder(double lowerBound, double upperBound, int numberOfGenes, std::mt19937& randomGenerator,
	              const std::vector<int>& orderingMask);


	std::vector<double> getEncodingForPhenotype(std::vector<int>& phenotype) const override;

	static MaskedEncoder get8FunctionDeflen6Coding(double lowerBound, double upperBound, int numberOfGenes, std::mt19937& randomGenerator);
	static MaskedEncoder get8FunctionLooseCoding(double lowerBound, double upperBound, int numberOfGenes, std::mt19937& randomGenerator);
private:
	std::vector<int> orderingMask;
};
