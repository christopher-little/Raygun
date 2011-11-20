#include <iostream>
#include <string>

using namespace std;

#include <QApplication>
#include <QPoint>

#include "GUI/mainwindow.h"
#include "GUI/rtthread.h"


int main(int argc, char **argv)
{
	// Check command-line flags
	bool testModeFlag = false;	// Render and display image immediately for testing purposes
	for(int i=0; i<argc; i++)
	{
		if(string(argv[i]) == string("-testmode"))
			testModeFlag = true;
	}

	// Create the application and main window
	QApplication app(argc, argv);

	// Start up the main window
	MainWindow *mainWindow = new MainWindow(testModeFlag);
	mainWindow->move(QPoint(100,150));
	mainWindow->show();

	int ret = app.exec();
	cout << "app.exec() returned: " << ret << endl;

	return 0;
}
