#ifndef BASICQTAPP_H
#define BASICQTAPP_H

#include <QMainWindow>

namespace Ui
{
class BasicQtApp;
}

class BasicQtApp : public QMainWindow
{
	Q_OBJECT

public:
	explicit BasicQtApp(QWidget *parent = 0);
	~BasicQtApp();

private slots:
	void on_horizontalSlider_sliderMoved(int position);
	void on_horizontalSlider_valueChanged(int position);
	void on_selectedSample_currentIndexChanged(const QString &text);
	void on_selectedDecoration_currentIndexChanged(const QString &text);

private:
	Ui::BasicQtApp *ui;
	QWidget *mCurrent;
};

#endif // BASICQTAPP_H
