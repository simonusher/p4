#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OrderP3GUI.h"
#include <random>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "../order-p3/include/order-p3/optimizer/P3Optimizer.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

Q_DECLARE_METATYPE(IterationData)
class WorkerThread : public QThread
{
    Q_OBJECT
public:
	WorkerThread(Problem* problem, unsigned long long executionTimeInSeconds) : problem(problem), optimizer(nullptr) {
    	optimizer = P3Optimizer::createOptimizerWithTimeConstraint(problem, 
			[&](BestSolutionData* bestSolutionData){ emit newBestFound(bestSolutionData); }, executionTimeInSeconds,
							 [&](const IterationData& iterationData) { emit iterationPassed(iterationData); });
	}
	~WorkerThread()
    {
    	delete optimizer;
	    qDebug() << "done";
    }
    void run() override {
    	
        while (!isInterruptionRequested() && !optimizer->finished()) {
	        optimizer->runIteration();
        }
        emit lastBestSolution(optimizer->getLastFoundBestData());
    }
signals:
    void lastBestSolution(FinalSolutionData bestSolutionData);
	void newBestFound(BestSolutionData* newBestSolutionData);
	void iterationPassed(const IterationData& iterationData);
private:
	P3Optimizer* optimizer;
	Problem* problem;
};

class OrderP3GUI : public QMainWindow
{
	Q_OBJECT

public:
	OrderP3GUI(QWidget *parent = Q_NULLPTR);
private slots:
	void onStartButtonClicked();
	void onStopButtonClicked();
	void onSelectFileButtonClicked();
	void loadSelectedFile(const QString& fileName);
	void updateTimer();
	void stopExecution();
private:
	std::chrono::steady_clock::time_point startTime;
	QtCharts::QChart* chart;
	QtCharts::QLineSeries* chartSeries;
	
	QTimer* elapsedTimer;
	Problem* problem;
	bool problemLoaded;
	Ui::OrderP3GUIClass ui;
	bool running;
	void updateBestSolutionData(BestSolutionData* bestSolutionData);
	void updateIterationData(const IterationData& iterationData);
	WorkerThread* worker;
};
