#include "basicqtapp.h"
#include "ui_basicqtapp.h"

#include "libcppprofiler/src/cppprofiler.h"

#include "styles/widgetgallery.h"
#include "tree/tree.h"
#include "QtQuick2/qtquick2applicationviewer.h"
#include <qdesktopwidget.h>
#include <qmessagebox.h>
#include <QDir>

#include "qglgui/glgui.h"
using namespace QGL;

extern std::shared_ptr<GlGui> gui;

BasicQtApp::BasicQtApp(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BasicQtApp),
	mCurrent(nullptr)
{
	PROFILE_FUNCTION

	ui->setupUi(this);
	
	ui->selectedSample->addItem("None");
	ui->selectedSample->addItem("Styles");
	ui->selectedSample->addItem("Tree");
	ui->selectedSample->addItem("QtQuick2");
	
	setGeometry(QRect(0, 0, 200, 200));
	move(0, 0);
	
	//List decorations
	QDir dir("../../../windowdecorations");
	QStringList decorations = dir.entryList();
	decorations.removeFirst();
	decorations.removeFirst();
	ui->selectedDecoration->addItems(decorations);
	
	QApplication::setStyle("windows");

	on_selectedSample_currentIndexChanged("QtQuick2");
}

BasicQtApp::~BasicQtApp()
{
	PROFILE_FUNCTION

	delete ui;
	delete this;
}

void BasicQtApp::on_horizontalSlider_sliderMoved(int position)
{
	PROFILE_FUNCTION

	float amount = (float)position / (float)ui->horizontalSlider->maximum();
	setWindowOpacity(1.0 - amount);
	if (amount > 0.0) QMessageBox::critical(this, "Some shit happens", "Some shit happens");
}

void BasicQtApp::on_horizontalSlider_valueChanged(int position)
{
	PROFILE_FUNCTION
	on_horizontalSlider_sliderMoved(position);
}


void BasicQtApp::on_selectedSample_currentIndexChanged(const QString &text)
{
	delete mCurrent;
	mCurrent = nullptr;
	
	if (text == "Styles") mCurrent = new WidgetGallery;
	else if (text == "Tree") mCurrent = new tree;
	else if (text == "QtQuick2")
	{
		QtQuick2ApplicationViewer *app = new QtQuick2ApplicationViewer;
		app->setMainQmlFile("main.qml");
		app->showExpanded();
		app->setPosition(QPoint(10.0, 500.0));
		mCurrent = nullptr;
	}

	if (mCurrent)
	{
		mCurrent->show();
		QDesktopWidget desktop;
		
		QRect geometry = mCurrent->frameGeometry();
		QPoint pos(desktop.geometry().width() - geometry.width(), desktop.geometry().height() - geometry.height()) ;
		mCurrent->move(pos);
	}
}

void BasicQtApp::on_selectedDecoration_currentIndexChanged(const QString &text)
{
	gui->SetWindowTheme("../../../windowdecorations", text.toUtf8().data());
}

