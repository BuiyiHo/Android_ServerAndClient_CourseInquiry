#include "dialog_adduser.h"
#include "ui_dialog_adduser.h"
#include "protocol.h"
#include <QTcpSocket>
Dialog_addUser::Dialog_addUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_addUser)
{
    ui->setupUi(this);
}

Dialog_addUser::~Dialog_addUser()
{
    delete ui;
}

void Dialog_addUser::on_buttonBox_accepted()
{
    UserDTO dto;
    dto.userName = ui->lineEdit->text().toStdString();
    dto.password = ui->lineEdit_2->text().toStdString();
    // 发送
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="addUser";
        msg.msgContent = xpack::json::encode(dto);
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());

        // 阻塞
        if(tcpsocket->waitForBytesWritten(3333))
        {

        }
    }

    // 断开连接
    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;
}

