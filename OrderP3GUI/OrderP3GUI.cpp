#include "OrderP3GUI.h"
#include <QFileDialog>
#include <QTime>
#include <QChartView>
#include <filesystem>
#include <QMessageBox>
#include "Utils.h"
using namespace utils;

OrderP3GUI::OrderP3GUI(QWidget* parent) :
	QMainWindow(parent),
	elapsedTimer(nullptr),
	problem(nullptr),
	problemLoaded(false),
	running(false)
{
	qRegisterMetaType<IterationData>();
	qRegisterMetaType<FinalSolutionData>();
	ui.setupUi(this);
	initializeChart();
	running = false;
	problemLoaded = false;
}

void OrderP3GUI::onStartButtonClicked()
{
	if (!problemLoaded) {
		QMessageBox::information(this, tr("Error"), tr("Please select a problem definition file first."));
		return;
	}
	int selectedTimeInSeconds = getSelectedTimeInSeconds();
	if (selectedTimeInSeconds <= 0) {
		QMessageBox::information(this, tr("Error"), tr("Specified running time is incorrect"));
		return;
	}
	if (!running)
	{
		running = true;
		resetChart();
		problem->reset();
		executionTimeInSeconds = selectedTimeInSeconds;
		worker = new WorkerThread(problem, selectedTimeInSeconds);
		connect(worker, &WorkerThread::newBestFound, this, &OrderP3GUI::updateBestSolutionData);
		connect(worker, &WorkerThread::iterationPassed, this, &OrderP3GUI::updateIterationData);
		connect(worker, &WorkerThread::lastBestSolution, this, &OrderP3GUI::updateFinalSolutionData);
		connect(worker, &WorkerThread::finished, this, &OrderP3GUI::stopExecution);
		elapsedTimer = new QTimer(this);
		startTime = std::chrono::steady_clock::now();
		elapsedTimer->start(1000);
		connect(elapsedTimer, &QTimer::timeout, this, &OrderP3GUI::updateTimer);
		worker->start();
		ui.bt_start->setEnabled(false);
		ui.bt_stop->setEnabled(true);
		ui.bt_select_file->setEnabled(false);
		ui.bt_save_best_solution->setEnabled(false);
		ui.time_passed_progress_bar->setEnabled(true);
	}
}

void OrderP3GUI::onStopButtonClicked()
{
	stopExecution();
}

void OrderP3GUI::stopExecution() {
	if (running)
	{
		worker->requestInterruption();
		worker->wait();
		delete worker;
		worker = nullptr;
		this->running = false;
		elapsedTimer->stop();
		delete elapsedTimer;
		ui.bt_start->setEnabled(true);
		ui.bt_stop->setEnabled(false);
		ui.bt_select_file->setEnabled(true);
		ui.time_passed_progress_bar->setEnabled(false);
		ui.time_passed_progress_bar->setValue(100);
	}
}

void OrderP3GUI::onSelectFileButtonClicked() {
	QString selectedFilePath = QFileDialog::getOpenFileName(this, tr("Select problem definition file"), "", tr("Problem definition file (*.txt)"));
	if (!selectedFilePath.isEmpty()) {
		loadSelectedFile(selectedFilePath);
	}
}

void OrderP3GUI::saveBestInSelectedLocation(const QString& saveFilePath) {
	QFile file(saveFilePath);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("File couldn't be loaded"), file.errorString());
	}
	else {
		QTextStream fileStream(&file);
		fileStream << "Problem: \n\t";
		fileStream << ui.lb_loaded_file_value->text() << "\n";
		fileStream << "Best found solution: \n\t";
		fileStream << finalSolutionData.phenotype;
		fileStream << "\n";
		fileStream << "Solution fitness: \n\t";
		fileStream << finalSolutionData.solutionFitness;
		QMessageBox::information(this, tr("Success"), tr("Saved successfully"));
	}
}

void OrderP3GUI::onSaveFileButtonClicked() {
	QString selectedFilePath = QFileDialog::getSaveFileName(this, tr("Select where to save best solution"), "", tr("Text files (*.txt);;All files(*)"));
	if (!selectedFilePath.isEmpty()) {
		saveBestInSelectedLocation(selectedFilePath);
	}
}

void OrderP3GUI::loadSelectedFile(const QString& filePath) {
	FlowshopSchedulingProblem* problem = new FlowshopSchedulingProblem();
	bool readCorrectly = problem->readFromFile(filePath.toStdString());
	if (readCorrectly) {
		delete this->problem;
		this->problem = problem;
		problemLoaded = true;
		std::string fileName = std::filesystem::path(filePath.toStdString()).filename().string();
		ui.lb_loaded_file_value->setText(QString::fromStdString(fileName));
	}
	else {
		delete problem;
	}
}

void OrderP3GUI::updateTimer() {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = now - startTime;
	int seconds = static_cast<int>(floor(elapsedSeconds.count()));
	QTime time(0, 0);
	time = time.addSecs(seconds);
	ui.lb_elapsed_time_value->setText(time.toString("mm:ss"));
	double progress = static_cast<double>(seconds) / executionTimeInSeconds;
	ui.time_passed_progress_bar->setValue(static_cast<int>(progress * 100));
}

int OrderP3GUI::getSelectedTimeInSeconds() {
	QTime selectedTime = ui.timeEdit->time();
	int timeInSeconds = selectedTime.hour() * 3600 + selectedTime.minute() * 60 + selectedTime.second();
	return timeInSeconds;
}

void OrderP3GUI::updateBestSolutionData(BestSolutionData* bestSolutionData) {
	ui.lb_best_solution_value->setText(QString::number(bestSolutionData->solutionFitness));
	ui.lb_ffe_value->setText(QString::number(bestSolutionData->fitnessFunctionEvaluationsPassedWhenFound));
	int seconds = static_cast<int>(floor(bestSolutionData->elapsedTimeInSeconds));
	QTime time(0, 0);
	time = time.addSecs(seconds);
	ui.lb_time_passed_value->setText(time.toString("mm:ss"));
	if (!minFitnessSet) {
		minFitnessSet = true;
		yAxis->setMax(bestSolutionData->solutionFitness + 100);
		yAxis->setMin(bestSolutionData->solutionFitness);
	}
}

void OrderP3GUI::updateIterationData(const IterationData& iterationData) {
	chartSeries->append(iterationData.iterationNumber, iterationData.bestFitness);
	xAxis->setMax(iterationData.iterationNumber);
	yAxis->setMax(iterationData.bestFitness + 100);
}

void OrderP3GUI::updateFinalSolutionData(FinalSolutionData finalSolutionData) {
	this->finalSolutionData = finalSolutionData;
	ui.bt_save_best_solution->setEnabled(true);
}

void OrderP3GUI::initializeChart() {
	chart = new QtCharts::QChart();
	chartSeries = new QtCharts::QLineSeries();
	QtCharts::QChartView* chartView = new QtCharts::QChartView(chart);
	chart->addSeries(chartSeries);
	xAxis = new QtCharts::QValueAxis;
	xAxis->setRange(0, 1000);
	xAxis->setLabelFormat("%d");
	xAxis->setTitleText("Iteration number");
	yAxis = new QtCharts::QValueAxis;
	yAxis->setRange(-1, 1);
	yAxis->setTitleText(tr("Solution fitness"));
	chart->addAxis(xAxis, Qt::AlignBottom);
	chartSeries->attachAxis(xAxis);
	chart->addAxis(yAxis, Qt::AlignLeft);
	chartSeries->attachAxis(yAxis);
	chart->legend()->hide();
	chart->setTitle("Optimization");
	ui.chart_layout->addWidget(chartView);
}

void OrderP3GUI::resetChart() {
	chartSeries->clear();
	minFitnessSet = false;
}
