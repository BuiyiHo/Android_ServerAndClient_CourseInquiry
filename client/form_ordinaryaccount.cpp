#include "form_ordinaryaccount.h"
#include "ui_form_ordinaryaccount.h"
#include "protocol.h"
Form_OrdinaryAccount::Form_OrdinaryAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_OrdinaryAccount)
{
    ui->setupUi(this);


    queryTable();
}


void Form_OrdinaryAccount::queryTable()
{

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
            ui->qTableWidget1->setColumnCount(6);
            QStringList tHeader;
            ui->qTableWidget1->clear();
            tHeader << QString("code") << QString("title")<< QString("section")<< QString("time")<< QString("classroom")<< QString("instructor");
            ui->qTableWidget1->setHorizontalHeaderLabels(tHeader);     
            ui->qTableWidget1->horizontalHeader()->setVisible(true);	
            ui->qTableWidget1->verticalHeader()->setVisible(false);  
            ui->qTableWidget1->clearContents();
            for (int row = 0; row <courseList.size(); row++)
            {
                for (int column = 0; column < 6; column++)
                {
                    if (column == 0) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].code.c_str()))); }
                    if (column == 1) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].title.c_str()))); }
                    if (column == 2) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].section.c_str()))); }
                    if (column == 3) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].time.c_str()))); }
                    if (column == 4) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].classroom.c_str()))); }
                    if (column == 5) { ui->qTableWidget1->setItem(row, column, new QTableWidgetItem(QString("%1").arg(courseList[row].instructor.c_str()))); }
                }
            }
           ui->qTableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
           
        }
    }

    tcpsocket->disconnect();
    tcpsocket->deleteLater();
    tcpsocket=NULL;

}

Form_OrdinaryAccount::~Form_OrdinaryAccount()
{
    delete ui;
}

void Form_OrdinaryAccount::on_pushButton_clicked()
{

    queryTable();
}

