#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

#include "si_xh.h"
#include "setcontrol.h"
#include "mainwindowTrace.h"
#include "new_traceview.h"
#include "maketras.h"

extern SetControl scon; // управляющие параметры
extern MainwindowTrace* pMW;


SI_XH::SI_XH(QWidget *parent) :
    QWidget(parent)
{
  setMouseTracking(true); //

}

void SI_XH::paintEvent(QPaintEvent* /*paint*/)
{
 QPainter painter;
 if (pMW!=0) {
  New_traceView* pView = dynamic_cast<New_traceView*> (pMW->centralWidget());
  DocTras* m_pDoc=pView->GetDocument();
  QVector<Tras>::iterator it=m_pDoc->m_Trackes.begin();

  painter.begin(this);
   // цикл по всем трассам
   for (; it!=m_pDoc->m_Trackes.end();++it)
   {
    QSize sz_pix;
    sz_pix.setHeight(height());
    sz_pix.setWidth(width());

    it->DrawH(painter,sz_pix) ;
   }
//
  painter.end();
 }
}
void SI_XH::mouseMoveEvent(QMouseEvent *event)
{

 QPoint point=event->pos();
 float ftmp;
 ftmp =
         ((height() -point.y()-1.0)*(float)scon.h)
     /height();
 QString str;


 str.sprintf("H%8.3f",ftmp);

/// QMessageBox::critical(NULL,"error",str);
 str=str+QString(trUtf8(" км"));

 emit signalH(str);
 event->accept();

}
