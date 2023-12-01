#ifndef DIALOG_ADDCOURSE_H
#define DIALOG_ADDCOURSE_H

#include <QDialog>

namespace Ui {
class Dialog_addcourse;
}

class Dialog_addcourse : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_addcourse(QWidget *parent = nullptr);
    ~Dialog_addcourse();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_addcourse *ui;
};

#endif // DIALOG_ADDCOURSE_H
