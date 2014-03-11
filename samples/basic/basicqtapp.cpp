#include "basicqtapp.h"
#include "ui_basicqtapp.h"

#include "libcppprofiler/src/cppprofiler.h"

BasicQtApp::BasicQtApp(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BasicQtApp)
{
	PROFILE_FUNCTION
	
	ui->setupUi(this);
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
	
	float amount = (float)position / (float)ui->horizontalSlider->maximum() + 0.1;
	setWindowOpacity(amount);
}
