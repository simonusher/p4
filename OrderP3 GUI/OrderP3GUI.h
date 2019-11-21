#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OrderP3GUI.h"
#include "../order-p3/include/order-p3/optimizer/Pyramid.h"
#include <random>
#include "QThread"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/optimizer/population/PopulationFactoryImpl.h"
#include "QDebug"

class WorkerThread : public QThread
{
    Q_OBJECT
	~WorkerThread()
    {
	    qDebug() << "done";
    }
    void run() override {
    	std::random_device d;
		std::mt19937 randomGenerator(d());
        FlowshopSchedulingProblem p;
		p.initializeProblem(31);
		NullOptimizer optimizer(p);
		RandomKeyEncoder encoder(0, 1, p.getProblemSize(), randomGenerator);
		RandomKeyDecoder decoder;
		SolutionFactoryImpl factoryImpl(encoder, decoder);
		RandomRescalingOptimalMixer mixerImpl(p, 0.1, 0, 1, randomGenerator);
		PopulationFactoryImpl popFactoryImpl(p, mixerImpl, randomGenerator);
		Pyramid pyramid(p, factoryImpl, popFactoryImpl, optimizer, [&](Solution* s) { this->newBestFound(s);});
    	
    	int maxffe = 256896400;
    	while(!isInterruptionRequested() && p.getFitnessFunctionEvaluations() < maxffe)
    	{
			pyramid.runSingleIteration();
    	}
        emit resultReady(pyramid.getBestFitness());
    }
signals:
    void resultReady(double value);
	void newBestFound(Solution* solution);
};

class OrderP3GUI : public QMainWindow
{
	Q_OBJECT

public:
	OrderP3GUI(QWidget *parent = Q_NULLPTR);
	Pyramid* pyramid;
private slots:
	void onStartButtonClicked();
	void onStopButtonClicked();
private:
	Ui::OrderP3GUIClass ui;
	bool running;
	void updateTextField(Solution* solution);
	WorkerThread* worker;
};
