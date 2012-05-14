#include "mainwindow.h"

#include "Colour.h"

MainWindow::MainWindow(bool testModeFlag, QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle("Raygun");

	// Create and connect all "file menu" actions
	openAction = new QAction("&Open", this);
	saveAction = new QAction("&Save", this);
	saveImageAction = new QAction("Save &Image", this);
	quitAction = new QAction("E&xit", this);

	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(saveImageAction, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveImageAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);



	mainWidget = new QWidget();
    textEdit = new QTextEdit(mainWidget);
	renderbtn = new QPushButton("Render", mainWidget);
    renderbtn->setMinimumHeight(80);

	mainWidget->setLayout(new QVBoxLayout());
	mainWidget->layout()->addWidget(textEdit);
    mainWidget->layout()->addWidget(renderbtn);
	setCentralWidget(mainWidget);

    // Set initial main window size
    resize(800,600);


	// Initialize the image buffer, ray tracer thread and display window
	image = new ImageBuffer(1280,720);
	rtLock = new QMutex();
	rtthread = new RTThread(image, rtLock);
	displayWindow = new QLabel(this, Qt::Window);
	connect(renderbtn, SIGNAL(clicked()), rtthread, SLOT(start()));
	connect(rtthread, SIGNAL(finished()), this, SLOT(displayImage()));

	// Render default scene immediately if the testFlag is set
	if(testModeFlag)
		rtthread->start();
}


void MainWindow::displayImage()
{
	// Copy the rendered image buffer to a QImage
	QImage i(image->width(), image->height(), QImage::Format_RGB32);
	for(int row=0; row<image->height(); row++) for(int col=0; col<image->width(); col++)
	{
		Colour c = image->getPixel(row,col);
		QColor qc(static_cast<int>(c.r()*255), static_cast<int>(c.g()*255), static_cast<int>(c.b()*255));
		i.setPixel(col,row,qc.rgb());
	}

	rtLock->unlock();

	// Briefly hide the window while the image is swapped
	if(displayWindow->isVisible())
		displayWindow->hide();
	// Move the window next to the main window if it hasn't been displayed yet
	else
		displayWindow->move(frameGeometry().topRight() + QPoint(100,0));
	// Assign the image to a Pixmap and display in a QLabel window
	displayWindow->setPixmap(QPixmap::fromImage(i));
	displayWindow->show();
}

void MainWindow::saveImage()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Jpeg (*.jpg *.jpeg);;PNG (*.png)");

	if(fileName != "")
	{
		rtLock->lock();
		// Copy the rendered image buffer to a QImage
		QImage i(image->width(), image->height(), QImage::Format_RGB32);
		for(int row=0; row<image->height(); row++) for(int col=0; col<image->width(); col++)
		{
			Colour c = image->getPixel(row,col);
			QColor qc(static_cast<int>(c.r()*255), static_cast<int>(c.g()*255), static_cast<int>(c.b()*255));
			i.setPixel(col,row,qc.rgb());
		}
		rtLock->unlock();

		i.save(fileName, 0, 100);
	}
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
