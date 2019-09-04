#pragma once
#include <random>
class RandomKeyEncoder
{
public:
	RandomKeyEncoder(double lowerBound, double upperBound, int numberOfGenes);
	std::vector<double> getRandomEncoding();
	std::vector<double> getRandomEncoding(int numberOfGenes);
	double getRandomKey();

	int getNumberOfGenes() const;
private:
	void initializeRandom();
	void initializeRandomEngine();
	void initializeDistribution();
	std::mt19937 randomEngine;
	std::uniform_real_distribution<double> keyDistribution;
	double lowerBound;
	double upperBound;
	int numberOfGenes;
};

