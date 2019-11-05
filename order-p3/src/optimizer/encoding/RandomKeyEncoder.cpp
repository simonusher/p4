#include "../../../include/order-p3/optimizer/encoding/RandomKeyEncoder.h"

RandomKeyEncoder::RandomKeyEncoder(double lowerBound, double upperBound, int numberOfGenes, std::mt19937& randomGenerator) :
	randomGenerator(randomGenerator),
	keyDistribution(lowerBound, upperBound),
	numberOfGenes(numberOfGenes)
{
}

std::vector<double> RandomKeyEncoder::getRandomEncoding() const {
	return getRandomEncoding(this->numberOfGenes);
}

std::vector<double> RandomKeyEncoder::getRandomEncoding(int numberOfGenes) const {
	std::vector<double> encoding(numberOfGenes);
	for(size_t i = 0; i < numberOfGenes; i++) {
		encoding[i] = getRandomKey();
	}
	return encoding;
}

std::vector<double> RandomKeyEncoder::getEncodingForPhenotype(std::vector<int>& phenotype) const {
	std::vector<double> randomEncoding(getRandomEncoding(phenotype.size()));
	std::sort(randomEncoding.begin(), randomEncoding.end());
	std::vector<double> phenotypeEncoding(randomEncoding.size());
	for (size_t i = 0; i < phenotype.size(); i++) {
		phenotypeEncoding[phenotype[i]] = randomEncoding[i];
	}
	return phenotypeEncoding;
}

double RandomKeyEncoder::getRandomKey() const {
	return this->keyDistribution(this->randomGenerator);
}

int RandomKeyEncoder::getNumberOfGenes() const {
	return numberOfGenes;
}