#ifndef DLGPROPPOINTTR_H
#define DLGPROPPOINTTR_H

#include <QDialog>

namespace Ui {
class DlgPropPointTr;
}
class Tras;
class DlgPropPointTr : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPropPointTr(Tras& tr,int num_point1,QWidget *parent = nullptr);
    ~DlgPropPointTr();
    bool GetIsUsedXY();
    bool m_flag_xy; // флаг использования координат xy
    void accept();
private slots:

    void on_cB_coorXY_clicked();


private:
    void SetPropCoor();
    Tras* tr1;
    int num_point;
    Ui::DlgPropPointTr *ui;
};

#endif // DLGPROPPOINTTR_H
