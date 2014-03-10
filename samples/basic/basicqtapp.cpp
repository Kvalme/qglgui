#include "basicqtapp.h"
#include "ui_basicqtapp.h"

BasicQtApp::BasicQtApp(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::BasicQtApp)
{
	ui->setupUi(this);
}

BasicQtApp::~BasicQtApp()
{
	delete ui;
}

void BasicQtApp::on_horizontalSlider_sliderMoved(int position)
{
	float amount = (float)position / (float)ui->horizontalSlider->maximum() + 0.1;
	setWindowOpacity(amount);
}
