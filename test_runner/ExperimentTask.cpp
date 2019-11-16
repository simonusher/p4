#include "ExperimentTask.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/optimizer/PopulationFactoryImpl.h"

ExperimentTask::ExperimentTask(int flowshopIndex, bool useRescaling, bool useReencoding,
	const std::function<bool(Problem*, Pyramid*)>& stopCondition) :
	flowshopIndex(flowshopIndex),
	useRescaling(useRescaling),
	useReencoding(useReencoding),
	stopCondition(stopCondition)
{}

void ExperimentTask::execute() {
	FlowshopSchedulingProblem problem;
	problem.initializeProblem(flowshopIndex);
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 randomGenerator(seed);

	std::ofstream myfile;
	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	NullOptimizer optimizer(&problem);
	RandomKeyEncoder encoder(0, 1, problem.getProblemSize(), randomGenerator);
	RandomKeyDecoder decoder;
	SolutionFactoryImpl factoryImpl(encoder, decoder);
	RandomRescalingOptimalMixer mixerImpl(&problem, 0.1, 0, 1, randomGenerator);
	PopulationFactoryImpl popFactoryImpl(&problem, &mixerImpl, randomGenerator);

	Pyramid pyramid(&problem, &factoryImpl, &popFactoryImpl, &optimizer);

	int i = 0;
	while (!stopCondition(problem, &finalPyramid)) {
		i++;
		finalPyramid.runSingleIteration();
		if (best_fitness < finalPyramid.getBestFitness())
		{
			best_fitness = finalPyramid.getBestFitness();
			std::cout << ffeFound << " " << best_fitness << std::endl;
			ffeFound = problem->getFitnessFunctionEvaluations();
			myfile << ffeFound << ";" << best_fitness << std::endl;
		}
	}
	delete localOptimizer;
}
