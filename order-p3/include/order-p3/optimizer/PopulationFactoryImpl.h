#pragma once
#include "PopulationFactory.h"

class PopulationFactoryImpl: public PopulationFactory {
public:
	PopulationFactoryImpl(Problem* problem, std::mt19937& randomGenerator);
	Population* newPopulation() override;
private:
	std::mt19937& randomGenerator;
	Problem* problem;
};
