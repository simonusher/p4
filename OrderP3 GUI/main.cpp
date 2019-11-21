#include "OrderP3GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OrderP3GUI w;
	w.show();
	return a.exec();
}
