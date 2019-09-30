#include "pch.h"
#include "../order-p3/include/order-p3/problem/deceptive_ordering/AbsoluteOrderingProblem.h"
#include <numeric>

TEST(AbsoluteOrderingProblemTests, GivenProblemWith8FunctionsWhenEvaluatingCorrectSolutionTheResultShouldBe32) {
	AbsoluteOrderingProblem problem(8);
	std::vector<int> solution(problem.getProblemSize());
	std::iota(solution.begin(), solution.end(), 0);
	double result = problem.evaluate(solution);
	EXPECT_EQ(1, result);
}

TEST(AbsoluteOrderingProblemTests, FitnessShouldBeCalculatedCorrectlyForIncorrectVectors) {
	AbsoluteOrderingProblem problem(2);
	std::vector<int> solution { 0, 2, 1, 3, 7, 5, 6, 4 };
	double result = problem.evaluate(solution);
	EXPECT_EQ(0.45, result);
}

TEST(AbsoluteOrderingProblemTests, FitnessShouldBeCalculatedCorrectlyForExampleFromPaper) {
	AbsoluteOrderingProblem problem(5);
	std::vector<int> solution{ 0, 1, 10, 13, 4, 5, 6, 7, 8, 9, 2, 11, 3, 12, 14, 15, 19, 17, 18, 16 };
	
	double result = problem.evaluate(solution);
	EXPECT_DOUBLE_EQ(0.34, result);
}