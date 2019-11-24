#pragma once

#include "ui_OrderP3GUI.h"
#include <QTimer>
#include "../order-p3/include/order-p3/optimizer/P3Optimizer.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "WorkerThread.h"

class OrderP3GUI : public QMainWindow
{
	Q_OBJECT

public:
	OrderP3GUI(QWidget* parent = Q_NULLPTR);
private slots:
	void onStartButtonClicked();
	void onStopButtonClicked();
	void onSelectFileButtonClicked();
	void saveBestInSelectedLocation(const QString& saveFilePath);
	void onSaveFileButtonClicked();
	void loadSelectedFile(const QString& filePath);
	void updateTimer();
	void stopExecution();
private:
	int getSelectedTimeInSeconds();
	void updateBestSolutionData(BestSolutionData* bestSolutionData);
	void updateIterationData(const IterationData& iterationData);
	void updateFinalSolutionData(FinalSolutionData finalSolutionData);
	void initializeChart();
	void resetChart();

	Ui::OrderP3GUIClass ui;
	QtCharts::QChart* chart;
	QtCharts::QLineSeries* chartSeries;
	QtCharts::QValueAxis* xAxis;
	QtCharts::QValueAxis* yAxis;
	bool minFitnessSet = false;
	QTimer* elapsedTimer;
	std::chrono::steady_clock::time_point startTime;
	Problem* problem;
	bool problemLoaded;
	bool running;
	int executionTimeInSeconds;
	WorkerThread* worker;
	FinalSolutionData finalSolutionData;
};
