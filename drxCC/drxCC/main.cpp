#include "drxcc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	drxCC w;
	w.show();
	return a.exec();
}
