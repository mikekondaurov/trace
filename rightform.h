#ifndef RIGHTFORM_H
#define RIGHTFORM_H

#include <QWidget>

namespace Ui {
class RightForm;
}

class RightForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit RightForm(QWidget *parent = 0);
    ~RightForm();
    
////private:
    Ui::RightForm *ui;
public slots:
////   void slotMousePosIko(QPoint);

private slots:
    void on_cBNTRASS_currentIndexChanged(int index);
};

#endif // RIGHTFORM_H
