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

double Solution::evaluate(CEvaluator& evaluator) {
	this->fitness = evaluator.dEvaluate(this->phenotype);
	return fitness;
}

double Solution::getFitness() const { return fitness; }
