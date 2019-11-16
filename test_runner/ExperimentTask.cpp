#include "ExperimentTask.h"
#include <utility>
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/optimizer/PopulationFactoryImpl.h"

ExperimentTask::ExperimentTask(int flowshopIndex, bool useRescaling, bool useReencoding, int numberOfRuns,
	int ffeBudget, const std::string& outputPath):
	flowshopIndex(flowshopIndex),
	useRescaling(useRescaling),
	useReencoding(useReencoding),
	numberOfRuns(numberOfRuns),
	ffeBudget(ffeBudget),
	outputPath(outputPath)
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
	while (problem.getFitnessFunctionEvaluations() < ffeBudget) {
		i++;
		pyramid.runSingleIteration();
		if (best_fitness < pyramid.getBestFitness())
		{
			best_fitness = pyramid.getBestFitness();
			ffeFound = problem.getFitnessFunctionEvaluations();
			myfile << ffeFound << ";" << best_fitness << std::endl;
		}
		
	}
	std::cout << flowshopIndex << std::endl;
}

int ExperimentTask::getFlowshopIndex() const { return flowshopIndex; }
