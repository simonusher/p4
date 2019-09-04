#include "../include/order-p3/Solution.h"


Solution::Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder): genotype(encoder.getRandomEncoding()), phenotype(encoder.getNumberOfGenes()) {
	decodePhenotype(decoder);
}

void Solution::decodePhenotype(RandomKeyDecoder& decoder) {
	phenotype = decoder.decode(genotype);
}

std::vector<double>& Solution::getGenotype() {
	return genotype;
}

std::vector<int>& Solution::getPhenotype() {
	return phenotype;
}