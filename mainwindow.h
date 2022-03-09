#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <qcustomplot.h>
#include <QtCore/QVariant>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>



class MainWindow : public QMainWindow
{
	Q_OBJECT
  
public:
	explicit MainWindow(QWidget *parent = 0);

  	void timerEvent( QTimerEvent * );
	void addRealtimeSample(double v);

private:
	void mousePress();
	void mouseWheel();
	void addRandomGraph();
	void addRealtimeGraph();
	void removeSelectedGraph();
	void removeAllGraphs();
	void moveLegend();
	void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
  
	QWidget *centralWidget;
	QVBoxLayout *verticalLayout;
	QFrame *frame_2;
	QVBoxLayout *verticalLayout_3;
	QCustomPlot *customPlot;
	QFrame *frame;
	QVBoxLayout *verticalLayout_2;
	QLabel *label;
	QMenuBar *menuBar;
	QStatusBar *statusBar;
	QSharedPointer<QCPDataContainer<QCPGraphData> > animdata;
	const int nRealtimePoints = 500;
	const double dt = 0.02;
	double t = 0;
};

#endif // MAINWINDOW_H
