#include "rtthread.h"

#include <iostream>

#include "test_scenes.h"

using namespace std;

RTThread::RTThread(ImageBuffer *i, QMutex *m, QObject *parent) : QThread(parent)
{
	image = i;
	rtLock = m;
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
	if(rtLock->tryLock())
	{
		rt->render(image);
	}
	else
	{
		cout << "RTThread could not obtain lock, there must be a render thread in progress." << endl;
	}
}
