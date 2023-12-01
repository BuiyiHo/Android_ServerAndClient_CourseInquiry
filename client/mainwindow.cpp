#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "protocol.h"
#include "Form_OrdinaryAccount.h"
#include "form_admin.h".h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete this->ui;
}


void MainWindow::on_pushButton_clicked()
{
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {

        UserDTO userDTO;
        userDTO.userName = ui->lineEdit->text().toStdString();
        userDTO.password = ui->lineEdit_2->text().toStdString();
        MsgDTO msg;
        msg.msgType="login";
        msg.msgContent= xpack::json::encode(userDTO);
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());

        if(tcpsocket->waitForReadyRead(-1))
        {
            QString buf=tcpsocket->readAll();
            qDebug("recv:%s",buf.toStdString().c_str());

            MsgDTO respDTO;
            xpack::json::decode(buf.toStdString().c_str(),respDTO);
            if(respDTO.res==false){
                QMessageBox::critical(this, "error",respDTO.msgContent.c_str(), QMessageBox::Yes );
            }else{
                xpack::json::decode(respDTO.msgContent,userDTO);
                if(userDTO.role==1){
                    Form_OrdinaryAccount * subform =new Form_OrdinaryAccount();
                    subform->mainForm=this;
                    subform->show();
                }
                else if(userDTO.role==2){
                    Form_Admin * subform =new Form_Admin();
                    subform->show();
                }

            }
        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;
}



