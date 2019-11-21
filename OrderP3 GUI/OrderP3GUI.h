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
public:
	WorkerThread(Problem* problem) : problem(problem) {
		
	}
	~WorkerThread()
    {
    	delete optimizer;
	    qDebug() << "done";
    }
    void run() override {
    	int maxffe = 256896400;
    	optimizer = P3Optimizer::createOptimizerWithFfeConstraint(problem, 
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
private:
	Problem* problem;
	bool problemLoaded;
	Ui::OrderP3GUIClass ui;
	bool running;
	void updateTextField(BestSolutionFoundData* bestSolutionData);
	WorkerThread* worker;
};
