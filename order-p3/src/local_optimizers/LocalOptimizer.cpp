#include "../../include/order-p3/local_optimizers/LocalOptimizer.h"

LocalOptimizer::LocalOptimizer(Problem* problem): problem(problem) {}

void LocalOptimizer::optimizeLocally(Solution* solution) {
	return optimizeLocally(*solution);
}
