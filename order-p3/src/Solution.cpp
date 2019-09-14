#include "../include/order-p3/Solution.h"


Solution::Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder): genotype(encoder.getRandomEncoding()), phenotype(encoder.getNumberOfGenes()) {
	decodeGenotypeToPhenotype(decoder);
}

void Solution::setPhenotype(std::vector<int>& newPhenotype, RandomKeyEncoder& encoder) {
	phenotype = newPhenotype;
	encodePhenotypeToGenotype(encoder);
}

void Solution::setGenotype(std::vector<double>& newGenotype, RandomKeyDecoder& decoder) {
	this->genotype = newGenotype;
	decodeGenotypeToPhenotype(decoder);
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

void Solution::decodeGenotypeToPhenotype(RandomKeyDecoder& decoder) {
	phenotype = decoder.decode(genotype);
}

void Solution::encodePhenotypeToGenotype(RandomKeyEncoder& encoder) {
	genotype = encoder.getEncodingForPhenotype(phenotype);
}