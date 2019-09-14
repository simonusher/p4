#include "../../include/order-p3/problem/Problem.h"

double Problem::evaluate(Solution &solution) {
	return evaluate(solution.getPhenotype());
}
