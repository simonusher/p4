#include "../../include/order-p3/optimizer/PyramidSizingP3.h"
#include "../../include/order-p3/local_optimizers/NullOptimizer.h"
#include "../../include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../../include/order-p3/optimizer/PopulationFactoryImpl.h"
#include "../../include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"

PyramidSizingP3::PyramidSizingP3(Problem* problem) : problem(problem) {
	localOptimizer = new NullOptimizer(problem);
	std::random_device d;
	randomGenerator = std::mt19937(d());
	encoder = new RandomKeyEncoder(0, 1, problem->getProblemSize(), randomGenerator);
	decoder = new RandomKeyDecoder();
	solutionFactory = new SolutionFactoryImpl(*encoder, *decoder);
	solutionMixer = new RandomRescalingOptimalMixer(problem, 0.1, 0, 1, randomGenerator);
	// solutionMixer = new OptimalMixer(problem);
	populationFactory = new PopulationFactoryImpl(problem, solutionMixer, randomGenerator);

	nextSize = 5;
}

PyramidSizingP3::~PyramidSizingP3() {
	delete localOptimizer;
	delete encoder;
	delete decoder;
	delete solutionFactory;
	delete solutionMixer;
	delete populationFactory;
	for (Pyramid* pyramid : pyramids) {
		delete pyramid;
	}
}

void PyramidSizingP3::runSingleIteration() {
	runSingleIteration(0);
	checkWhichPyramidsToStop();
}

void PyramidSizingP3::runSingleIteration(int pyramidIndex) {
	ensureCapacity(pyramidIndex);
	pyramids[pyramidIndex]->runSingleIteration();
	bool bestInPyramid = checkIfBestInPyramid(pyramidIndex);
	if(bestInPyramid && pyramidIndex == 0) {
		pyramid0IterationsWithoutImprovement = 0;
	} else {
		pyramid0IterationsWithoutImprovement++;
	}
	if (pyramids[pyramidIndex]->getIterationsRun() % 4 == 0) {
		runSingleIteration(pyramidIndex + 1);
	}
}

void PyramidSizingP3::stopAllPyramidsUntil(int pyramidIndex) {
	for (int i = 0; i < pyramidIndex + 1; i++) {
		delete pyramids[i];
	}
	pyramids.erase(pyramids.begin(), pyramids.begin() + pyramidIndex + 1);
	pyramidSizes.erase(pyramidSizes.begin(), pyramidSizes.begin() + pyramidIndex + 1);
}

double PyramidSizingP3::getBestFitness() {
	if(bestSolution == nullptr) {
		return std::numeric_limits<double>::lowest();
	}
	return bestSolution->getFitness();
}

Solution* PyramidSizingP3::getBestSolution() {
	return bestSolution;
}

void PyramidSizingP3::ensureCapacity(int level) {
	if(pyramids.size() <= level) {
		pyramidSizes.push_back(nextSize);
		pyramids.push_back(new Pyramid(problem, solutionFactory, populationFactory, localOptimizer));
		iterationsWithoutImprovement.push_back(0);
		nextSize *= 2;
	}
}

bool PyramidSizingP3::checkIfBestInPyramid(int pyramidIndex) {
	if(bestSolution == nullptr || pyramids[pyramidIndex]->getBestFitness() > bestSolution->getFitness()) {
		delete bestSolution;
		bestSolution = new Solution(*pyramids[pyramidIndex]->getBestSolution());
		return true;
	} else {
		return false;
	}
}

void PyramidSizingP3::checkWhichPyramidsToStop() {
	// double mean = pyramids[0]->mean();
	// int indexToStop = -1;
	// for(int i = 1; i < pyramids.size(); i++) {
	// 	double nextMean = pyramids[i]->mean();
	// 	if(pyramids[i - 1]->getNumberOfPopulations() >= pyramidSizes[i - 1] && mean < nextMean) {
	// 		indexToStop = i - 1;
	// 	}
	// 	mean = nextMean;
	// }
	// if(indexToStop != -1) {
	// 	std::cout << "stopping " << indexToStop << std::endl;
	// 	stopAllPyramidsUntil(indexToStop);
	// }
	//
	if(pyramid0IterationsWithoutImprovement >= 1000) {
		delete pyramids[0];
		std::cout << "stopping " << 0 << std::endl;
		pyramids.erase(pyramids.begin());
		pyramidSizes.erase(pyramidSizes.begin());
		iterationsWithoutImprovement.erase(iterationsWithoutImprovement.begin());
		pyramid0IterationsWithoutImprovement = 0;
	}
	// for(int i = 0; i < pyramids.size();) {
	// 	if(iterationsWithoutImprovement[i] >= 100) {
	// 		delete pyramids[i];
	// 		std::cout << "stopping " << i << std::endl;
	// 		pyramids.erase(pyramids.begin() + i);
	// 		pyramidSizes.erase(pyramidSizes.begin() + i);
	// 		iterationsWithoutImprovement.erase(iterationsWithoutImprovement.begin() + i);
	// 	} else {
	// 		i++;
	// 	}
	// }
}
