#include "../../../include/order-p3/optimizer/population/PopulationFactory.h"


PopulationFactory::PopulationFactory(int problemSize, SolutionMixer& solutionMixer, std::mt19937& randomGenerator)
	: randomGenerator(randomGenerator), solutionMixer(solutionMixer), problemSize(problemSize){ }

Population* PopulationFactory::newPopulation() {
	return new Population(problemSize, solutionMixer, randomGenerator);
}