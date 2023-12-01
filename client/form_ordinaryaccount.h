#ifndef FORM_ORDINARYACCOUNT_H
#define FORM_ORDINARYACCOUNT_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class Form_OrdinaryAccount;
}

class Form_OrdinaryAccount : public QWidget
{
    Q_OBJECT

public:
    explicit Form_OrdinaryAccount(QWidget *parent = nullptr);
    ~Form_OrdinaryAccount();
    void queryTable();
    void* mainForm;
private slots:
    void on_pushButton_clicked();

private:
    Ui::Form_OrdinaryAccount *ui;

};

#endif // FORM_ORDINARYACCOUNT_H
