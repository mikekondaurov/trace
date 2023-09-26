#ifndef PROPPOINTTR_H
#define PROPPOINTTR_H

#include <QDialog>

namespace Ui {
class PropPointTr;
}
class Tras;
class PropPointTr : public QDialog
{
    Q_OBJECT

public:
    explicit PropPointTr(Tras& tr1,int num_point1,QWidget *parent = nullptr);
    ~PropPointTr();

private:
    Ui::PropPointTr *ui;
    int num_point;
    Tras* ptr;
    void accept() override;
};

#endif // PROPPOINTTR_H
