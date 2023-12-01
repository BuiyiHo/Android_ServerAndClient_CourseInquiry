#include "form1.h"
#include "ui_form1.h"
#include "MainWindow.h"

Form1::Form1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form1)
{
    ui->setupUi(this);
    parent->hide();

}

Form1::~Form1()
{
    delete ui;
}
