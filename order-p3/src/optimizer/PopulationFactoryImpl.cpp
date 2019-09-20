#include "../../include/order-p3/optimizer/PopulationFactoryImpl.h"


PopulationFactoryImpl::PopulationFactoryImpl(Problem* problem, std::mt19937& randomGenerator) : randomGenerator(randomGenerator), problem(problem) { }

Population* PopulationFactoryImpl::newPopulation() {
	return new Population(problem, randomGenerator);
}