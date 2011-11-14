#include <iostream>

#include <QtGui>

#include "RayTracer.h"
#include "ImageBuffer.h"
#include "ImageIO.h"

using namespace std;


int main(int argc, char **argv)
{
	int w = 640;
	int h = 360;
	RayTracer rt;
	ImageBuffer *buf = new ImageBuffer(w,h);

	QApplication app(argc, argv);
	QWidget window;

	QGraphicsScene scene;
	QGraphicsView view(&scene);
	QPixmap pixmap("../Skyrim.jpeg");
	if(pixmap.isNull())
		cout << "Image didn't load!" << endl;
	QGraphicsPixmapItem pixitem(pixmap);
	scene.addItem(&pixitem);
	view.show();


	QPushButton *btn = new QPushButton("Quit");
	QObject::connect(btn, SIGNAL(clicked()), qApp, SLOT(quit()));

	QVBoxLayout layout;
	layout.addWidget(btn);
	window.setLayout(&layout);
	window.show();

	rt.render(buf);
	writeJPG("test.jpg", buf);

	return app.exec();
}
