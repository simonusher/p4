#include "../../include/order-p3/local_optimizers/HillClimber.h"

HillClimber::HillClimber(Problem* problem): problem(problem) {}

void HillClimber::hillClimb(Solution* solution) {
	hillClimb(*solution);
}
