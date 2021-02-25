#include "mainwindow.h"

const char instructionsTxt[] = 
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
		       "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
		       "p, li { white-space: pre-wrap; }\n"
		       "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
		       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Select the axes</span> to drag and zoom them individually.</p>\n"
		       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Double click labels</span> or legend items to set user specified strings.</p>\n"
		       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Left click</span> on graphs or legend to select graphs.</p>\n"
		       "<p style=\" m"
	"argin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Right click</span> for a popup menu to add/remove graphs and move the legend</p></body></html>";

const char titleTxt[] = "QCustomPlot Interaction and Realtime example";

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent) {
	srand(QDateTime::currentDateTime().toTime_t());

	// Layout
	centralWidget = new QWidget();
	verticalLayout = new QVBoxLayout(centralWidget);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(11, 11, 11, 11);
	frame_2 = new QFrame(centralWidget);
	frame_2->setFrameShape(QFrame::StyledPanel);
	frame_2->setFrameShadow(QFrame::Sunken);
	frame_2->setLineWidth(1);
	frame_2->setMidLineWidth(0);
	verticalLayout_3 = new QVBoxLayout(frame_2);
	verticalLayout_3->setSpacing(0);
	verticalLayout_3->setContentsMargins(0, 0, 0, 0);
	customPlot = new QCustomPlot(frame_2);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
	customPlot->setSizePolicy(sizePolicy);
	verticalLayout_3->addWidget(customPlot);
	verticalLayout->addWidget(frame_2);
	frame = new QFrame(centralWidget);
	frame->setFrameShape(QFrame::StyledPanel);
	frame->setFrameShadow(QFrame::Raised);
	verticalLayout_2 = new QVBoxLayout(frame);
	verticalLayout_2->setSpacing(6);
	verticalLayout_2->setContentsMargins(11, 11, 11, 11);
	label = new QLabel(frame);
	verticalLayout_2->addWidget(label);
	verticalLayout->addWidget(frame);
	setCentralWidget(centralWidget);
	statusBar = new QStatusBar();
	setStatusBar(statusBar);

	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
				    QCP::iSelectLegend | QCP::iSelectPlottables);
	customPlot->xAxis->setRange(-1, 8);
	customPlot->yAxis->setRange(-5, 5);
	customPlot->axisRect()->setupFullAxesBox();
  
	customPlot->plotLayout()->insertRow(0);
	QCPTextElement *title = new QCPTextElement(customPlot, titleTxt, QFont("sans", 17, QFont::Bold));
	customPlot->plotLayout()->addElement(0, 0, title);
  
	customPlot->xAxis->setLabel("x Axis");
	customPlot->yAxis->setLabel("y Axis");
	customPlot->legend->setVisible(true);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	customPlot->legend->setFont(legendFont);
	customPlot->legend->setSelectedFont(legendFont);
	customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

	addRealtimeGraph();
	addRandomGraph();
	addRandomGraph();
  
	// connect slot that ties some axis selections together (especially opposite axes):
	connect(customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
	// connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
	connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
	connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
	// make bottom and left axes transfer their ranges to top and right axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
  
	// connect some interaction slots:
	connect(customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
	connect(customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
	connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
	// connect slot that shows a message in the status bar when a graph is clicked:
	connect(customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  
	// setup policy and connect slot for context menu popup:
	customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

	setWindowTitle(titleTxt);
	label->setText(instructionsTxt);
}

void MainWindow::titleDoubleClick(QMouseEvent* event) {
	Q_UNUSED(event)
		if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
		{
			// Set the plot title by double clicking on it
			bool ok;
			QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
			if (ok)
			{
				title->setText(newTitle);
				customPlot->replot();
			}
		}
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part) {
	// Set an axis label by double clicking on it
	if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
	{
		bool ok;
		QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
		if (ok)
		{
			axis->setLabel(newLabel);
			customPlot->replot();
		}
	}
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item) {
	// Rename a graph by double clicking on its legend item
	Q_UNUSED(legend)
		if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
		{
			QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
			bool ok;
			QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
			if (ok)
			{
				plItem->plottable()->setName(newName);
				customPlot->replot();
			}
		}
}

void MainWindow::selectionChanged() {
	/*
	  normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
	  the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
	  and the axis base line together. However, the axis label shall be selectable individually.
   
	  The selection state of the left and right axes shall be synchronized as well as the state of the
	  bottom and top axes.
   
	  Further, we want to synchronize the selection of the graphs with the selection state of the respective
	  legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
	  or on its legend item.
	*/
  
	// make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
	if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
	    customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
	{
		customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
		customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
	}
	// make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
	if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
	    customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
	{
		customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
		customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
	}
  
	// synchronize selection of graphs with selection of corresponding legend items:
	for (int i=0; i<customPlot->graphCount(); ++i)
	{
		QCPGraph *graph = customPlot->graph(i);
		QCPPlottableLegendItem *item = customPlot->legend->itemWithPlottable(graph);
		if (item->selected() || graph->selected())
		{
			item->setSelected(true);
			graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
		}
	}
}

void MainWindow::mousePress() {
	// if an axis is selected, only allow the direction of that axis to be dragged
	// if no axis is selected, both directions may be dragged
  
	if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
		customPlot->axisRect()->setRangeDrag(customPlot->xAxis->orientation());
	else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
		customPlot->axisRect()->setRangeDrag(customPlot->yAxis->orientation());
	else
		customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel() {
	// if an axis is selected, only allow the direction of that axis to be zoomed
	// if no axis is selected, both directions may be zoomed
  
	if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
		customPlot->axisRect()->setRangeZoom(customPlot->xAxis->orientation());
	else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
		customPlot->axisRect()->setRangeZoom(customPlot->yAxis->orientation());
	else
		customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addRealtimeGraph() {
	customPlot->addGraph();
	customPlot->graph()->setName(QString("Realtime"));
	animdata.reset(new QCPDataContainer<QCPGraphData>);
	for(int i=0;i<nRealtimePoints;i++) {
		QCPGraphData data(i*dt,0);
		animdata->add(data);
	}
	customPlot->graph()->setData(animdata);
	QPen graphPen;
	graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
	graphPen.setWidthF(2);
	customPlot->graph()->setPen(graphPen);
	customPlot->replot();
	startTimer(40);
}

void MainWindow::addRealtimeSample(double v) {
	// shift the values
	for (auto i = animdata->end(); i != (animdata->begin()); --i) {
		i->value = (i-1)->value;
	}
	// add a new datapoint at the start
	animdata->begin()->value = v;
}

void MainWindow::timerEvent( QTimerEvent * ) {
	// demonstrates that adding a few samples before plotting speeds things up
	for(int i = 0; i < 5; i++) {
		addRealtimeSample(sin(t*5));
		t = t + dt;
	}
	customPlot->replot();
}

void MainWindow::addRandomGraph() {
	int n = 2000; // number of points in graph
	double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
	double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
	double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
	double yOffset = (rand()/(double)RAND_MAX - 0.5)*10;
	double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
	double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
	double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
	double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
	QVector<double> x(n), y(n);
	for (int i=0; i<n; i++)
	{
		x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
		y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
	}
  
	customPlot->addGraph();
	customPlot->graph()->setName(QString("New graph %1").arg(customPlot->graphCount()-1));
	customPlot->graph()->setData(x, y);
	QPen graphPen;
	graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
	graphPen.setWidthF(2);
	customPlot->graph()->setPen(graphPen);
	customPlot->replot();
}

void MainWindow::removeSelectedGraph() {
	if (customPlot->selectedGraphs().size() > 0)
	{
		customPlot->removeGraph(customPlot->selectedGraphs().first());
		customPlot->replot();
	}
}

void MainWindow::removeAllGraphs() {
	customPlot->clearGraphs();
	customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos) {
	QMenu *menu = new QMenu(this);
	menu->setAttribute(Qt::WA_DeleteOnClose);
  
	if (customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
	{
		menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
		menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
		menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
		menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
		menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
	} else  // general context menu on graphs requested
	{
		menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
		if (customPlot->selectedGraphs().size() > 0)
			menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
		if (customPlot->graphCount() > 0)
			menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
	}
  
	menu->popup(customPlot->mapToGlobal(pos));
}

void MainWindow::moveLegend() {
	if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
	{
		bool ok;
		int dataInt = contextAction->data().toInt(&ok);
		if (ok)
		{
			customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
			customPlot->replot();
		}
	}
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex) {
	// since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
	// usually it's better to first check whether interface1D() returns non-zero, and only then use it.
	double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
	QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
	statusBar->showMessage(message, 2500);
}




