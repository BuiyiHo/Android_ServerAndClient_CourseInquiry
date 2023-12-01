#ifndef FORM_ADMIN_H
#define FORM_ADMIN_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class Form_Admin;
}

class Form_Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Admin(QWidget *parent = nullptr);
    ~Form_Admin();
    void courseQuery();
    void userQuery();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::Form_Admin *ui;
};

#endif // FORM_ADMIN_H
