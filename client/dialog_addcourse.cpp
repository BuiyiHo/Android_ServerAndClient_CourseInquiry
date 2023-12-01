#include "dialog_addcourse.h"
#include "ui_dialog_addcourse.h"
#include "protocol.h"
#include <QTcpSocket>
Dialog_addcourse::Dialog_addcourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_addcourse)
{
    ui->setupUi(this);
}

Dialog_addcourse::~Dialog_addcourse()
{
    delete ui;
}

void Dialog_addcourse::on_buttonBox_accepted()
{
    CourseDTO courseDTO;
    courseDTO.code = ui->lineEdit->text().toStdString();
    courseDTO.title = ui->lineEdit_2->text().toStdString();
    courseDTO.section =  ui->lineEdit_3->text().toStdString();
    courseDTO.time = ui->lineEdit_4->text().toStdString();
    courseDTO.classroom = ui->lineEdit_5->text().toStdString();
    courseDTO.instructor =  ui->lineEdit_6->text().toStdString();
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="addCourse";
        msg.msgContent = xpack::json::encode(courseDTO);
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());

        if(tcpsocket->waitForBytesWritten(3333))
        {

        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;
}

