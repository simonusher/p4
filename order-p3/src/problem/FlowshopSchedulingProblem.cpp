#include "../../include/order-p3/problem/FlowshopSchedulingProblem.h"
#include <iostream>

double FlowshopSchedulingProblem::evaluate(std::vector<int>& solution)
{
	ffe++;
	int i, m;

	double objectiveValue = 0.0;

	fitnessCalculationCache[0][0] = processingTimes[solution[0]][0];
	for (m = 1; m < nMachines; m++)
		fitnessCalculationCache[0][m] = fitnessCalculationCache[0][m - 1] + processingTimes[solution[0]][m];

	for (i = 1; i < nJobs; i++) {
		fitnessCalculationCache[i][0] = fitnessCalculationCache[i - 1][0] + processingTimes[solution[i]][0];
		for (m = 1; m < nMachines; m++)
			fitnessCalculationCache[i][m] = std::max(fitnessCalculationCache[i - 1][m], fitnessCalculationCache[i][m - 1]) + processingTimes[solution[i]][m];
	}

	objectiveValue = -fitnessCalculationCache[nJobs - 1][nMachines - 1];
	objectiveValue = 0;
	for (i = 0; i < nJobs; i++)
		objectiveValue += -fitnessCalculationCache[i][nMachines - 1];
	return objectiveValue;
}

bool FlowshopSchedulingProblem::readFromFile(const std::string& fileName) {
	std::ifstream infile(fileName);
	if(infile.is_open()) {
		std::string line;
		try {
			if(std::getline(infile, line)) {
				std::istringstream iss(line);
				int nJobs;
				int nMachines;
				int time;
				iss >> nJobs >> nMachines;
				for (int m = 0; m < nMachines; m++) {
					std::getline(infile, line);
					for (int j = 0; j < nJobs; j++) {
						iss >> time;
						processingTimes[j][m] = time;
					}
				}
				return true;
			} else {
				return false;
			}
		} catch (std::exception& exception) {
			std::cout << exception.what();
			return false;
		}
	} else {
		return false;
	}
	fitnessCalculationCache = std::vector<std::vector<int>>(nJobs, std::vector<int>(nMachines));
}

void FlowshopSchedulingProblem::initializeProblem(int index)
{
	problemIndex = index;
	char fn[100];

	switch (index)
	{
	case  0: break;
	default:
		switch ((index - 1) / 10) {
		case 0:
			sprintf_s(fn, "tai20_5.txt");
			problemSize = 20;
			loadTaillardFlowshop(20, 5, fn, index);
			break;
		case 1:
			sprintf_s(fn, "tai20_10.txt");
			problemSize = 20;
			loadTaillardFlowshop(20, 10, fn, index - 10);
			break;
		case 2:
			sprintf_s(fn, "tai20_20.txt");
			problemSize = 20;
			loadTaillardFlowshop(20, 20, fn, index - 20);
			break;
		case 3:
			sprintf_s(fn, "tai50_5.txt");
			problemSize = 50;
			loadTaillardFlowshop(50, 5, fn, index - 30);
			break;
		case 4:
			sprintf_s(fn, "tai50_10.txt");
			problemSize = 50;
			loadTaillardFlowshop(50, 10, fn, index - 40);
			break;
		case 5:
			sprintf_s(fn, "tai50_20.txt");
			problemSize = 50;
			loadTaillardFlowshop(50, 20, fn, index - 50);
			break;
		case 6:
			sprintf_s(fn, "tai100_5.txt");
			problemSize = 100;
			loadTaillardFlowshop(100, 5, fn, index - 60);
			break;
		case 7:
			sprintf_s(fn, "tai100_10.txt");
			problemSize = 100;
			loadTaillardFlowshop(100, 10, fn, index - 70);
			break;
		case 8:
			sprintf_s(fn, "tai100_20.txt");
			problemSize = 100;
			loadTaillardFlowshop(100, 20, fn, index - 80);
			break;
		case 9:
			sprintf_s(fn, "tai200_10.txt");
			problemSize = 200;
			loadTaillardFlowshop(200, 10, fn, index - 90);
			break;
		case 10:
			sprintf_s(fn, "tai200_20.txt");
			problemSize = 200;
			loadTaillardFlowshop(200, 20, fn, index - 100);
			break;
		case 11:
			sprintf_s(fn, "tai500_20.txt");
			problemSize = 500;
			loadTaillardFlowshop(500, 20, fn, index - 110);
			break;
		}
	}
}

void FlowshopSchedulingProblem::loadTaillardFlowshop(int J, int M, char* fileName, int instance) {
	int i, j, c, m, t;

	nJobs = J;
	nMachines = M;
	processingTimes = std::vector<std::vector<int>>(nJobs, std::vector<int>(nMachines));


	std::ifstream infile(fileName);

	std::string line;

	for (i = 1; i < instance; i++) {
		for (c = 0; c < (3 + nMachines); c++) {
			if (!std::getline(infile, line)) {
				printf("%s\n", line.c_str());
				printf("Error: Something wrong with the format of file %s", fileName);
				exit(0);
			}
		}
	}


	for (c = 0; c < 3; c++) {
		if (!std::getline(infile, line)) {
			printf("Error: Something wrong with the format of file %s", fileName);
			exit(0);
		}
	}

	for (m = 0; m < nMachines; m++) {
		std::getline(infile, line);
		std::istringstream iss(line);
		for (j = 0; j < nJobs; j++) {
			iss >> t;
			processingTimes[j][m] = t;
		}
	}
	fitnessCalculationCache = std::vector<std::vector<int>>(nJobs, std::vector<int>(nMachines));
}