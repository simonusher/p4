#include "../include/order-p3/RandomKeyEncoder.h"


RandomKeyEncoder::RandomKeyEncoder(double lowerBound, double upperBound) {
	this->lowerBound = lowerBound;
	this->upperBound = upperBound;
	initializeRandomEngine();
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