#include "../../../include/order-p3/optimizer/population/PopulationFactoryImpl.h"


PopulationFactoryImpl::PopulationFactoryImpl(Problem& problem, SolutionMixer& solutionMixer, std::mt19937& randomGenerator)
	: randomGenerator(randomGenerator), solutionMixer(solutionMixer), problem(problem) { }

Population* PopulationFactoryImpl::newPopulation() {
	return new Population(problem, solutionMixer, randomGenerator);
}