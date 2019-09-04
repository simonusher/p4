#include "../include/order-p3/RandomKeyEncoder.h"


RandomKeyEncoder::RandomKeyEncoder(double lowerBound, double upperBound, int numberOfGenes)
	: lowerBound(lowerBound),
	upperBound(upperBound),
	numberOfGenes(numberOfGenes)
{
	initializeRandom();
}

std::vector<double> RandomKeyEncoder::getRandomEncoding() {
	return getRandomEncoding(this->numberOfGenes);
}

std::vector<double> RandomKeyEncoder::getRandomEncoding(int numberOfGenes) {
	std::vector<double> encoding(numberOfGenes);
	for(size_t i = 0; i < numberOfGenes; i++) {
		encoding[i] = getRandomKey();
	}
	return encoding;
}

double RandomKeyEncoder::getRandomKey() {
	return this->keyDistribution(this->randomEngine);
}

int RandomKeyEncoder::getNumberOfGenes() const {
	return numberOfGenes;
}

void RandomKeyEncoder::initializeRandom() {
	initializeRandomEngine();
	initializeDistribution();
}

void RandomKeyEncoder::initializeRandomEngine() {
	std::random_device randomDevice;
	this->randomEngine.seed(randomDevice());
}

void RandomKeyEncoder::initializeDistribution() {
	this->keyDistribution = std::uniform_real_distribution<double>(lowerBound, upperBound);
}