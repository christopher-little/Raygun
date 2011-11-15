#include "rtthread.h"

#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "ImageBuffer.h"
#include "test_scenes.h"

RTThread::RTThread(ImageBuffer *i, QWidget *parent) : QThread(parent)
{
	image = i;
	scene = testscene::transformScene();
	rt = new RayTracer(scene);
}

RTThread::~RTThread()
{
	delete scene;
	delete rt;
}


void RTThread::run()
{
	rt->render(image);
}
