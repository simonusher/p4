#pragma once
#include "PopulationFactory.h"

class PopulationFactoryImpl: public PopulationFactory {
public:
	PopulationFactoryImpl(Problem& problem, SolutionMixer& solutionMixer, std::mt19937& randomGenerator);
	Population* newPopulation() override;
private:
	std::mt19937& randomGenerator;
	SolutionMixer& solutionMixer;
	Problem& problem;
};
