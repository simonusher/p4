#include <utility>
#include "../../../include/order-p3/optimizer/P3Optimizer.h"
#include "../problem/FlowshopSchedulingProblem.h"
#include "../local_optimizers/NullOptimizer.h"
#include "solution/SolutionFactoryImpl.h"
#include "solution/RandomRescalingOptimalMixer.h"
#include "population/PopulationFactoryImpl.h"

void P3Optimizer::initializeTimeStopCondition(unsigned long long timeInSeconds) {
	startTime = stdClock::now();
	timePoint endTime = startTime + chrono::seconds(timeInSeconds);
	stopCondtion = new TimePassedStopCondition(endTime);
}

void P3Optimizer::initializeFfeStopCondition(int fitnessFunctionEvaluationsThreshold) {
	stopCondtion = new FfeStopCondition([&]() { return problem.getFitnessFunctionEvaluations(); }, fitnessFunctionEvaluationsThreshold);
}

P3Optimizer* P3Optimizer::createOptimizerWithTimeConstraint(Problem* problem, 
	const std::function<void(BestSolutionData*)>& onNewBestSolutionFound, 
	unsigned long long executionTimeInSeconds,
	const std::function<void(const IterationData&)>& onIterationPassed)
{
	P3Optimizer* optimizer = new P3Optimizer(*problem, onNewBestSolutionFound, onIterationPassed);
	optimizer->initializeTimeStopCondition(executionTimeInSeconds);
	return optimizer;
}

P3Optimizer* P3Optimizer::createOptimizerWithFfeConstraint(Problem* problem, const std::function<void(BestSolutionData*)> & onNewBestSolutionFound, int ffeThreshold,
	const std::function<void(const IterationData&)>& onIterationPassed) {
	P3Optimizer* optimizer = new P3Optimizer(*problem, onNewBestSolutionFound, onIterationPassed);
	optimizer->initializeFfeStopCondition(ffeThreshold);
	optimizer->startTime = stdClock::now();
	return optimizer;
}

void P3Optimizer::runIteration() {
	pyramid->runSingleIteration();
	iterationsPassed++;
	if(onIterationPassed) {
		onIterationPassed(IterationData{
			lastBestSolutionData.solutionFitness,
			lastBestSolutionData.iterationNumber
		});
	}
}

bool P3Optimizer::finished() {
	return stopCondtion->finished();
}

P3Optimizer::~P3Optimizer() {
	delete randomGenerator; 
	delete localOptimizer;
	delete encoder;
	delete decoder;
	delete solutionFactory;
	delete solutionMixer;
	delete populationFactory;
	delete pyramid;
	delete stopCondtion;
}

P3Optimizer::P3Optimizer(Problem& problem, std::function<void(BestSolutionData*)> onNewBestSolutionFound, std::function<void(const IterationData&)> onIterationPassed) :
	problem(problem),
	onNewBestSolutionFound(std::move(onNewBestSolutionFound)),
	stopCondtion(nullptr),
	onIterationPassed(std::move(onIterationPassed))
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
	lastBestSolutionData = getSolutionData(solution);
	onNewBestSolutionFound(&lastBestSolutionData);
}

FinalSolutionData P3Optimizer::getLastFoundBestData() {
	return FinalSolutionData {
		pyramid->getBestSolution()->getGenotype(),
		pyramid->getBestSolution()->getPhenotype(),
		pyramid->getBestSolution()->getFitness(),
		lastBestSolutionData.fitnessFunctionEvaluationsPassedWhenFound,
		lastBestSolutionData.elapsedTimeInSeconds,
		lastBestSolutionData.iterationNumber
	};
}

BestSolutionData P3Optimizer::getSolutionData(Solution* solution) {
	BestSolutionData data {
		solution->getFitness(),
		problem.getFitnessFunctionEvaluations(),
		getElapsedTimeInSeconds(),
		iterationsPassed
	};
	return data;
}

double P3Optimizer::getElapsedTimeInSeconds() {
	auto now = stdClock::now();
	chrono::duration<double> elapsed = now - startTime;
	return elapsed.count();
}
