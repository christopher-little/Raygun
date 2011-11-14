#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	openAction = new QAction("&Open", this);
	saveAction = new QAction("&Save", this);
	quitAction = new QAction("E&xit", this);

	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);

	textEdit = new QTextEdit;
	setCentralWidget(textEdit);

	setWindowTitle("Raygun");
}

void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt);;C++ Files (*.cpp *.h)");

	if(fileName != "")
	{
		QFile file(fileName);
		if(!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::critical(this, "Error", "Could not open file.");
			return;
		}
		QTextStream in(&file);
		textEdit->setText(in.readAll());
		file.close();
	}
}

void MainWindow::save()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;C++ Files (*.cpp *.h)");

	if(fileName != "")
	{
		QFile file(fileName);
		if(!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::critical(this, "Error", "Could not write file.");
			return;
		}
		else
		{
			QTextStream stream(&file);
			stream << textEdit->toPlainText();
			stream.flush();
			file.close();
		}
	}
}
