#include <iostream>

using namespace std;

#include <QApplication>
#include <QPoint>

#include "GUI/mainwindow.h"
#include "GUI/rtthread.h"


int main(int argc, char **argv)
{
	// Create the application and main window
	QApplication app(argc, argv);

	// Start up the main window
	MainWindow *mainWindow = new MainWindow();
	mainWindow->move(QPoint(300,300));
	mainWindow->show();

	int ret = app.exec();
	cout << "app.exec() returned: " << ret << endl;

	return 0;
}
