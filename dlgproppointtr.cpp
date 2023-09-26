#include <QIntValidator>
#include <QMessageBox>
#include "dlgproppointtr.h"
#include "ui_dlgproppointtr.h"
#include "maketras.h"
#include "convcoor.h"

DlgPropPointTr::DlgPropPointTr(Tras& tr,int num_point1,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPropPointTr)
{
 ui->setupUi(this);
 tr1=&tr;
 num_point=num_point1;
 QString str,strT;
 strT=trUtf8("Свойства точки %1");
 str=strT.arg(num_point);
 setWindowTitle(str);
 ui->dSBox_B->setValue(tr1->m_TrasPoint[num_point].B);
 ui->dSBox_D->setValue(tr1->m_TrasPoint[num_point].D);
 str.sprintf("%d",tr1->m_TrasPoint[num_point].H);
/// QIntValidator* piv = new QIntValidator;
 ui->dSBox_H->setValue(tr1->m_TrasPoint[num_point].H);

 float m_x,m_y;
 DkmBToXY(tr1->m_TrasPoint[num_point].D,tr1->m_TrasPoint[num_point].B, m_x, m_y);

 ui->dSBox_X->setValue(m_x);
 ui->dSBox_Y->setValue(m_y);

 if (num_point1==0)
 {
  ui->lE_V->setHidden(true);
  ui->lE_A->setHidden(true);
 }
 else
 {
  QIntValidator* piv = new QIntValidator;
  piv->setBottom(0);
  str.sprintf("%d",tr1->m_TrasPoint[num_point].V);
  ui->lE_V->setText(str);
  ui->lE_V->setValidator(piv);

  str.sprintf("%d",tr1->m_TrasPoint[num_point].A);
  ui->lE_A->setText(str);
  ui->lE_A->setValidator(piv);
 }
#if 0
 ui->cBox_IFF->addItem(trUtf8("неопр."));
 ui->cBox_IFF->addItem(trUtf8("свой"));
 ui->cBox_IFF->addItem(trUtf8("чужой"));
 ui->cBox_IFF->addItem(trUtf8("СКВ"));
#endif

 QStringList lst;
 lst << trUtf8("неопр.") << trUtf8("свой") << trUtf8("чужой") << trUtf8("СКВ");
 ui->cBox_IFF->addItems(lst);
 ui->cBox_IFF->setCurrentIndex(tr1->type_tar);
/// m_flag_xy=false;
 ui->cB_coorXY->setChecked(false); //  setCheckState(m_flag_xy);
 SetPropCoor();
}

DlgPropPointTr::~DlgPropPointTr()
{
    delete ui;
}
// Установка ХY или BD
void DlgPropPointTr::SetPropCoor()
{
 ui->dSBox_B->setEnabled(!ui->cB_coorXY->isChecked());
 ui->dSBox_D->setEnabled(!ui->cB_coorXY->isChecked());

 ui->dSBox_X->setEnabled(ui->cB_coorXY->isChecked());
 ui->dSBox_Y->setEnabled(ui->cB_coorXY->isChecked());
 m_flag_xy=ui->cB_coorXY->isChecked();
}


void DlgPropPointTr::on_cB_coorXY_clicked()
{
 SetPropCoor();

}
// перегруженная функция
void DlgPropPointTr::accept()
{
 if (ui->cB_coorXY->isChecked())
 {
  float fx,fy,fd;
  fx=ui->dSBox_X->value();
  fy=ui->dSBox_Y->value();
  fd=sqrtf(fx*fx+fy*fy);
  if (500<fd)
  {
   QMessageBox::information(this,"Ку-ку","Дальность больше 500 км\n Думай!");

   return; // оставить диалог
  }
  GetBG(fx,fy,tr1->m_TrasPoint[num_point].B,
   tr1->m_TrasPoint[num_point].D);

 }
 else
 {
  tr1->m_TrasPoint[num_point].B=ui->dSBox_B->value();
  tr1->m_TrasPoint[num_point].D=ui->dSBox_D->value();
 }
 tr1->m_TrasPoint[num_point].A=ui->lE_A->text().toUInt();
 tr1->m_TrasPoint[num_point].V=ui->lE_V->text().toUInt();
 tr1->m_TrasPoint[num_point].H=ui->dSBox_H->value();
 tr1->type_tar=static_cast<kmb_trace::TargetType>(ui->cBox_IFF->currentIndex());
 if (tr1->CheckTras()!=0)
 {
  QMessageBox::information(this,"Ну-Ну","Неправильная трасса\n Думай!");
  return; // оставить диалог
 }

 QDialog::accept(); // принятие закрытия диалога
}
