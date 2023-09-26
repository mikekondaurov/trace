#include <QtGui>
#include <QDebug>

#include "maketras.h"
#include "convcoor.h"
#include "setcontrol.h"

TrasPoint::TrasPoint(float B1,float D1,int H1,unsigned int A1,unsigned int V1)
{
 B=B1;
 D=D1;
 H= H1;
 A=A1;
 V=V1;

}


//Конструктор класса
Tras::Tras(float B,float D,int H1,
    kmb_trace::TargetType type_tar1       )
{
 AddTP(B,D,H1);
 type_tar=type_tar1;
 //epr=1.1;
 NumTR=0;
}
//добавление точки трассы

int Tras::AddTP(float B,float D,unsigned int H1,
       unsigned int A,unsigned int V)
{
 if (V==0) return 1;
 int size;
 size = m_TrasPoint.size();
 if (0<size)
  if ((m_TrasPoint[size-1].B-B)*(m_TrasPoint[size-1].B-B)+
      (m_TrasPoint[size-1].D-D)*(m_TrasPoint[size-1].D-D)+
      (m_TrasPoint[size-1].H-H1)*(m_TrasPoint[size-1].H-H1)==0)
      return 2;

 TrasPoint tp(B,D,H1,A,V);
 m_TrasPoint.push_back(tp);
 int rc=CheckTras();
 if (rc!=0)
 {
   m_TrasPoint.pop_back();
   return rc;

 }

 return 0;
}
void Tras::DrawH(QPainter& painter,QSize sz_pix)
{
 extern SetControl scon; // управляющие параметры
 float fx,fy;
 int x,y;
 int size = m_TrasPoint.size();
 if (size==0)  // Если нет точек выходим
        return;
 BDToXY(m_TrasPoint[0].B,m_TrasPoint[0].D,fx,fy);
 x = fx*sz_pix.width()/(scon.dist*2)+(float)sz_pix.width()/2;
/// y =  -fy*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
 y =sz_pix.height()-1.0- (m_TrasPoint[0].H*sz_pix.height())/(scon.h*1000.0);
 // Вывод начала трассы
 painter.drawRect(x,y,3,3);
 // Вывод линий
 for (int i=1;i<size;i++)
 {
  int x2,y2;
  BDToXY(m_TrasPoint[i-1].B,m_TrasPoint[i-1].D,fx,fy);
  x = fx*sz_pix.width()/(scon.dist*2)+(float)sz_pix.width()/2;
  y =sz_pix.height()-1.0- (m_TrasPoint[i-1].H*sz_pix.height())/(scon.h*1000.0);

  BDToXY(m_TrasPoint[i].B,m_TrasPoint[i].D,fx,fy);
  x2 = fx*sz_pix.width()/(scon.dist*2)+(float)sz_pix.width()/2;
  y2 =sz_pix.height()-1.0- (m_TrasPoint[i].H*sz_pix.height())/(scon.h*1000.0);

  painter.drawLine(x,y,x2,y2);

 }
 // вывод точки рубежа
 BDToXY(m_TrasPoint[NumTR].B,m_TrasPoint[NumTR].D,fx,fy);
 x = fx*sz_pix.width()/(scon.dist*2)+(float)sz_pix.width()/2;
 y =sz_pix.height()-1.0- (m_TrasPoint[NumTR].H*sz_pix.height())/(scon.h*1000.0);
 painter.drawRect(x,y,3,3);


 // Рисование точек начала маневра
 BDToXY(m_TrasPoint[0].B,m_TrasPoint[0].D,fx,fy);
 fy = m_TrasPoint[0].H; // метры

 x = fx*sz_pix.width()/(scon.dist*2)+(float)sz_pix.width()/2;
 y =sz_pix.height()-1.0- (fy*sz_pix.height())/(scon.h*1000.0);
 painter.drawRect(x,y,3,3);

 // вывод реальных точек - не работает

 int size1 = m_RealTT.size();
 for (int i=0; i<size1;i++)
 {
  float fx_sum,fy_sum;
 // float cur_t1=m_RealTT[i].time_move;

  for (float cur_t=0;cur_t<=m_RealTT[i].time_move; cur_t+=1)
  {
   fx_sum=fx+m_RealTT[i].fvx*cur_t/1000+(m_RealTT[i].fax/ 2000)*cur_t*cur_t  ;
   fy_sum=fy+m_RealTT[i].fvz*cur_t +(m_RealTT[i].faz/2)*cur_t*cur_t;


   x = fx_sum*sz_pix.width()/(scon.dist*2)+(float)sz_pix.width()/2;
   y =sz_pix.height()-1.0- (fy_sum*sz_pix.height())/(scon.h*1000.0);

   if (m_RealTT[i].fax !=0 ||
        m_RealTT[i].fay!=0 ||
        m_RealTT[i].faz!=0 )
   {
    painter.setPen(Qt::green);
    painter.drawPoint(x,y);

   }
   else {
    painter.setPen(Qt::black);
    painter.drawPoint(x,y);

   }
  }
  fx=fx_sum;
  fy=fy_sum;

 }

}

void Tras::Draw(QPainter& painter,QSize sz_pix,float dist)
{
//// SetControl scon; // управляющие параметры

 float fx,fy;
 int x,y;

 int size = m_TrasPoint.size();
 if (size==0)  // Если нет точек выходим
        return;
 BDToXY(m_TrasPoint[0].B,m_TrasPoint[0].D,
        fx,fy);
 x = fx*sz_pix.width()/(dist*2)+(float)sz_pix.width()/2;
 y =  -fy*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
    // Вывод начала трассы
 painter.drawRect(x,y,3,3);
 // Вывод линий
 for (int i=1;i<size;i++)
 {

  float fx2,fy2,fx1,fy1;
  BDToXY(m_TrasPoint[i-1].B,m_TrasPoint[i-1].D,
     fx1,fy1);
  BDToXY(m_TrasPoint[i].B,m_TrasPoint[i].D,
     fx2,fy2);

  x = fx1*sz_pix.width()/(dist*2)+(float)sz_pix.width()/2;
  y = - fy1*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
  int x2,y2;
  x2 = fx2*sz_pix.width()/(dist*2)+(float)sz_pix.width()/2;
  y2 = - fy2*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
  painter.drawLine(x,y,x2,y2);
 }
 //Вывод точки рубежа
 BDToXY(m_TrasPoint[NumTR].B,m_TrasPoint[NumTR].D,
     fx,fy);
 x = fx*sz_pix.width()/(dist*2)+(float)sz_pix.width()/2;
 y = - fy*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
 painter.drawRect(x,y,3,3);

 // Рисование точек начала маневра
 BDToXY(m_TrasPoint[0].B,m_TrasPoint[0].D,
     fx,fy);
 x = fx*sz_pix.width()/(dist*2)+(float)sz_pix.width()/2;
 y = - fy*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
 painter.drawRect(x,y,3,3);

 int size1 = m_RealTT.size();
 for (int i=0; i<size1;i++)
 {
  float fx_sum,fy_sum;
  ///float cur_t1=m_RealTT[i].time_move;

  for (float cur_t=0;cur_t<=m_RealTT[i].time_move; cur_t+=0.1)
  {
   fx_sum=fx+m_RealTT[i].fvx*cur_t/1000+(m_RealTT[i].fax/ 2000)*cur_t*cur_t  ;
   fy_sum=fy+m_RealTT[i].fvy*cur_t/1000 +(m_RealTT[i].fay/2000)*cur_t*cur_t;

   x = fx_sum*sz_pix.width()/(dist*2)+(float)sz_pix.width()/2;
   y = - fy_sum*sz_pix.height()/(dist*2) + (float)sz_pix.height()/2;
   if (m_RealTT[i].fax !=0 ||
        m_RealTT[i].fay!=0 ||
        m_RealTT[i].faz!=0 )
   {
    painter.setPen(Qt::green);
    painter.drawPoint(x,y);
   }
   else {
    painter.setPen(Qt::black);
    painter.drawPoint(x,y);
   }
  }
  fx=fx_sum;
  fy=fy_sum;

 }
}

void Tras::BDToXY(float B, float D, float &x, float &y)
{
    if (B==-1)
    {
     x=0;
     y=D;
    }
    else
    {
     y=D*cos(B*M_PI/180);
     x=D*sin(B*M_PI/180);
    }

}
// **********************************************
// 0 - все хорошо
//***********************************************
int Tras::CheckTras()
{
 ////double DX,DY,DZ;
 float tx,ty,tx1,ty1;
 float DT,VX,VY,VZ;
 ////double VX0=0,VY0=0,VZ0=0;
 ////double TP=0;  // время прибытия в ОТ

    // Обнуление массива реальных точек
 m_RealTT.erase(m_RealTT.begin(),m_RealTT.end());
 RealTT rtt;
 m_RealTT.push_back(rtt);
 int size;
 size = m_TrasPoint.size();

 if (size==1)
 {
  m_RealTT[0].time_move=-1;

 }
 if (2<=size)
 {
  BDToXY(m_TrasPoint[1].B,m_TrasPoint[1].D*1000,
      tx,ty);
  BDToXY(m_TrasPoint[0].B,
       m_TrasPoint[0].D*1000,tx1,ty1);
  DT=sqrt((tx-tx1)*(tx-tx1)+(ty-ty1)*(ty-ty1)+
  (m_TrasPoint[1].H-m_TrasPoint[0].H)*
  (m_TrasPoint[1].H-m_TrasPoint[0].H));
  DT = DT /  m_TrasPoint[1].V;
  m_RealTT[0].fvx = (tx-tx1)/DT;
  m_RealTT[0].fvy = (ty-ty1)/DT;
  m_RealTT[0].fvz = (m_TrasPoint[1].H-m_TrasPoint[0].H)/DT;
  m_RealTT[0].fax =m_RealTT[0].fay
      =m_RealTT[0].faz=0 ;
  m_RealTT[0].time_move=DT;
 }
 for (int i=2;i<size;i++)
 {

  float DVX,DVY,DVZ,TM;
  float DT;
  float VX0,VY0,VZ0;
  CalcV(i-1,VX0,VY0,VZ0,DT);
  CalcV(i,VX,VY,VZ,DT);

  DVX = VX - VX0;
  DVY = VY - VY0;
  DVZ = VZ - VZ0;
  TM=DVX*DVX+DVY*DVY+DVZ*DVZ;

  TM = sqrt(DVX*DVX+DVY*DVY+DVZ*DVZ)
       /m_TrasPoint[i-1].A;
  if (m_RealTT[m_RealTT.size()-1].time_move
      <= TM/2
      || DT< TM/2 )
  {
////         m_TrasPoint.pop_back();
      return i+100;
  }
  m_RealTT[m_RealTT.size()-1].time_move
      -=TM/2;
  // Добавляем новую точку трассы
  m_RealTT.push_back(rtt);
  m_RealTT[m_RealTT.size()-1].time_move=
      TM;
  m_RealTT[m_RealTT.size()-1].fvx = VX0;
  m_RealTT[m_RealTT.size()-1].fvy = VY0;
  m_RealTT[m_RealTT.size()-1].fvz = VZ0;
  m_RealTT[m_RealTT.size()-1].fax =
      DVX/TM;
  m_RealTT[m_RealTT.size()-1].fay=DVY/TM;
  m_RealTT[m_RealTT.size()-1].faz=DVZ/TM;
  // Добавляем новую точку трассы

  m_RealTT.push_back(rtt);
  m_RealTT[m_RealTT.size()-1].time_move=
     DT- TM/2;
  m_RealTT[m_RealTT.size()-1].fvx = VX;
  m_RealTT[m_RealTT.size()-1].fvy = VY;
  m_RealTT[m_RealTT.size()-1].fvz = VZ;
  m_RealTT[m_RealTT.size()-1].fax = 0;
  m_RealTT[m_RealTT.size()-1].fay=0;
  m_RealTT[m_RealTT.size()-1].faz=0;

 }

 return 0;

}
void Tras::CalcV(int i, float &VX, float &VY,float &VZ,float& DT)
{
    float tx,ty,tx1,ty1;

    BDToXY(m_TrasPoint[i].B,m_TrasPoint[i].D*1000,
         tx,ty);
     BDToXY(m_TrasPoint[i-1].B,
          m_TrasPoint[i-1].D*1000,tx1,ty1);
///	 DT=sqrt((tx-tx1)*(tx-tx1)+(ty-ty1)*(ty-ty1)+
     DT=sqrt((float)((tx-tx1)*(tx-tx1)+(ty-ty1)*(ty-ty1)+

     (m_TrasPoint[i].H-m_TrasPoint[i-1].H)*
     (m_TrasPoint[i].H-m_TrasPoint[i-1].H)));
     DT = DT /  m_TrasPoint[i].V;
     VX = (tx-tx1)/DT;
     VY = (ty-ty1)/DT;
     VZ = (m_TrasPoint[i].H-m_TrasPoint[i-1].H)/DT;

}

//____________________________________
DocTras::DocTras()
{

}

void DocTras::AddTras(float B, float D, int H, kmb_trace::TargetType type_tar1)
{

 Tras tr(B,D,H,type_tar1);
 m_Trackes.push_back(tr);

}
//*********************************************
// Возвращает номер трассы
// -1 - если не найдена
//*********************************************
// num_point = 1 - поиск любой точки
// num_point = 0 - поиск последней точки
// db - дельта пеленга dd - дельта дальности
int DocTras::FindTras (int cur_tras,float B, float D
 , float db,float dd,int& num_point)
{
 int i;
 // Узнаем размер контейнера точек
 int ntrace = m_Trackes.size();
 // Уходим , если он пуст
 int last_el;
 if (ntrace==0) // Если нет трасс уходим
        return -1;


 int j;
 if (cur_tras!=-1)  // если не стоим на новой трассе
 {
    last_el=m_Trackes[cur_tras].m_TrasPoint.size();
   if (num_point==0)
   {
    if ( fabs(m_Trackes[cur_tras].m_TrasPoint[last_el-1].D
     -D)<dd &&
    fabs(m_Trackes[cur_tras].m_TrasPoint[last_el-1].B -B)<db)
    return cur_tras;
   }
   else
   {
    for (j=0;j<last_el;j++)
    {
     if ( fabs(m_Trackes[cur_tras].m_TrasPoint[j].D
      -D)<dd &&
     fabs(m_Trackes[cur_tras].m_TrasPoint[j].B -B)<db)
     {
         num_point=j;
         return cur_tras;
     }
    }
   }
 }
  for ( i=0;i<ntrace;i++)
  {
   last_el=m_Trackes[i].m_TrasPoint.size();
   if (num_point==0)
   {
    if ( fabs(m_Trackes[i].m_TrasPoint[last_el-1].D
     -D)<dd &&
    fabs(m_Trackes[i].m_TrasPoint[last_el-1].B -B)<db)
    return i;
   }
   else
   {
    for (j=0;j<last_el;j++)
    {
     if ( fabs(m_Trackes[i].m_TrasPoint[j].D
      -D)<dd &&
     fabs(m_Trackes[i].m_TrasPoint[j].B -B)<db)
     {
         num_point=j;
         return i;
     }
    }
   }
  }
 return -1;

}
// удалить все трассы
void DocTras::DeleteAllTras()
{
 m_Trackes.clear();

}
// Возвращает число трасс
int DocTras::GetTimeT(double& drc)
{
 int size = m_Trackes.size();

 return size;
}
