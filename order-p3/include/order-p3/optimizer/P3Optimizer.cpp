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
	const std::function<void(BestSolutionFoundData*)>& onNewBestSolutionFound, 
	unsigned long long executionTimeInSeconds)
{
	P3Optimizer* optimizer = new P3Optimizer(*problem, onNewBestSolutionFound);
	optimizer->initializeTimeStopCondition(executionTimeInSeconds);
	return optimizer;
}

P3Optimizer* P3Optimizer::createOptimizerWithFfeConstraint(Problem* problem, const std::function<void(BestSolutionFoundData*)> & onNewBestSolutionFound, int ffeThreshold) {
	P3Optimizer* optimizer = new P3Optimizer(*problem, onNewBestSolutionFound);
	optimizer->initializeFfeStopCondition(ffeThreshold);
	return optimizer;
}

void P3Optimizer::runIteration() {
	pyramid->runSingleIteration();
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

P3Optimizer::P3Optimizer(Problem& problem, std::function<void(BestSolutionFoundData*)> onNewBestSolutionFound) :
	problem(problem),
	onNewBestSolutionFound(std::move(onNewBestSolutionFound)),
	stopCondtion(nullptr)
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

BestSolutionFoundData* P3Optimizer::getLastFoundBestData() {
	return &lastBestSolutionData;
}

BestSolutionFoundData P3Optimizer::getSolutionData(Solution* solution) {
	BestSolutionFoundData data {
		solution->getPhenotypePtr(),
		solution->getGenotypePtr(),
		solution->getFitness(),
		problem.getFitnessFunctionEvaluations(),
		getElapsedTimeInSeconds()
	};
	return data;
}

long P3Optimizer::getElapsedTimeInSeconds() {
	auto now = stdClock::now();
	auto elapsed = chrono::duration_cast<chrono::seconds>(now - startTime);
	return elapsed.count();
}
