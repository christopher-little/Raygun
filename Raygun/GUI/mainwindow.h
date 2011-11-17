#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ImageBuffer.h"
#include "rtthread.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(bool testModeFlag, QWidget *parent = 0);

signals:

public slots:

private slots:
	// File menu action slots
	void open();
	void save();

	// Displays the rendered image buffer to the display window (assumes rtthread has completed safely)
	void displayImage();

	void saveImage();

protected:
	void closeEvent(QCloseEvent *event);

private:
	// File menu and associate actions
	QMenu *fileMenu;

	QAction *openAction;
	QAction *saveAction;
	QAction *quitAction;

	QWidget *mainWidget;	// Main window display area
	QTextEdit *textEdit;	// Text editor (***will be used for modifying scene)
	QPushButton *renderbtn;	// Big render button

	ImageBuffer *image;	// buffer where rtthread renders image to
	QMutex *rtLock;
	RTThread *rtthread;	// Thread object to run renderer in a background thread
	QLabel *displayWindow;	// Window which opens when a rendered image is ready for display
							// Note: this widget is NOT destroyed when it is closed, thus the
							//	window can be closed by user and shown again (using show()) safely.

	QAction *saveImageAction;	// Action to save the image buffer
};

#endif // MAINWINDOW_H
