#include "tree.h"
#include "ui_tree.h"

tree::tree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tree)
{
    ui->setupUi(this);
}

tree::~tree()
{
    delete ui;
}
