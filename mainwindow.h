#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "qcustomplot.h"
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
	~MainWindow();
  
private slots:
	void titleDoubleClick(QMouseEvent *event);
	void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
	void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
	void selectionChanged();
	void mousePress();
	void mouseWheel();
	void addRandomGraph();
	void removeSelectedGraph();
	void removeAllGraphs();
	void contextMenuRequest(QPoint pos);
	void moveLegend();
	void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
  
private:
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
};

#endif // MAINWINDOW_H
