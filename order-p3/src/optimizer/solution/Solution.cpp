#include "../../../include/order-p3/optimizer/solution/Solution.h"

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

double Solution::getRandomKey(int index) const {
	return genotype[index];
}

std::vector<double> Solution::getGenotype() const {
	return genotype;
}

std::vector<int> Solution::getPhenotype() const {
	return phenotype;
}

std::vector<double>* Solution::getGenotypeRef() {
	return &genotype;
}

std::vector<int>* Solution::getPhenotypeRef() {
	return &phenotype;
}

double Solution::evaluate(Problem& problem) {
	this->fitness = problem.evaluate(this->phenotype);
	return fitness;
}

double Solution::getFitness() const { return fitness; }

void Solution::decodeGenotypeToPhenotype(RandomKeyDecoder& decoder) {
	phenotype = decoder.decode(genotype);
}

void Solution::encodePhenotypeToGenotype(RandomKeyEncoder& encoder) {
	genotype = encoder.getEncodingForPhenotype(phenotype);
}