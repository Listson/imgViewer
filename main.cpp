#include <QApplication>
#include <QTextCodec>
#include "MyWidget.h"
int main(int argc, char **argv)
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QApplication *app = new QApplication(argc, argv);

	MyWidget *window = new MyWidget;
	window->show();

	return app->exec();
}
