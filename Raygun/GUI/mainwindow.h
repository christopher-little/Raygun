#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private slots:
	void open();
	void save();

private:
	QMenu *fileMenu;

	QAction *openAction;
	QAction *saveAction;
	QAction *quitAction;

	QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
