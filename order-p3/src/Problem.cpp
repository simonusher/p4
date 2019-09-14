#include "../include/order-p3/Problem.h"

double Problem::evaluate(Solution &solution) {
	return evaluate(solution.getPhenotype());
}
