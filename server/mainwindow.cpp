#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "xpack-1.0.1/json.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

//==================================protocol================================
struct UserDTO
{
    int id;
    std::string userName;
    std::string password;
    int role;   
    XPACK(O(id,userName, password,role));
};
struct LogDTO
{
    int id;
    std::string username;
    std::string connectTime;
    std::string ip;
    XPACK(O(id,username, connectTime,ip));
};

struct MsgDTO
{
    std::string msgType;
    std::string msgContent;
    bool res;
    XPACK(O(msgType, msgContent,res));
};
struct CourseDTO
{
    int id;
    std::string code;
    std::string title;
    std::string section;
    std::string time;
    std::string classroom;
    std::string instructor;
    XPACK(O(id, code,title, section ,time, classroom, instructor));
};
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("mysql123");
    db.setPort(3306);
    db.setUserName("root");  
    db.setPassword("123456");	
    if (!db.open())
        qDebug()<<"can't open db"<<db.lastError();
    else
        qDebug()<<"open db";
    QTcpServer* tcpserver = new QTcpServer();
    tcpserver->listen(QHostAddress::Any,9999);
    connect(tcpserver,&QTcpServer::newConnection,this,[=]()
    {
        qDebug("accept a new conn... ");
        QTcpSocket* tcpsocket=tcpserver->nextPendingConnection();
        connect(tcpsocket,&QIODevice::readyRead,this,[=]()
        {
            QString buf=tcpsocket->readAll();
            qDebug("recv: %s",buf.toStdString().c_str());

            MsgDTO msgDTO;
            xpack::json::decode(buf.toStdString(), msgDTO);    
            if(msgDTO.msgType.compare("login")==0){
                UserDTO userDTO;
                xpack::json::decode(msgDTO.msgContent, userDTO);
                QString sqlCMD= QString("select * from user where username='%1' and password='%2'")
                        .arg(userDTO.userName.c_str())
                        .arg(userDTO.password.c_str());
                QSqlQuery result = db.exec(sqlCMD);
                MsgDTO respMsg;
                respMsg.msgType="login";
                int numberOfRows = 0;
                if(result.last())
                {
                    numberOfRows =  result.at() + 1;
                    result.first();
                    result.previous();
                }
                if(numberOfRows==1){
                    respMsg.res=true;
                    while (result.next()){
                        userDTO.role=result.value("role").toInt();
                        respMsg.msgContent=xpack::json::encode(userDTO);


                        // log
                        QString ip = tcpsocket->peerAddress().toString();
                        ip = ip.mid(7);
                        QString sqlCMD_log= QString("insert log(username,connecttime,ip) value('%1',sysdate(),'%2')")
                                .arg(userDTO.userName.c_str())
                                .arg(ip);
                        db.exec(sqlCMD_log);
                    }
                }else{
                    respMsg.res=false;
                    respMsg.msgContent="用户名或密码错误";
                }
                std::string respMsgStr = xpack::json::encode(respMsg);
                tcpsocket->write(respMsgStr.c_str());
            }
            else if(msgDTO.msgType.compare("queryCourse")==0){

                QString sqlCMD= QString("select * from course");
                QSqlQuery result = db.exec(sqlCMD);

                // resp
                std::vector<CourseDTO> courseList;
                while (result.next()){
                    CourseDTO temp;
                    temp.id = result.value("id").toInt();
                    temp.code = result.value("code").toString().toStdString();
                    temp.title = result.value("title").toString().toStdString();
                    temp.classroom = result.value("classroom").toString().toStdString();
                    temp.instructor = result.value("instructor").toString().toStdString();
                    temp.section = result.value("section").toString().toStdString();
                    temp.time = result.value("time").toDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();

                    courseList.push_back(temp);
                }
                std::string courseListJson =xpack::json::encode(courseList);

                MsgDTO respMsg;
                respMsg.msgType="queryCourse";
                respMsg.msgContent=courseListJson;

                std::string respMsgStr = xpack::json::encode(respMsg);
                tcpsocket->write(respMsgStr.c_str());
            }
            else if(msgDTO.msgType.compare("modifyCourse")==0){

                CourseDTO courseDTO;
                xpack::json::decode(msgDTO.msgContent, courseDTO);

                //
                QString sqlCMD= QString("update course set code='%1',title='%2',section='%3',time='%4',classroom='%5',instructor='%6' where id=%7")
                        .arg(courseDTO.code.c_str())
                        .arg(courseDTO.title.c_str())
                        .arg(courseDTO.section.c_str())
                        .arg(courseDTO.time.c_str())
                        .arg(courseDTO.classroom.c_str())
                        .arg(courseDTO.instructor.c_str())
                        .arg(courseDTO.id);
                QSqlQuery result = db.exec(sqlCMD);
                int updatedNum = result.numRowsAffected();



            }
            else if(msgDTO.msgType.compare("addCourse")==0){
                CourseDTO courseDTO;
                xpack::json::decode(msgDTO.msgContent, courseDTO);

                //
                QString sqlCMD= QString("insert course(code,title,section,time,classroom,instructor) value('%1','%2','%3','%4','%5','%6')")
                        .arg(courseDTO.code.c_str())
                        .arg(courseDTO.title.c_str())
                        .arg(courseDTO.section.c_str())
                        .arg(courseDTO.time.c_str())
                        .arg(courseDTO.classroom.c_str())
                        .arg(courseDTO.instructor.c_str());
                QSqlQuery result = db.exec(sqlCMD);
                int updatedNum = result.numRowsAffected();



            }
            else if(msgDTO.msgType.compare("deleteCourse")==0){
                CourseDTO courseDTO;
                xpack::json::decode(msgDTO.msgContent, courseDTO);

                //
                QString sqlCMD= QString("delete from course where id=%1")
                        .arg(courseDTO.id);
                QSqlQuery result = db.exec(sqlCMD);
                int updatedNum = result.numRowsAffected();

            }
            else if(msgDTO.msgType.compare("queryUser")==0){

                QString sqlCMD= QString("select * from user");
                QSqlQuery result = db.exec(sqlCMD);
                std::vector<UserDTO> list;
                while (result.next()){
                    UserDTO temp;
                    temp.id = result.value("id").toInt();
                    temp.userName = result.value("userName").toString().toStdString();
                    temp.password = result.value("password").toString().toStdString();
                    list.push_back(temp);
                }
                MsgDTO respMsg;
                respMsg.msgType="queryUser";
                respMsg.msgContent=xpack::json::encode(list);


                std::string respMsgStr = xpack::json::encode(respMsg);
                tcpsocket->write(respMsgStr.c_str());
            }
            else if(msgDTO.msgType.compare("modifyUser")==0){

                UserDTO dto;
                xpack::json::decode(msgDTO.msgContent, dto);

                //
                QString sqlCMD= QString("update user set username='%1',password='%2' where id=%7")
                        .arg(dto.userName.c_str())
                        .arg(dto.password.c_str())
                        .arg(dto.id);
                QSqlQuery result = db.exec(sqlCMD);
                int updatedNum = result.numRowsAffected();

            }
            else if(msgDTO.msgType.compare("addUser")==0){

                // 取入参
                UserDTO dto;
                xpack::json::decode(msgDTO.msgContent, dto);


                //
                QString sqlCMD= QString("insert user(username,password,role) value('%1','%2',1)")
                        .arg(dto.userName.c_str())
                        .arg(dto.password.c_str());
                QSqlQuery result = db.exec(sqlCMD);
                int updatedNum = result.numRowsAffected();

            }
            else if(msgDTO.msgType.compare("deleteUser")==0){

                // 取入参
                UserDTO dto;
                xpack::json::decode(msgDTO.msgContent, dto);

                //
                QString sqlCMD= QString("delete from user where id=%1")
                        .arg(dto.id);
                QSqlQuery result = db.exec(sqlCMD);
                int updatedNum = result.numRowsAffected();

            }
            else if(msgDTO.msgType.compare("queryLog")==0){

                QString sqlCMD= QString("select * from log order by connecttime asc");
                QSqlQuery result = db.exec(sqlCMD);
                std::vector<LogDTO> list;
                while (result.next()){
                    LogDTO temp;
                    temp.id = result.value("id").toInt();
                    temp.username = result.value("userName").toString().toStdString();
                    temp.connectTime = result.value("connectTime").toDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
                    temp.ip = result.value("ip").toString().toStdString();
                    list.push_back(temp);
                }
                MsgDTO respMsg;
                respMsg.msgType="queryLog";
                respMsg.msgContent=xpack::json::encode(list);
                std::string respMsgStr = xpack::json::encode(respMsg);
                tcpsocket->write(respMsgStr.c_str());
            }

        });

        connect(tcpsocket,&QTcpSocket::disconnected,this,[=]()
        {
            qDebug() << "client disconnect...";
        });
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}






