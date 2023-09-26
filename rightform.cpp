#include "rightform.h"
#include "ui_rightform.h"
#include "mainwindowTrace.h"
#include "new_traceview.h"


RightForm::RightForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightForm)
{
 ui->setupUi(this);
///// QString str=trUtf8("новая");
///// ui->cBNTRASS->addItem(str);
/// ui->cBNTRASS->addItem("1");
 QString str=trUtf8("неопр.");
 ui->cBIFF->addItem(str);
 str=trUtf8("свой");
 ui->cBIFF->addItem(str);
 str=trUtf8("чужой");
 ui->cBIFF->addItem(str);
 str=trUtf8("СКВ");
 ui->cBIFF->addItem(str);
/// QIntValidator v(0,10000,this);
/// ui->lEH->setValidator(&v);
}

RightForm::~RightForm()
{
    delete ui;
}

// изменение номера трассы
void RightForm::on_cBNTRASS_currentIndexChanged(int index)
{

  extern MainwindowTrace* pMW;

  if (pMW==0)
   return;
  if (index<0)
      return;
  QObject* pobj;

  pobj = pMW->centralWidget();
//  New_traceView* pView=(New_traceView*)pMW->centralWidget();
///  New_traceView* pView=  qobject_cast<New_traceView*>(pobj) ;
///  New_traceView* pView1 =  qobject_cast<New_traceView*>(pobj) ;
   New_traceView* pView =  dynamic_cast<New_traceView*>(pobj) ;

  if (pView !=0)
  {
///      pView->CorNTras(index);
    pView->SetCurSel(index);
 ////  pView->FillList();
  }

}
