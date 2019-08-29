#pragma once
#include <random>
class RandomKeyEncoder
{
public:
	RandomKeyEncoder(double lowerBound, double upperBound);
	std::vector<double> getRandomEncoding(int numberOfGenes);
	double getRandomKey();
	
private:
	void initializeRandom();
	void initializeRandomEngine();
	void initializeDistribution();
	std::mt19937 randomEngine;
	std::uniform_real_distribution<double> keyDistribution;
	double lowerBound;
	double upperBound;
};

