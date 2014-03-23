#include "basicqtapp.h"
#include "ui_basicqtapp.h"

#include "libcppprofiler/src/cppprofiler.h"

#include "styles/widgetgallery.h"
#include <qdesktopwidget.h>

BasicQtApp::BasicQtApp(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BasicQtApp),
	mCurrent(nullptr)
{
	PROFILE_FUNCTION

	ui->setupUi(this);
	
	ui->selectedSample->addItem("None");
	ui->selectedSample->addItem("Styles");
	
	move(0, 0);
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
}

void BasicQtApp::on_selectedSample_currentIndexChanged(const QString &text)
{
	delete mCurrent;
	
	if (text == "Styles") mCurrent = new WidgetGallery;
	
	if (mCurrent)
	{
		mCurrent->show();
		QDesktopWidget desktop;
		
		QRect geometry = mCurrent->geometry();
		QPoint pos(desktop.geometry().width() - geometry.width(), desktop.geometry().height() - geometry.height()) ;
		mCurrent->move(pos);
	}
}

