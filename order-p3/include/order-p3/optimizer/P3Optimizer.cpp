#include <utility>
#include "../../../include/order-p3/optimizer/P3Optimizer.h"
#include "../problem/FlowshopSchedulingProblem.h"
#include "../local_optimizers/NullOptimizer.h"
#include "solution/SolutionFactoryImpl.h"
#include "solution/RandomRescalingOptimalMixer.h"
#include "population/PopulationFactoryImpl.h"

P3Optimizer::~P3Optimizer() {
	delete randomGenerator; 
	delete localOptimizer;
	delete encoder;
	delete decoder;
	delete solutionFactory;
	delete solutionMixer;
	delete populationFactory;
	delete pyramid;
}

P3Optimizer::P3Optimizer(Problem& problem, std::function<void(const BestSolutionFoundData&)> onNewBestSolutionFound) :
	problem(problem),
	onNewBestSolutionFound(std::move(onNewBestSolutionFound))
{
	std::random_device random_device;
	randomGenerator = new std::mt19937(random_device());
	localOptimizer = new NullOptimizer(problem);
	encoder = new RandomKeyEncoder(0, 1, problem.getProblemSize(), *randomGenerator);
	decoder = new RandomKeyDecoder();
	solutionFactory = new SolutionFactoryImpl(*encoder, *decoder);
	solutionMixer = new RandomRescalingOptimalMixer(problem, DEFAULT_RESCALING_PROBABILITY, 0.0, 1.0, *randomGenerator);
	populationFactory = new PopulationFactoryImpl(problem, *solutionMixer, *randomGenerator);
	pyramid = new Pyramid(problem, *solutionFactory, *populationFactory, *localOptimizer, [&](Solution* solution) { this->updateBest(solution); });
}

void P3Optimizer::updateBest(Solution* solution) {
	BestSolutionFoundData data {
		*solution->getPhenotypePtr(),
		*solution->getGenotypePtr(),
		solution->getFitness(),
		problem.getFitnessFunctionEvaluations()
	};
	onNewBestSolutionFound(data);
}
