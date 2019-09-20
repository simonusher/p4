#pragma once
#include "PopulationFactory.h"

class PopulationFactoryImpl: public PopulationFactory {
public:
	PopulationFactoryImpl(Problem* problem, std::mt19937& randomGenerator);
	Population* getNewPopulation() const;
private:
	std::mt19937& randomGenerator;
	Problem* problem;
};
