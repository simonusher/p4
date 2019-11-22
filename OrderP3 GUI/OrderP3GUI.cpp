#include "OrderP3GUI.h"
#include <QFileDialog>
#include <QTime>
#include <QDateTime>
#include <QChartView>

OrderP3GUI::OrderP3GUI(QWidget *parent)
	: QMainWindow(parent), problem(nullptr), running(false), problemLoaded(false), elapsedTimer(nullptr)
{
	qRegisterMetaType<IterationData>();
	ui.setupUi(this);
	running = false;
	problemLoaded = false;
	chart = new QtCharts::QChart();
	chartSeries = new QtCharts::QLineSeries();
	QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chart->addSeries(chartSeries);
	QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis;
    axisX->setRange(0, 1000);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
	QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTitleText("Audio level");
    chart->addAxis(axisX, Qt::AlignBottom);
    chartSeries->attachAxis(axisX);
    chart->addAxis(axisY, Qt::AlignLeft);
    chartSeries->attachAxis(axisY);
    chart->legend()->hide();
    chart->setTitle("Data from the microphone ()");
	ui.chart_layout->addWidget(chartView);
}

void OrderP3GUI::onStartButtonClicked()
{
	if(!running && problemLoaded)
	{
		running = true;
		worker = new WorkerThread(problem, 20);
		connect(worker, &WorkerThread::newBestFound, this, &OrderP3GUI::updateBestSolutionData);
		connect(worker, &WorkerThread::iterationPassed, this, &OrderP3GUI::updateIterationData);
		connect(worker, &WorkerThread::finished, this, &OrderP3GUI::stopExecution);
		elapsedTimer = new QTimer(this);
		startTime = std::chrono::steady_clock::now();
		elapsedTimer->start(1000);
		connect(elapsedTimer, &QTimer::timeout, this, &OrderP3GUI::updateTimer);
		worker->start();
		ui.bt_start->setEnabled(false);
		ui.bt_stop->setEnabled(true);
		ui.bt_select_file->setEnabled(false);
	}
}

void OrderP3GUI::onStopButtonClicked()
{
	stopExecution();
}

void OrderP3GUI::stopExecution() {
	if(running)
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
	}
}

void OrderP3GUI::onSelectFileButtonClicked() {
	QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik z definicj¹ problemu"), ".", tr("Pliki definicji problemu (*.txt)"));
	if(!selectedFileName.isEmpty()) {
		loadSelectedFile(selectedFileName);
	}
}

void OrderP3GUI::loadSelectedFile(const QString& fileName) {
	FlowshopSchedulingProblem* problem = new FlowshopSchedulingProblem();
	bool readCorrectly = problem->readFromFile(fileName.toStdString());
	if(readCorrectly) {
		delete this->problem;
		this->problem = problem;
		problemLoaded = true;
	} else {
		qDebug() << "failed";
		delete problem;
	}
}

void OrderP3GUI::updateTimer() {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = now - startTime;
	int seconds = static_cast<int>(floor(elapsedSeconds.count()));
	QTime time(0,0);
	time = time.addSecs(seconds);
	ui.lb_elapsed_time_value->setText(time.toString("mm:ss"));
}

void OrderP3GUI::updateBestSolutionData(BestSolutionData* bestSolutionData) {
	ui.lb_best_solution_value->setText(QString::number(bestSolutionData->solutionFitness));
	ui.lb_ffe_value->setText(QString::number(bestSolutionData->fitnessFunctionEvaluationsPassedWhenFound));
	int seconds = static_cast<int>(floor(bestSolutionData->elapsedTimeInSeconds));
	QTime time(0, 0);
	time = time.addSecs(seconds);
	ui.lb_time_passed_value->setText(time.toString("mm:ss"));
}

void OrderP3GUI::updateIterationData(const IterationData& iterationData) {
	qDebug() << iterationData.bestFitness;
	qDebug() << iterationData.ffeWhenFound;
}
