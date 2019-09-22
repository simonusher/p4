#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyEncoder.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyDecoder.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactory.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../order-p3/include/order-p3/problem/travelling_thief/TTPProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/LocalOptimizer.h"
#include "../order-p3/include/order-p3/local_optimizers/SwapHillClimber.h"
#include "../order-p3/include/order-p3/optimizer/PopulationFactory.h"
#include "../order-p3/include/order-p3/optimizer/PopulationFactoryImpl.h"
#include "../order-p3/include/order-p3/optimizer/Pyramid.h"
#include "../order-p3/include/order-p3/optimizer/solution/OptimalMixer.h"

int main() {
	TtpProblem* problem = new TtpProblem();
	problem->initialize("hard_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	RandomKeyEncoder encoder(0, 1, problem->getProblemSize());
	RandomKeyDecoder decoder;
	SolutionFactory* solutionFactory = new SolutionFactoryImpl(encoder, decoder);
	LocalOptimizer* localOptimizer = new SwapHillClimber(problem, &encoder);
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	SolutionMixer* mixer = new OptimalMixer(problem, &decoder);
	PopulationFactory* populationFactory = new PopulationFactoryImpl(problem, mixer, randomGenerator);
	Pyramid* pyramid = new Pyramid(problem, solutionFactory, populationFactory, localOptimizer);

	for (int i = 0; i < 10000; i++) {
		pyramid->runSingleIteration();
		std::cout << pyramid->getBestFitness() << std::endl;
	}

	delete pyramid;
	delete populationFactory;
	delete localOptimizer;
	delete problem;
	delete solutionFactory;
	return 0;
}
