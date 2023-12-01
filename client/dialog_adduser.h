#ifndef DIALOG_ADDUSER_H
#define DIALOG_ADDUSER_H

#include <QDialog>

namespace Ui {
class Dialog_addUser;
}

class Dialog_addUser : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_addUser(QWidget *parent = nullptr);
    ~Dialog_addUser();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_addUser *ui;
};

#endif // DIALOG_ADDUSER_H
