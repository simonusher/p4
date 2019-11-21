#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OrderP3GUI.h"
#include <random>
#include "QThread"
#include "QDebug"
#include "../order-p3/include/order-p3/optimizer/P3Optimizer.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"

class WorkerThread : public QThread
{
    Q_OBJECT
	~WorkerThread()
    {
    	delete optimizer;
	    qDebug() << "done";
    }
    void run() override {
        FlowshopSchedulingProblem problem;
		problem.initializeProblem(31);
    	int maxffe = 256896400;
    	optimizer = P3Optimizer::createOptimizerWithFfeConstraint(&problem, 
			[&](BestSolutionFoundData* bestSolutionData){ emit newBestFound(bestSolutionData); },
			maxffe);
        while (!isInterruptionRequested() && !optimizer->finished()) {
	        optimizer->runIteration();
        }
        emit resultReady(optimizer->getLastFoundBestData());
    }
signals:
    void resultReady(BestSolutionFoundData* bestSolutionData);
	void newBestFound(BestSolutionFoundData* newBestSolutionData);
private:
	P3Optimizer* optimizer;
};

class OrderP3GUI : public QMainWindow
{
	Q_OBJECT

public:
	OrderP3GUI(QWidget *parent = Q_NULLPTR);
	P3Optimizer* optimizer;
private slots:
	void onStartButtonClicked();
	void onStopButtonClicked();
private:
	Ui::OrderP3GUIClass ui;
	bool running;
	void updateTextField(BestSolutionFoundData* bestSolutionData);
	WorkerThread* worker;
};
