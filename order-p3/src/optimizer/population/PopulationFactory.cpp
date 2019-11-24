#include "../../../include/order-p3/optimizer/population/PopulationFactory.h"


PopulationFactory::PopulationFactory(Problem& problem, SolutionMixer& solutionMixer, std::mt19937& randomGenerator)
	: randomGenerator(randomGenerator), solutionMixer(solutionMixer), problem(problem) { }

Population* PopulationFactory::newPopulation() {
	return new Population(problem, solutionMixer, randomGenerator);
}