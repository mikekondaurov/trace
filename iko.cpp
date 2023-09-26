
#include <QtGui>
#include <QTextCodec>
#include <QMessageBox>
#include <QMenu>

#include "iko.h"
#include "setcontrol.h"
#include "convcoor.h"
#include "mainwindowTrace.h"
#include "new_traceview.h"
#include "maketras.h"
#include "convcoor.h"
#include "si_xh.h"
#include "proppointtr.h"
////#include "rightform.h"

extern MainwindowTrace* pMW;


IKO::IKO(QWidget *parent) :
    QWidget(parent)
{
///    setMinimumWidth(400);//0-300
///    setMinimumHeight(400);
/////    resize(500,500);
    setMinimumSize(550,550);

    DragMode = false;
   setMouseTracking(true); //
 #if 0
   setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),this,
       SLOT(slotShowContextMenu(const QPoint& pos))         );
 #endif
}

void IKO::paintEvent(QPaintEvent* event)
{
   IkoSize.setWidth(100);
   IkoSize.setHeight(100);
   IkoSize.scale(event->rect().size(), Qt::KeepAspectRatio);


    QPainter painter(this);
//////////////////    painter.begin(this);
     painter.setRenderHint(QPainter::Antialiasing);
     QImage imp_salut(":/rec/images/salut.png");
     painter.drawImage(0,0,imp_salut);

     painter.drawEllipse(0,0,IkoSize.width(),IkoSize.height());//　draw　an　ellipse
     painter.drawEllipse(QRectF(IkoSize.width()/4,IkoSize.height()/4,IkoSize.width()/2,IkoSize.height()/2));
     painter.drawLine(QPointF(IkoSize.width()/2,0),QPointF(IkoSize.width()/2,IkoSize.height()-0.5));
     // ось Х

      painter.drawLine(QPointF(0,IkoSize.height()/2),QPointF(IkoSize.width(),IkoSize.height()/2));
      if (pMW!=0) {
          New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
          DocTras* m_pDoc=pView->GetDocument();
          QVector<Tras>::iterator it=m_pDoc->m_Trackes.begin();

          //рисование целей
          QPoint center;
          center.setX((IkoSize.width()-0.5)/2);
          center.setY(IkoSize.height()/2);
          extern   SetControl scon; // управляющие параметры
          for (; it!=m_pDoc->m_Trackes.end();++it)
          {
           Tras tr=*it;
           QSize sz_pix;

           sz_pix.setHeight(IkoSize.height());
           sz_pix.setWidth(IkoSize.width());
           // рисование трассы на ИКО
           tr.Draw(painter,sz_pix,scon.dist);

          }
          if (DragMode)
          {
           int cur_sel = pView->GetCurSel();

           int num_lp= m_pDoc->m_Trackes[cur_sel-1].m_TrasPoint.size(); // номер последней точки выбранной трассы
           float fx,fy; // последняя точка выбранной трассы
           int x,y;

           DkmBToXY(m_pDoc->m_Trackes[cur_sel-1].m_TrasPoint[num_lp-1].D,m_pDoc->m_Trackes[cur_sel-1].m_TrasPoint[num_lp-1].B,
                  fx,fy);
           x = fx*IkoSize.width()/(scon.dist*2)+(float)IkoSize.width()/2;
           y =  -fy*IkoSize.height()/(scon.dist*2) + (float)IkoSize.height()/2;
           painter.drawLine(x,y,dragPoint.x(),dragPoint.y());
          }

      }
//////    painter.end();
}

void IKO::mousePressEvent(QMouseEvent *event)
{

 if (event->button() == Qt::LeftButton) {

  New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());

  float dkm,b;
  GetDB(event->pos(),dkm,b);

  int cur_sel = pView->GetCurSel();
  if (cur_sel==0) // Новая трасса
  {

   pView->AddTras(b,dkm);
   this->update();
   pView->psi_xh->update();
  }
  else
  {
   int num_point=0;  // поиск последней точки
   int rc=pView->GetDocument()->FindTras(cur_sel-1,b,dkm,2,5,num_point);
   if (rc!=-1)  // Трасса найдена
   {
       DragMode=true;
       if (cur_sel!=rc+1)
        pView->CorNTras(rc+1);
   }
  }

 }
 if (event->button() == Qt::RightButton) {
   slotShowContextMenu(event->pos());
 }
 event->accept();

}

void IKO::mouseReleaseEvent(QMouseEvent *event)
{
 if (event->button() == Qt::LeftButton) {
  if (DragMode) {
   New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
   int cur_sel = pView->GetCurSel();
   if (cur_sel!=0) //  трасса выбрана
   {
    float xkm,b;
    GetDB(event->pos(),xkm,b);
    DocTras* pDoc=pView->GetDocument();

    int rc= pDoc->m_Trackes[cur_sel-1].AddTP(b,xkm,pView->GetDefH(),pView->GetDefA(),pView->GetDefV());
    if (rc==0)
    {
     pView->FillList();
    }
    if (rc==1) QMessageBox::information(NULL,"V==0",trUtf8("Ошибка"));
    if (rc==2) QMessageBox::warning(NULL,"Совпадающие точки",trUtf8("Ошибка"));
    if (100<rc)
    {
     QString str;
     str.sprintf("Ошибочный участок %d",rc);
     QMessageBox::warning(NULL,trUtf8("Ошибка"),str);
    }

   }
   DragMode = false;
   this->update();
   pView->psi_xh->update();

  }
  event->accept();
 }
}

void IKO::mouseMoveEvent(QMouseEvent *event)
{

 if (DragMode)
 {
  dragPoint=event->pos(); // Сохраняем последнюю  точку

  this->update();


 }

 QPoint p1=event->pos();

 if ((IkoSize.width()<p1.rx()) || (IkoSize.height()<p1.ry())) // выходим за квадрат ИКО
 {
     event->accept();
     return;
 }
 float distance_km,bearing_gr;

 GetDB(p1,distance_km,bearing_gr);

    QString str;

    str.sprintf("D:%5.3f ",distance_km);
    str=str+QString(trUtf8(" км"));
    emit signalDistance(str);

    str.sprintf("B:%5.2f",bearing_gr);
    str=str+QString(trUtf8(" град."));
    emit signalBearing(str);

 float xx,yy;
 DkmBToXY(distance_km, bearing_gr,xx, yy);
    str.sprintf("X %5.3f",xx);
    str=str+QString(trUtf8(" км"));;

    emit sigXkm(str);

    str.sprintf("Y %5.3f",yy);
    str=str+QString(trUtf8(" км"));;

    emit sigYkm(str);

    event->accept();
}
// Получить дальность - пеленг
void IKO::GetDB(QPoint mp, float &distance_km, float &b)
{

  extern SetControl scon; // управляющие параметры

  QPoint center;
 ////// center.setX((width()-0.5)/2.0);
 //// center.setY(height()/2.0);
 //// IkoSize;
   center.setX((IkoSize.width()-0.5)/2.0);
   center.setY(IkoSize.height()/2.0);

  float sq;
  sq= (center.x()-mp.x())*(center.x()-mp.x())+(center.y()-mp.y())*(center.y()-mp.y());
  sq=sqrt(sq);

  distance_km = sq*scon.dist/center.x();

  int x,y;
  x=mp.x()-center.x();
  y=center.y()-mp.y();

  if ((x==0)&&(y==0))
  {
   b=-1;distance_km=0;
  }
  else
   GetBG( x,y,b,sq);
}
void IKO::slotShowContextMenu(const QPoint& pos)
{
 New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
 int cur_sel = pView->GetCurSel();
 DocTras* pDoc = pView ->GetDocument();
 float xkm,b;
 GetDB(pos,xkm,b);
 int num_point1=1;  // поиск любой точки
 int rc=pDoc->FindTras(cur_sel-1,b,
             xkm,2,5,num_point1);
 if (rc!=-1)  // Трасса найдена
 {
  num_point=num_point1;
  QMenu Ikomnu;
  Ikomnu.addAction("Удалить трассу",this,SLOT(OnDelTr()));
  Ikomnu.addAction("Точка рубежа",this,SLOT(OnRubeg()));

  Ikomnu.addAction("Удалить точку трассы",this,SLOT(OnDelTP()));
  Ikomnu.addAction("Свойства точки",this,SLOT(OnProp()));

  Ikomnu.exec(mapToGlobal(pos));
 }

}
void IKO::OnDelTr()
{
 New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
 int cur_sel = pView->GetCurSel();
 DocTras* pDoc = pView ->GetDocument();
 pDoc->m_Trackes.erase(
     pDoc->m_Trackes.begin()+cur_sel-1);

 this->update();
 pView->psi_xh->update();

 pView->CorNTras(0);  // выбираем пункт комбобокс - новая
}
void IKO::OnRubeg()
{
 New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
 int cur_sel = pView->GetCurSel();
 DocTras* pDoc = pView ->GetDocument();
 pDoc->m_Trackes[cur_sel-1].NumTR=num_point;
 update();
 pView->psi_xh->update();

}
//**********************************
// Удаление части точек трассы
//**********************************
void IKO::OnDelTP()
{
 New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
 int cur_sel = pView->GetCurSel();
 DocTras* pDoc = pView ->GetDocument();
 if (cur_sel==0) return;
 if (num_point==0)
 {
  QMessageBox::StandardButtons rc =   QMessageBox::information(this,"Удалить трассу",
                                                               "Единственная точка",QMessageBox::Ok | QMessageBox::No );
  if (rc== QMessageBox::Ok)
  {
   OnDelTr();
   pView->CorNTras(0);
  }
 }
 else
 {
  pDoc->m_Trackes[cur_sel-1].m_TrasPoint.erase(
    pDoc->m_Trackes[cur_sel-1].m_TrasPoint.begin()+num_point,
    pDoc->m_Trackes[cur_sel-1].m_TrasPoint.end());
  pDoc->m_Trackes[cur_sel-1].CheckTras();
  if (num_point<= pDoc->m_Trackes[cur_sel-1].NumTR)
   pDoc->m_Trackes[cur_sel-1].NumTR = 0;
  pView->CorNTras(cur_sel);

 }
 update();
 pView->psi_xh->update();

}
void IKO::OnProp()
{
 New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
 int cur_sel = pView->GetCurSel();
 DocTras* pDoc = pView ->GetDocument();

 Tras tr(pDoc->m_Trackes[cur_sel-1]); // Cоздаем копию выбранной трассы
 PropPointTr* pdlg = new PropPointTr(tr,num_point,this);
 if (pdlg->exec() == QDialog::Accepted) {
  pDoc->m_Trackes[cur_sel-1]=tr;
  pView->CorNTras(cur_sel);
 }
 delete pdlg;
}
