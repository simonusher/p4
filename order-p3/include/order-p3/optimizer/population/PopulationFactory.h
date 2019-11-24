#pragma once
#include "Population.h"

class PopulationFactory {
public:
	PopulationFactory(Problem& problem, SolutionMixer& solutionMixer, std::mt19937& randomGenerator);
	Population* newPopulation();
private:
	std::mt19937& randomGenerator;
	SolutionMixer& solutionMixer;
	Problem& problem;
};
