#pragma once
#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include "../order-p3/include/order-p3/optimizer/P3Optimizer.h"
Q_DECLARE_METATYPE(IterationData)
Q_DECLARE_METATYPE(FinalSolutionData)
class WorkerThread : public QThread
{
	Q_OBJECT
public:
	WorkerThread(Problem* problem, unsigned long long executionTimeInSeconds);

	~WorkerThread();

	void run() override;
signals:
	void lastBestSolution(FinalSolutionData bestSolutionData);
	void newBestFound(BestSolutionData* newBestSolutionData);
	void iterationPassed(const IterationData& iterationData);
private:
	P3Optimizer* optimizer;
};