#include "../../../include/order-p3/optimizer/encoding/MaskedEncoder.h"

MaskedEncoder::MaskedEncoder(double lowerBound, double upperBound, int numberOfGenes, std::mt19937& randomGenerator,
                             const std::vector<int>& orderingMask): RandomKeyEncoder(lowerBound, upperBound,
                                                                                     numberOfGenes, randomGenerator),
                                                                    orderingMask(orderingMask) {}

std::vector<double> MaskedEncoder::getEncodingForPhenotype(std::vector<int>& phenotype) const {
	std::vector<double> orderedGenotype = RandomKeyEncoder::getEncodingForPhenotype(phenotype);
	std::vector<double> maskedGenotype(orderedGenotype.size());
	for(int i = 0; i < maskedGenotype.size(); i++) {
		maskedGenotype[orderingMask[i]] = orderedGenotype[i];
	}
	return maskedGenotype;
}

MaskedEncoder MaskedEncoder::get8FunctionDeflen6Coding(double lowerBound, double upperBound, int numberOfGenes,
	std::mt19937& randomGenerator) {
	return MaskedEncoder(lowerBound, upperBound, numberOfGenes, randomGenerator,
		{ 0, 2, 4, 6, 1, 3, 5, 7, 8, 10, 12, 14, 9, 11, 13, 15, 16, 18, 20, 22, 17, 19, 21, 23, 24, 26, 28, 30, 25, 27, 29, 31 });
}

MaskedEncoder MaskedEncoder::get8FunctionLooseCoding(double lowerBound, double upperBound, int numberOfGenes,
	std::mt19937& randomGenerator) {
	return MaskedEncoder(lowerBound, upperBound, numberOfGenes, randomGenerator,
		{ 0, 8, 16, 24, 1, 9, 17, 25, 2, 10, 18, 26, 3, 11, 19, 27, 4, 12, 20, 28, 4, 13, 21, 29, 6, 14, 22, 30, 7, 15, 23, 31 });
}
