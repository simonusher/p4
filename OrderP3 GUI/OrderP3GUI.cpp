#include "OrderP3GUI.h"
#include <QFileDialog>

OrderP3GUI::OrderP3GUI(QWidget *parent)
	: QMainWindow(parent), problem(nullptr), running(false), problemLoaded(false)
{
	ui.setupUi(this);
	running = false;
	problemLoaded = false;
}

void OrderP3GUI::onStartButtonClicked()
{
	if(!running && problemLoaded)
	{
		running = true;
		worker = new WorkerThread(problem);
		connect(worker, &WorkerThread::newBestFound, this, &OrderP3GUI::updateTextField);
		connect(worker, &WorkerThread::finished, worker, &QObject::deleteLater);
		worker->start();
		ui.bt_stop->setEnabled(false);
		ui.bt_start->setEnabled(true);
	}
}

void OrderP3GUI::onStopButtonClicked()
{
	if(running)
	{
		worker->requestInterruption();
		worker->wait();
		this->running = false;
		ui.bt_start->setEnabled(true);
		ui.bt_stop->setEnabled(false);
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

void OrderP3GUI::updateTextField(BestSolutionFoundData* bestSolutionData)
{
	ui.lb_best_solution->setText(QString::number(bestSolutionData->newSolutionFitness));
}
