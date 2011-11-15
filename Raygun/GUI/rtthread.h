#ifndef RTTHREAD_H
#define RTTHREAD_H

#include <QThread>
#include <QImage>

#include "ImageBuffer.h"
#include "Scene.h"
#include "RayTracer.h"

class RTThread : public QThread
{
	Q_OBJECT
public:
	explicit RTThread(ImageBuffer *i, QWidget *parent = 0);
	~RTThread();

signals:

public slots:

private:
	ImageBuffer *image;
	Scene *scene;
	RayTracer *rt;

	void run();
};

#endif // RTTHREAD_H
