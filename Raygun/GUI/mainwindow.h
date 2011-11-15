#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ImageBuffer.h"
#include "rtthread.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private slots:
	void displayImage();

	void open();
	void save();

protected:
	void closeEvent(QCloseEvent *event);

private:
	QMenu *fileMenu;

	QAction *openAction;
	QAction *saveAction;
	QAction *quitAction;

	QWidget *mainWidget;
	QTextEdit *textEdit;
	QPushButton *renderbtn;

	ImageBuffer *image;
	RTThread *rtthread;
};

#endif // MAINWINDOW_H
