#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyEncoder.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyDecoder.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactory.h"
#include "../order-p3/include/order-p3/problem/travelling_thief/TTPProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/LocalOptimizer.h"
#include "../order-p3/include/order-p3/local_optimizers/SwapHillClimber.h"
#include "../order-p3/include/order-p3/optimizer/Pyramid.h"
#include "../order-p3/include/order-p3/optimizer/solution/OptimalMixer.h"
#include "../order-p3/include/order-p3/problem/deceptive_ordering/AbsoluteOrderingProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/problem/SortFunctionProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/OptimalInversionHillClimber.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include <functional>
#include "../order-p3/include/order-p3/optimizer/encoding/MaskedDecoder.h"
#include "../order-p3/include/order-p3/problem/deceptive_ordering/RelativeOrderingProblem.h"
#include <filesystem>
#include "../order-p3/include/order-p3/optimizer/solution/ReencodingMixer.h"
#include "../order-p3/include/order-p3/optimizer/encoding/MaskedEncoder.h"


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	os << "[ ";
	for (int i = 0; i < vec.size(); i++) {
		os << vec[i];
		if(i != vec.size() - 1) {
			os << ", ";
		}
	}
	os << " ],\n";
	return os;
}


void runTest(int problemIndex, Problem& problem, bool removeDuplicatesUpper, bool useLocalOptimizer, int experimentNumber,
	std::function<bool(Problem*, Pyramid*)> stopCondition, bool usePreprocessedLinkage)
{
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 randomGenerator(seed);

	std::ofstream myfile;
	myfile.open("experiments/" + std::to_string(problemIndex) + "/p" + std::to_string(problemIndex) + "d" + std::to_string(removeDuplicatesUpper) + "plo" +
		std::to_string(useLocalOptimizer) + "linkage" + std::to_string(usePreprocessedLinkage) + "_" + std::to_string(experimentNumber) + ".csv");
	myfile << "Seed: " << std::to_string(seed) << std::endl;
	myfile << "FFE found;Fitness" << std::endl;

	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	NullOptimizer optimizer(problem);
	RandomKeyEncoder encoder(0, 1, problem.getProblemSize(), randomGenerator);
	RandomKeyDecoder decoder;
	SolutionFactory factoryImpl(encoder, decoder);
	RandomRescalingOptimalMixer mixerImpl(problem, 0.1, 0, 1, randomGenerator);
	PopulationFactory popFactoryImpl(problem, mixerImpl, randomGenerator);

	LocalOptimizer* localOptimizer;
	if (useLocalOptimizer)
	{
		localOptimizer = new SwapHillClimber(problem, randomGenerator);
	}
	else {
		localOptimizer = new NullOptimizer(problem);
	}

	Pyramid finalPyramid(problem, factoryImpl, popFactoryImpl, *localOptimizer, [&](Solution* solution) -> void {
		best_fitness = solution->getFitness();
		ffeFound = problem.getFitnessFunctionEvaluations();
		std::cout << ffeFound << " " << best_fitness << std::endl;
		myfile << ffeFound << ";" << best_fitness << std::endl;
	});

	int i = 0;
	while (!stopCondition(&problem, &finalPyramid)) {
		i++;
		finalPyramid.runSingleIteration();
	}
	delete localOptimizer;
}

void run_tests() {
	int numberOfExperiments = 10;
	int budget = 220712150;
	std::function<bool(Problem*, Pyramid*)> stop_condition = [&](Problem* problem, Pyramid* pyramid) { return problem->getFitnessFunctionEvaluations() >= budget;  };

	for (int i = 31; i < 41; i++)
	{
		if (!std::filesystem::exists("experiments/" + std::to_string(i))) {
			std::filesystem::create_directory("experiments/" + std::to_string(i));
		}
		for (int j = 0; j < numberOfExperiments; j++)
		{
			std::cout << j << std::endl;
			FlowshopSchedulingProblem p;
			p.initializeProblem(i);
			runTest(i, p, false, false, j, stop_condition, false);
		}
	}
}

void test2() {
	std::random_device d;
	std::mt19937 randomGenerator(d());
	TtpProblem problem;
	problem.initialize("hard_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	RandomKeyEncoder encoder(0, 1, problem.getProblemSize(), randomGenerator);
	RandomKeyDecoder decoder;
	NullOptimizer optimizer(problem);
	RandomRescalingOptimalMixer mixer(problem, 0.1, 0, 1, randomGenerator);
	SolutionFactory solutionFactory(encoder, decoder);
	PopulationFactory populationFactory(problem, mixer, randomGenerator);
	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;
	
	Pyramid pyramid(problem, solutionFactory, populationFactory, optimizer, [&](Solution* solution) -> void {
		best_fitness = solution->getFitness();
		ffeFound = problem.getFitnessFunctionEvaluations();
		std::cout << ffeFound << " " << best_fitness << std::endl;
	});

	for (int i = 0; pyramid.getBestFitness() != 1; i++) {
		pyramid.runSingleIteration();
	}
	std::cout << pyramid.getBestSolution()->getPhenotype();
}

int main() {
	run_tests();
}
