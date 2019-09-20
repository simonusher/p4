#include "../../include/order-p3/optimizer/Pyramid.h"
#include "../../include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../../include/order-p3/local_optimizers/SwapHillClimber.h"
#include "../../include/order-p3/optimizer/PopulationFactoryImpl.h"

int main() {
	RandomKeyEncoder encoder(0, 1, 10);
	RandomKeyDecoder decoder;
	SolutionFactory* solutionFactory = new SolutionFactoryImpl(encoder, decoder);
	Problem* problem = nullptr;
	LocalOptimizer* localOptimizer = new SwapHillClimber(problem, &encoder);
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	PopulationFactory* populationFactory = new PopulationFactoryImpl(problem, randomGenerator);
	Pyramid* pyramid = new Pyramid(problem, solutionFactory, populationFactory, localOptimizer);


	delete pyramid;
	delete populationFactory;
	delete localOptimizer;
	delete problem;
	delete solutionFactory;
	return 0;
}
