#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle("Raygun");

	// Create and connect all "file menu" actions
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



	mainWidget = new QWidget();
	textEdit = new QTextEdit;
	renderbtn = new QPushButton("Render");

	mainWidget->setLayout(new QVBoxLayout());
	mainWidget->layout()->addWidget(textEdit);
	mainWidget->layout()->addWidget(renderbtn);
	setCentralWidget(mainWidget);


	image = new ImageBuffer(960,540);
	rtthread = new RTThread(image);
	connect(renderbtn, SIGNAL(clicked()), rtthread, SLOT(start()));
	connect(rtthread, SIGNAL(finished()), this, SLOT(displayImage()));
}


void MainWindow::displayImage()
{
	unsigned char *buf = image->getUCharArray();
	QImage i(buf, image->width(), image->height(), QImage::Format_RGB32);
	QPixmap p;
	p.convertFromImage(i);
	delete buf;

	QGraphicsScene *scene = new QGraphicsScene();
	scene->addPixmap(p);
	QGraphicsView *view = new QGraphicsView(scene);
	connect(this, SIGNAL(destroyed()), view, SLOT(close()));
	view->move(frameGeometry().topRight() + QPoint(100,0));
	view->show();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	// Make sure the app quits when this main window is closed
	qApp->quit();
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
