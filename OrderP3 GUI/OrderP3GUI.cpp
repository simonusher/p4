#include "OrderP3GUI.h"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/optimizer/population/PopulationFactoryImpl.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"

OrderP3GUI::OrderP3GUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	running = false;
}

void OrderP3GUI::onStartButtonClicked()
{
	if(!running)
	{
		running = true;
		worker = new WorkerThread();
		connect(worker, &WorkerThread::newBestFound, this, &OrderP3GUI::updateTextField);
		connect(worker, &WorkerThread::finished, worker, &QObject::deleteLater);
		worker->start();
		ui.start_button->setEnabled(false);
		ui.stop_button->setEnabled(true);
	}
}

void OrderP3GUI::onStopButtonClicked()
{
	if(running)
	{
		worker->requestInterruption();
		worker->wait();
		this->running = false;
		ui.start_button->setEnabled(true);
		ui.stop_button->setEnabled(false);
	}
}

void OrderP3GUI::updateTextField(Solution* solution)
{
	ui.text_edit->setText(QString::number(solution->getFitness()));
}
