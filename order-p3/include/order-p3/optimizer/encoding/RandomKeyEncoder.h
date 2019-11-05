#pragma once
#include <random>
class RandomKeyEncoder
{
public:
	RandomKeyEncoder(double lowerBound, double upperBound, int numberOfGenes, std::mt19937& randomGenerator);
	std::vector<double> getRandomEncoding() const;
	std::vector<double> getRandomEncoding(int numberOfGenes) const ;
	std::vector<double> getEncodingForPhenotype(std::vector<int>& phenotype) const;
	double getRandomKey() const;

	int getNumberOfGenes() const;
private:
	std::mt19937& randomGenerator;
	std::uniform_real_distribution<double> keyDistribution;
	int numberOfGenes;
};

