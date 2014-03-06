#ifndef BASICQTAPP_H
#define BASICQTAPP_H

#include <QMainWindow>

namespace Ui {
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

private:
    Ui::BasicQtApp *ui;
};

#endif // BASICQTAPP_H
