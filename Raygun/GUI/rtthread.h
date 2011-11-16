#ifndef RTTHREAD_H
#define RTTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>

#include "ImageBuffer.h"
#include "Scene.h"
#include "RayTracer.h"

class RTThread : public QThread
{
	Q_OBJECT
public:
	explicit RTThread(ImageBuffer *i, QMutex *m, QObject *parent = 0);
	~RTThread();

signals:

public slots:

private:
	ImageBuffer *image;	// Reference to image buffer provided by MainWindow
	Scene *scene;	// Scene object to be rendered
	RayTracer *rt;

	QMutex *rtLock;	// image buffer lock provided by MainWindow.
					// Note: this is locked by RTThread, but is not unlocked
					//	until MainWindow has finished copying the buffer for display

	void run();
};

#endif // RTTHREAD_H
