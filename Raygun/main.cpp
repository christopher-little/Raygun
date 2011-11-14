#include <iostream>

#include <QtGui>

#include "RayTracer.h"
#include "ImageBuffer.h"
#include "ImageIO.h"
#include "GUI/mainwindow.h"

using namespace std;


int main(int argc, char **argv)
{
	int w = 640;
	int h = 360;
	RayTracer rt;
	ImageBuffer *buf = new ImageBuffer(w,h);

	// Create the application and main window
	QApplication app(argc, argv);
	QWidget window;

	// Add a quit button
	QPushButton *quitbtn = new QPushButton("Quit");
	QObject::connect(quitbtn, SIGNAL(clicked()), qApp, SLOT(quit()));

	// Give the quit button a widget window to sit in
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(quitbtn);
	window.setLayout(layout);
	window.move(QPoint(300,300));
	window.show();


	// Ray trace an image
	rt.render(buf);

	// Write the image to file
	QImage imageBuf(buf->getUCharArray(), buf->width(), buf->height(), QImage::Format_RGB32);
	imageBuf.save("image.png");

	// Create a pixmap from the image, draw it to a graphics scene, then display the scene using the graphics view
	QPixmap imageMap;
	if(!imageMap.convertFromImage(imageBuf))
	{
		cout << "Error: Could not convert imagebuffer QImage to QPixmap." << endl;
	}
	QGraphicsScene *scene = new QGraphicsScene();
	scene->addPixmap(imageMap);
	QGraphicsView *sceneView = new QGraphicsView(scene);
	//sceneView.move(window.pos()+=QPoint(200,0));
	//sceneView.show();

	delete buf;




	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();




	return app.exec();
}
