#include "ExperimentTask.h"
#include <utility>
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/local_optimizers/SwapHillClimber.h"


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	os << "[ ";
	for (int i = 0; i < vec.size(); i++) {
		os << vec[i];
		if (i != vec.size() - 1) {
			os << ", ";
		}
	}
	os << " ]";
	return os;
}

ExperimentTask::ExperimentTask(int flowshopIndex, bool useRescaling, bool useReencoding, int numberOfRuns,
	int ffeBudget, const std::string& outputPath, double knownBestSolutionFitness):
	flowshopIndex(flowshopIndex),
	useRescaling(useRescaling),
	useReencoding(useReencoding),
	numberOfRuns(numberOfRuns),
	ffeBudget(ffeBudget),
	outputPath(outputPath),
	knownBestSolutionFitness(knownBestSolutionFitness)
{}

void ExperimentTask::execute() {
	for(int i = 0; i < numberOfRuns; i++) {
		runExperiment(i);
	}
	writeSummary();
}

int ExperimentTask::getFlowshopIndex() const { return flowshopIndex; }

void ExperimentTask::runExperiment(int experimentNumber) {
	FlowshopSchedulingProblem problem;
	problem.initializeProblem(flowshopIndex);
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 randomGenerator(seed);

	std::ofstream experimentFile(outputPath + std::to_string(experimentNumber) + ".csv");
	experimentFile << "seed: " << seed << std::endl;
	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	NullOptimizer optimizer(problem);
	RandomKeyEncoder encoder(0, 1, problem.getProblemSize(), randomGenerator);
	RandomKeyDecoder decoder;
	SolutionFactory factoryImpl(encoder, decoder);
	RandomRescalingOptimalMixer mixerImpl(problem, 0.1, 0, 1, randomGenerator);
	PopulationFactory popFactoryImpl(problem, mixerImpl, randomGenerator);

	Pyramid pyramid(problem, factoryImpl, popFactoryImpl, optimizer, [&](Solution* solution) {
		if(problem.getFitnessFunctionEvaluations() < ffeBudget) {
			best_fitness = solution->getFitness();
			ffeFound = problem.getFitnessFunctionEvaluations();
			experimentFile << ffeFound << ";" << best_fitness << ";" << *solution->getPhenotypePtr() << std::endl;
		}
	});

	int i = 0;
	while (problem.getFitnessFunctionEvaluations() < ffeBudget && pyramid.getBestFitness() != knownBestSolutionFitness) {
		i++;
		pyramid.runSingleIteration();
	}
	bestSolutionsWithFfeFoundInRuns.emplace_back(ffeFound, best_fitness);
}

void ExperimentTask::writeSummary() {
	std::ofstream summaryFile(outputPath + "summary.csv");
	for (std::pair<int, int>& bestSolutionsWithFfeFoundInRun : bestSolutionsWithFfeFoundInRuns) {
		summaryFile << bestSolutionsWithFfeFoundInRun.first << ";" << bestSolutionsWithFfeFoundInRun.second << std::endl;
	}
}
