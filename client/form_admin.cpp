#include "form_admin.h"
#include "ui_form_admin.h"
#include "protocol.h"
#include <QMessageBox>
#include "Dialog_addcourse.h"
#include "dialog_adduser.h"
Form_Admin::Form_Admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Admin)
{
    ui->setupUi(this);
}

Form_Admin::~Form_Admin()
{
    delete ui;
}

int currentRowIndex_course=-1;
int currentRowIndex_user=-1;
void Form_Admin::courseQuery()
{
    currentRowIndex_course=-1;
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="queryCourse";
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());
        if(tcpsocket->waitForReadyRead(3333))
        {
            QString buf=tcpsocket->readAll();
            qDebug("recv:%s",buf.toStdString().c_str());

            MsgDTO respDTO;
            xpack::json::decode(buf.toStdString(),respDTO);

            std::vector<CourseDTO> courseList;
            xpack::json::decode(respDTO.msgContent,courseList);

            qDebug("courseList size = %d",courseList.size());
            ui->qTableWidget1->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->qTableWidget1->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->qTableWidget1->setRowCount(courseList.size());
            ui->qTableWidget1->setColumnCount(7);
            connect(ui->qTableWidget1,&QTableWidget::itemClicked,this,[=](){
                currentRowIndex_course=ui->qTableWidget1->currentRow();
            });
            QStringList tHeader;
            ui->qTableWidget1->clear();	
            tHeader <<  QString("id") <<QString("code") << QString("title")<< QString("section")<< QString("time")<< QString("classroom")<< QString("instructor");
            ui->qTableWidget1->setColumnHidden(0, true);
            ui->qTableWidget1->setHorizontalHeaderLabels(tHeader);     
            ui->qTableWidget1->horizontalHeader()->setVisible(true);
            ui->qTableWidget1->verticalHeader()->setVisible(false);   
            ui->qTableWidget1->clearContents();

            for (int row = 0; row <courseList.size(); row++)
            {
                for (int column = 0; column < 7; column++)
                {
                    if (column == 0) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].id))); }
                    if (column == 1) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].code.c_str()))); }
                    if (column == 2) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].title.c_str()))); }
                    if (column == 3) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].section.c_str()))); }
                    if (column == 4) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].time.c_str()))); }
                    if (column == 5) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].classroom.c_str()))); }
                    if (column == 6) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].instructor.c_str()))); }
                }
            }
           ui->qTableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
           
        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;

}
void Form_Admin::on_pushButton_clicked()
{
    courseQuery();
}

void Form_Admin::on_pushButton_3_clicked()
{

    if(currentRowIndex_course==-1){
        QMessageBox::critical(this,"error","please select one row");
        return ;
    }
    qDebug("current row = %d",currentRowIndex_course);
    CourseDTO courseDTO;
    courseDTO.id = ui->qTableWidget1->item(currentRowIndex_course, 0)->text().toInt();
    courseDTO.code = ui->qTableWidget1->item(currentRowIndex_course, 1)->text().toStdString();
    courseDTO.title = ui->qTableWidget1->item(currentRowIndex_course, 2)->text().toStdString();
    courseDTO.section = ui->qTableWidget1->item(currentRowIndex_course, 3)->text().toStdString();
    courseDTO.time = ui->qTableWidget1->item(currentRowIndex_course, 4)->text().toStdString();
    courseDTO.classroom = ui->qTableWidget1->item(currentRowIndex_course, 5)->text().toStdString();
    courseDTO.instructor = ui->qTableWidget1->item(currentRowIndex_course, 6)->text().toStdString();

    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="modifyCourse";
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

void Form_Admin::on_pushButton_2_clicked()
{
    Dialog_addcourse* dialog = new Dialog_addcourse();
    dialog->setModal(true);
    dialog->exec();

    qDebug("modal end..");

    courseQuery();
}

void Form_Admin::on_pushButton_4_clicked()
{
    if(currentRowIndex_course==-1){
        QMessageBox::critical(this,"error","please select one row");
        return ;
    }
    qDebug("current row = %d",currentRowIndex_course);
    CourseDTO courseDTO;
    courseDTO.id = ui->qTableWidget1->item(currentRowIndex_course, 0)->text().toInt();

    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="deleteCourse";
        msg.msgContent = xpack::json::encode(courseDTO);
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());

        if(tcpsocket->waitForBytesWritten(3333))
        {
             courseQuery();
        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;
}


void Form_Admin::userQuery()
{
    currentRowIndex_user=-1;
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="queryUser";
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());

        if(tcpsocket->waitForReadyRead(3333))
        {
            QString buf=tcpsocket->readAll();
            qDebug("recv:%s",buf.toStdString().c_str());

            MsgDTO respDTO;
            xpack::json::decode(buf.toStdString(),respDTO);

            std::vector<UserDTO> userList;
            xpack::json::decode(respDTO.msgContent,userList);
            ui->qTableWidget1_2->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->qTableWidget1_2->setSelectionMode(QAbstractItemView::SingleSelection);

            ui->qTableWidget1_2->setRowCount(userList.size());
            ui->qTableWidget1_2->setColumnCount(3);
            connect(ui->qTableWidget1_2,&QTableWidget::itemClicked,this,[=](){
                currentRowIndex_user=ui->qTableWidget1_2->currentRow();
            });

            QStringList tHeader;
            ui->qTableWidget1_2->clear();	
            tHeader <<  QString("id") <<QString("username") << QString("password");
            ui->qTableWidget1_2->setColumnHidden(0, true); 
            ui->qTableWidget1_2->setHorizontalHeaderLabels(tHeader);  
            ui->qTableWidget1_2->horizontalHeader()->setVisible(true);	
            ui->qTableWidget1_2->verticalHeader()->setVisible(false);   
            ui->qTableWidget1_2->clearContents();
            for (int row = 0; row <userList.size(); row++)
            {
                for (int column = 0; column < 7; column++)
                {
                    if (column == 0) { ui->qTableWidget1_2->setItem(row, column, new QTableWidgetItem(QString("%1").arg(userList[row].id))); }
                    if (column == 1) { ui->qTableWidget1_2->setItem(row, column, new QTableWidgetItem(QString("%1").arg(userList[row].userName.c_str()))); }
                    if (column == 2) { ui->qTableWidget1_2->setItem(row, column, new QTableWidgetItem(QString("%1").arg(userList[row].password.c_str()))); }
                    }
            }
            ui->qTableWidget1_2->horizontalHeader()->setStretchLastSection(true);						

        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;

}

void Form_Admin::on_pushButton_5_clicked()
{
    userQuery();
}



void Form_Admin::on_pushButton_7_clicked()
{
    if(currentRowIndex_user==-1){
        QMessageBox::critical(this,"error","please select one row");
        return ;
    }
    UserDTO dto;
    dto.id = ui->qTableWidget1_2->item(currentRowIndex_user, 0)->text().toInt();
    dto.userName = ui->qTableWidget1_2->item(currentRowIndex_user, 1)->text().toStdString();
    dto.password = ui->qTableWidget1_2->item(currentRowIndex_user, 2)->text().toStdString();
 
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="modifyUser";
        msg.msgContent = xpack::json::encode(dto);
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

void Form_Admin::on_pushButton_6_clicked()
{
    Dialog_addUser* dialog = new Dialog_addUser();
    dialog->setModal(true);
    dialog->exec();


    userQuery();
}


void Form_Admin::on_pushButton_8_clicked()
{
    if(currentRowIndex_user==-1){
        QMessageBox::critical(this,"error","please select one row");
        return ;
    }
    UserDTO dto;
    dto.id = ui->qTableWidget1_2->item(currentRowIndex_user, 0)->text().toInt();

    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="deleteUser";
        msg.msgContent = xpack::json::encode(dto);
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());

        if(tcpsocket->waitForBytesWritten(3333))
        {
             userQuery();
        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;
}

void Form_Admin::on_pushButton_9_clicked()
{
    QTcpSocket* tcpsocket=new QTcpSocket(this);
    tcpsocket->connectToHost(QHostAddress("192.0.23.249"),9999);
    if(tcpsocket->waitForConnected(3000))
    {
        MsgDTO msg;
        msg.msgType="queryLog";
        std::string msgStr= xpack::json::encode(msg);
        tcpsocket->write(msgStr.c_str());
        if(tcpsocket->waitForReadyRead(3333))
        {
            QString buf=tcpsocket->readAll();
            qDebug("recv:%s",buf.toStdString().c_str());

            MsgDTO respDTO;
            xpack::json::decode(buf.toStdString(),respDTO);

            std::vector<LogDTO> list;
            xpack::json::decode(respDTO.msgContent,list);
            ui->qTableWidget1_3->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->qTableWidget1_3->setSelectionMode(QAbstractItemView::SingleSelection);

            ui->qTableWidget1_3->setRowCount(list.size());
            ui->qTableWidget1_3->setColumnCount(4);

            connect(ui->qTableWidget1_3,&QTableWidget::itemClicked,this,[=](){
                currentRowIndex_user=ui->qTableWidget1_3->currentRow();
            });

            QStringList tHeader;
            ui->qTableWidget1_3->clear();	
            tHeader <<  QString("id") <<QString("username") << QString("connect time")<< QString("IP");
            ui->qTableWidget1_3->setColumnHidden(0, true); 
            ui->qTableWidget1_3->setHorizontalHeaderLabels(tHeader);    
            ui->qTableWidget1_3->horizontalHeader()->setVisible(true);	
            ui->qTableWidget1_3->verticalHeader()->setVisible(false);   
            ui->qTableWidget1_3->clearContents();
            for (int row = 0; row <list.size(); row++)
            {
                for (int column = 0; column < 7; column++)
                {
                    if (column == 0) { ui->qTableWidget1_3->setItem(row, column, new QTableWidgetItem(QString("%1").arg(list[row].id))); }
                    if (column == 1) { ui->qTableWidget1_3->setItem(row, column, new QTableWidgetItem(QString("%1").arg(list[row].username.c_str()))); }
                    if (column == 2) { ui->qTableWidget1_3->setItem(row, column, new QTableWidgetItem(QString("%1").arg(list[row].connectTime.c_str()))); }
                    if (column == 3) { ui->qTableWidget1_3->setItem(row, column, new QTableWidgetItem(QString("%1").arg(list[row].ip.c_str()))); }

                }
            }
            ui->qTableWidget1_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            
        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;
}

