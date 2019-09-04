#include "../include/order-p3/RandomKeyDecoder.h"

std::vector<int> RandomKeyDecoder::decode(const std::vector<double>& encodedGenotype) {
	std::vector<int> phenotype(encodedGenotype.size());
	std::iota(phenotype.begin(), phenotype.end(), 0);
	std::sort(phenotype.begin(), phenotype.end(), 
		[&encodedGenotype](int firstIndex, int secondIndex) {
		return encodedGenotype[firstIndex] < encodedGenotype[secondIndex];
	});
	return phenotype;
}
