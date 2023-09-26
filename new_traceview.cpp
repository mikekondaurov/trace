#include <QLabel>
#include "new_traceview.h"
#include "iko.h"
#include "rightform.h"
#include "ui_rightform.h"
#include "si_xh.h"
#include "maketras.h"
#include "convcoor.h"

New_traceView::New_traceView(QWidget* parent)
    :QSplitter(Qt::Horizontal,parent)
{
 QSplitter* pleft;
/// QWidget* pright;
//// RightForm* pright1;
 pright1 = new RightForm;

 pleft = new QSplitter(Qt::Vertical);

 pIKO=new IKO;
//// pleft->addWidget(new IKO);
 pleft->addWidget(pIKO);
 psi_xh = new SI_XH;

 pleft->addWidget(psi_xh);

/// pright = new QWidget;


 addWidget(pleft);

 addWidget(pright1);

 m_pDocument = new DocTras;

// connect(pIKO,SIGNAL(signalAddTras(float b,float dxykm)),
///         this,SLOT(slotAddTrass(float,float)));


 connect(pIKO,SIGNAL(signalDistance(QString)),pright1->ui->label_Distance,
         SLOT(setText(QString)));
 connect(pIKO,SIGNAL(signalBearing(QString)),pright1->ui->labelB,
                     SLOT(setText(QString)));

 connect(pIKO,SIGNAL(sigXkm(QString)),pright1->ui->label_Xkm,
                     SLOT(setText(QString)));

 connect(pIKO,SIGNAL(sigYkm(QString)),pright1->ui->label_Ykm,
                     SLOT(setText(QString)));


 connect(psi_xh,SIGNAL(signalH(QString)),pright1->ui->label_H,
                     SLOT(setText(QString)));

 m_ntr_val=0; // нет выбранных трасс
 modelTrPoints = new QStandardItemModel; // модель точек трассы

}

New_traceView::~New_traceView()
{
 delete m_pDocument;
 if (modelTrPoints)
  delete modelTrPoints;
}
//****************************************
//  Изменение combo - num_pos - номер выбранной трассы (0-новая)
// заполнение таблицы точек выбранной трассы
//
void New_traceView::CorNTras(int num_pos)
{
 DocTras* m_pDoc=GetDocument();

 int num_tr =m_pDoc->m_Trackes.size(); // число трасс
 pright1->ui->cBNTRASS->clear();

 QString s;
 s=trUtf8("новая");
 pright1->ui->cBNTRASS->addItem(s);
 for (int j=0;j<num_tr;j++)
 {
   s.sprintf("%d",j+1);
   pright1->ui->cBNTRASS->addItem(s);
 }
 pright1->ui->cBNTRASS->setCurrentIndex(num_pos);
/// pright1->ui->cBNTRASS->setCurrentIndex(num_tr);


/// FillList();
}
// Заполнение таблицы точек выбранной трассы
void New_traceView::FillList()
{
 modelTrPoints->clear();

 if (m_ntr_val==0) // номер трассы, выбранной в комбо
     return;
 QStringList horizontalHeader;

 horizontalHeader <<  QString(trUtf8("B,град.")) << QString(trUtf8("D,км"))<< QString(trUtf8("X,км"))
  << QString(trUtf8("Y,км")) << QString(trUtf8("H,м"))<< QString(trUtf8("Гос.оп.")) << QString(trUtf8("V,м/с"))
  << QString(trUtf8("A, м/(ceк*ceк)"));
 modelTrPoints->setHorizontalHeaderLabels(horizontalHeader);
 Tras tr=GetDocument()->m_Trackes[m_ntr_val-1];

 for (int i=0;i<tr.m_TrasPoint.size();i++)
 {
   FillElList(i);
 }
 pright1->ui->tableViewTrass->setModel(modelTrPoints);

}
// i - порядковый номер точки
void New_traceView::FillElList(int i)
{
 QString str;

 str.sprintf("%5.2f",GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].B);
 modelTrPoints->setItem(i,0,new QStandardItem(str));

 str.sprintf("%5.3f",GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].D);
 modelTrPoints->setItem(i,1,new QStandardItem(str));
 float xx,yy;

 DkmBToXY(GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].D,
     GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].B,xx, yy);
 str.sprintf("%5.3f",xx);
 modelTrPoints->setItem(i,2,new QStandardItem(str));

 str.sprintf("%5.3f",yy);
 modelTrPoints->setItem(i,3,new QStandardItem(str));

 str.sprintf("%d",GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].H);
 modelTrPoints->setItem(i,4,new QStandardItem(str));
 kmb_trace::TargetType type_tar
       =GetDocument()->m_Trackes[m_ntr_val-1].type_tar;
 switch(type_tar) {
  case kmb_trace::Unknown:
     str=trUtf8("неопр.");
     modelTrPoints->setItem(i,5,new QStandardItem(str));
  break;
 case kmb_trace::Foe:
    str=trUtf8("чужой");
    modelTrPoints->setItem(i,5,new QStandardItem(str));
 break;
 case kmb_trace::Friend:
    str=trUtf8("свой");
    QStandardItem* psi;
    psi =  new QStandardItem(str);
    psi->setForeground(Qt::red);
    modelTrPoints->setItem(i,5,psi);
 break;
 case kmb_trace::Our_Hel:
    str=trUtf8("СКВ");
    modelTrPoints->setItem(i,5,new QStandardItem(str));
 break;

 }
 if (0<i)
 {
  str.sprintf("%d",GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].V)  ;
  modelTrPoints->setItem(i,6,new QStandardItem(str));
  str.sprintf("%d",GetDocument()->m_Trackes[m_ntr_val-1].m_TrasPoint[i].A)  ;
  modelTrPoints->setItem(i,7,new QStandardItem(str));

 }
}
void New_traceView::AddTras(float B, float D)
{
 DocTras* m_pDoc;
 m_pDoc=GetDocument();

 ///int H=pright1->ui->lEH->text().toInt();
 int H = pright1->ui->sB_H->value();

 int ival;

 ival=pright1->ui->cBIFF->currentIndex();
 switch(ival) {
  case 0:
     m_pDoc->AddTras(B,D,H,kmb_trace::Unknown);
  break;
 case 1:
    m_pDoc->AddTras(B,D,H,kmb_trace::Friend);
 break;
 case 2:
    m_pDoc->AddTras(B,D,H,kmb_trace::Foe);
 break;
 case 3:
    m_pDoc->AddTras(B,D,H,kmb_trace::Our_Hel);
 break;

 }


 CorNTras(m_pDoc->m_Trackes.size());

}

void New_traceView::SetCurSel(int cur_sel)
{
 m_ntr_val=cur_sel;  // номер трассы, выбранной
 FillList();

}
// h - по умолчанию
int New_traceView::GetDefH()
{
 return pright1->ui->sB_H->value();
}
unsigned int New_traceView::GetDefA()
{
 return pright1->ui->lE_A->text().toUInt();
}
unsigned int New_traceView::GetDefV()
{
 return pright1->ui->lE_V->text().toUInt();
}
