#include "mainwindowTrace.h"
#include "new_traceview.h"
#include "filenew.xpm"
#include "fileopen.xpm"
#include "filesave.xpm"
#include "setcontrol.h"
#include "iko.h"
#include "si_xh.h"
#include "maketras.h"

#include <QMenu>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QTextCodec>

extern SetControl scon; // управляющие параметры

MainwindowTrace::MainwindowTrace(QWidget *parent)
    : QMainWindow(parent)
{
    New_traceView* pView = new New_traceView;

    QMenu* pmnuFile = new QMenu(trUtf8("Файл"));
     QAction* pactNew = new QAction(trUtf8("Новый"),0);
      pactNew->setShortcut(QKeySequence("CTRL+N"));
      pactNew->setToolTip(trUtf8("Новый документ"));
      pactNew->setStatusTip(trUtf8("Cоздание нового документа"));
      pactNew->setWhatsThis(trUtf8("Cоздание нового документа"));
      pactNew->setIcon(QPixmap(filenew));
 //     connect(pactNew, &QAction::triggered, this, &MainwindowTrace::slotNew);
      connect(pactNew, SIGNAL(triggered()), this, SLOT(slotNew()));
     pmnuFile->addAction(pactNew);

     QAction* pactOpen = new QAction("Open file", 0);
      pactOpen->setText("&Открыть..."); // изменяет Open file
      pactOpen->setShortcut(QKeySequence("CTRL+O"));
      pactOpen->setToolTip("Открыть документ");
      pactOpen->setStatusTip("Открыть существующий план");
      pactOpen->setWhatsThis("Открыть существующий план");
      pactOpen->setIcon(QPixmap(fileopen));

      connect(pactOpen, SIGNAL(triggered()), this, SLOT(OnOpen()));
     pmnuFile->addAction(pactOpen);

     QAction* pactSave = new QAction("Save File", 0);
      pactSave->setText("&Сохранить");
      pactSave->setShortcut(QKeySequence("CTRL+S"));
      pactSave->setToolTip("Сохранить трассы");
      pactSave->setStatusTip("Сохранить трассы в файл");
      pactSave->setWhatsThis("Save the file to disk");
      pactSave->setIcon(QPixmap(filesave));
      connect(pactSave, SIGNAL(triggered()), this, SLOT(slotSave()));


//     pmnuFile->addAction(pactNew);
//     connect(pmnuFile,SIGNAL(triggered(QAction*)),SLOT(slotChangeH(QAction*)));

    // pmnuFile->addAction(pactOpen,this,SLOT(slotNew()));
     pmnuFile->addAction(pactSave);
     pmnuFile->addAction("Save &As...", this, SLOT(slotSaveAs()));

     pmnuFile->addSeparator();
     pmnuFile->addAction(trUtf8("Выход"),qApp,SLOT(quit()),QKeySequence("CTRL+Q"));

    QMenu* pmnuMode = new QMenu(trUtf8("&Отображение"));
     QMenu* pmnuSubMenuShow = new QMenu(trUtf8("Масштаб ИКО"),pmnuMode);
     pmnuMode->addMenu(pmnuSubMenuShow);
      pActScale[0] =
              pmnuSubMenuShow->addAction("500");

      pActScale[0]->setIcon(QIcon(":/rec/images/scale500.png"));

      pActScale[0]->setIconVisibleInMenu(false);

      pActScale[0]->setCheckable(true);
      pActScale[0]->setChecked(true);

      pActScale[1] = pmnuSubMenuShow->addAction("300");
      pActScale[1]->setCheckable(true);

      pActScale[1]->setIcon(QIcon(":/rec/images/scale300.png"));

      pActScale[1]->setIconVisibleInMenu(false);


      pActScale[2] = pmnuSubMenuShow->addAction("150");
      pActScale[2]->setCheckable(true);

      pActScale[2]->setIcon(QIcon(":/rec/images/scale150.png"));

      pActScale[2]->setIconVisibleInMenu(false);

      pActScale[3] = pmnuSubMenuShow->addAction("75");
      pActScale[3]->setCheckable(true);

      pActScale[3]->setIcon(QIcon(":/rec/images/scale75.png"));

      pActScale[3]->setIconVisibleInMenu(false);

      pActScale[4] = pmnuSubMenuShow->addAction("32");
      pActScale[4]->setCheckable(true);
      pActScale[4]->setIcon(QIcon(":/rec/images/scale32.png"));

      pActScale[4]->setIconVisibleInMenu(false);

      connect(pmnuSubMenuShow,SIGNAL(triggered(QAction*)),SLOT(slotChangeScale(QAction*)));


      QMenu* pmnuSubMenuHeight = new QMenu(trUtf8("Высота СИ X-H"),pmnuMode);
      pmnuMode->addMenu(pmnuSubMenuHeight);
      pActHeight[0] = pmnuSubMenuHeight->addAction(trUtf8("40 км"));
////      pActHeight40->actionGroup();
      pActHeight[0]->setCheckable(true);
      pActHeight[0]->setChecked(true);

      pActHeight[0]->setIcon(QIcon(":/rec/images/H40.png"));

      pActHeight[0]->setIconVisibleInMenu(false);


      pActHeight[1] = pmnuSubMenuHeight->addAction(trUtf8("20 км"));
      pActHeight[1]->setCheckable(true);

      pActHeight[1]->setIcon(QIcon(":/rec/images/H20.png"));

      pActHeight[1]->setIconVisibleInMenu(false);

      pActHeight[2] = pmnuSubMenuHeight->addAction(trUtf8("10 км"));
      pActHeight[2]->setCheckable(true);
      pActHeight[2]->setIcon(QIcon(":/rec/images/H10.png"));

      pActHeight[2]->setIconVisibleInMenu(false);
      connect(pmnuSubMenuHeight,SIGNAL(triggered(QAction*)),SLOT(slotChangeH(QAction*)));

///OnImit
      QAction* pactImit = new QAction(tr("Имитация"), 0);
      pmnuMode->addAction(pactImit);
       pactImit->setText("Имитация");
       pactImit->setToolTip("Имитация");
       pactImit->setStatusTip("Имитация");
       pactImit->setWhatsThis("Имитация");
       connect(pactImit, SIGNAL(triggered()), this, SLOT(OnImit()));

    QMenu* pmnuHelp = new QMenu(trUtf8("&Помощь"));


    menuBar()->addMenu(pmnuFile);
    menuBar()->addMenu(pmnuMode);
    menuBar()->addMenu(pmnuHelp);
    pmnuHelp->addAction(trUtf8("О программе"),this,SLOT(slotAbout()),Qt::Key_F1);


    QToolBar* ptbFile = new QToolBar("File Operations");
    ptbFile->addAction(pactNew);
    ptbFile->addAction(pactOpen);
    ptbFile->addAction(pactSave);

    for (int i=0;i<5;i++)
        ptbFile->addAction(pActScale[i]);

    for (int i=0;i<3;i++)
        ptbFile->addAction(pActHeight[i]);



    addToolBar(Qt::RightToolBarArea, ptbFile);


 ////   statusBar()->addWidget(new QLabel(this));
    setCentralWidget(pView);
    setWindowTitle(TITLE_NO_NAME);

}

MainwindowTrace::~MainwindowTrace()
{
    
}
void MainwindowTrace::slotAbout()
{
    QMessageBox::about(this,trUtf8("Писалка трасс"),trUtf8("Версия Астра Луникс 1.0"));
}
void MainwindowTrace::slotSaveAs()
{
    if (windowTitle()==TITLE_NO_NAME)
    {
     QString str=QFileDialog::getSaveFileName(this,"Выбор файла","","*.trc ");
     if (str==NULL)
         return;
      setWindowTitle("trace-"+str);
      setWindowFilePath(str);

    }
    QString fname;
    fname = windowFilePath();
    QFile f(fname);
    if (f.open(QIODevice::WriteOnly ))
    {
     char head_f[]="SAL01";
     f.write(head_f,5); // сохранили заголовок
     New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());
     DocTras* pdoc= pView->GetDocument();
     QDataStream stream(&f);
     stream << pdoc->m_Trackes;


     f.flush();
     f.close();
    }
    else
      qDebug("error open file\n");

    // настройка кодека для корректной обработки русских букв
   //     QTextCodec *codec1 = QTextCodec::codecForName("UTF-8");
   //     QTextCodec::setCodecForLocale(codec1);


}
void MainwindowTrace::slotChangeScale(QAction* prAc)
{
 QString str = prAc->text();
 if (scon.dist==str.toInt())
     return;
 for (int i=0;i<5;i++)
    pActScale[i]->setChecked(false);

 switch(str.toInt()) {
  case 500:
     pActScale[0]->setChecked(true);
     scon.dist=str.toInt();
  break;
  case 300:
     pActScale[1]->setChecked(true);
     scon.dist=str.toInt();

  break;
  case 150:
     pActScale[2]->setChecked(true);
     scon.dist=str.toInt();

  break;
  case 75:
     pActScale[3]->setChecked(true);
     scon.dist=str.toInt();

  break;
  case 32:
     pActScale[4]->setChecked(true);
     scon.dist=str.toInt();

  break;

 }
 New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());
 pView->pIKO->update();

}
void MainwindowTrace::slotChangeH(QAction* pHAc)
{
 QString str = pHAc->text();
 str=str.mid(0,2);

 for (int i=0;i<3;i++)
    pActHeight[i]->setChecked(false);

 switch(str.toInt()) {
  case 40:
     pActHeight[0]->setChecked(true);
     scon.h=40;

  break;
  case 20:
     pActHeight[1]->setChecked(true);
//     scon.h=str.toInt();
      scon.h=str.toInt();

  break;
  case 10:
     pActHeight[2]->setChecked(true);
     scon.h=str.toInt();

  break;

 }
 New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());
 pView->psi_xh->update();

}
void MainwindowTrace::slotNew()
{
 New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());

 pView->GetDocument()->DeleteAllTras();
 pView->pIKO->update();
 pView->psi_xh->update();

 pView->SetCurSel(0);
 pView->CorNTras(0);
 pView->FillList();
}
void MainwindowTrace::slotSave()
{
 if (windowTitle()==TITLE_NO_NAME)
 {
  QString str=QFileDialog::getSaveFileName(this,"Выбор файла",TITLE_NO_NAME,"*.trc ");
  if (str==NULL)
      return;
   setWindowTitle("trace-"+str);
   setWindowFilePath(str);

 }
 QString fname;
 fname = windowFilePath();
 QFile f(fname);
 if (f.open(QIODevice::WriteOnly ))
 {
  char head_f[]="SAL01";
  f.write(head_f,5); // сохранили заголовок
  New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());
  DocTras* pdoc= pView->GetDocument();
  QDataStream stream(&f);
  stream << pdoc->m_Trackes;


  f.flush();
  f.close();
 }
 else
   qDebug("error open file\n");

 // настройка кодека для корректной обработки русских букв
//     QTextCodec *codec1 = QTextCodec::codecForName("UTF-8");
//     QTextCodec::setCodecForLocale(codec1);


}
//*******************************************************************************************
// Открыть файл
//*******************************************************************************************
void MainwindowTrace::OnOpen()
{
 QString str=QFileDialog::getOpenFileName(this,"Открыть файл","","*.trc");//После закрытия диалогового окна мы проверяем
 //строку str на содержимое, и если оно есть, то
 if (!str.isEmpty())
 {
  QFile file;
  QDataStream stream;
  file.setFileName(str);
  if (!file.open(QIODevice::ReadOnly))
  {
    QMessageBox::warning(this,trUtf8("Ошибка открытия  "),
                         tr("файл %1\n%2 ").arg(file.fileName()).arg(file.errorString()));
   return ;
  }

  stream.setDevice((QIODevice *)&file);
  char head_f[5];
  if (stream.readRawData((char*)&head_f,5)==-1)
  {
     QMessageBox::about(this,trUtf8(""),trUtf8("Слишком мало байт в файле"));
     file.close();
     return ;
  }
  if (strncasecmp("SAL01",head_f,5)!=0 )
  {
      QMessageBox::warning(this,trUtf8("Ошибка  "),tr("Неправильный заголовок файла  "));
      file.close();

      return;
  }
  New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());
  DocTras* pdoc= pView->GetDocument();

  stream >> pdoc->m_Trackes;

 /// int NumTR; // номер точки рубежа
 /// NumTR = pdoc->m_Trackes[0].NumTR;
 ///// NumTR = pdoc->m_Trackes[1].NumTR;


  pView->pIKO->update();
  pView->psi_xh->update();

  pView->SetCurSel(0);
  pView->CorNTras(0);
  pView->FillList();

 }

}
void MainwindowTrace::OnImit()
{
 New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());

 DocTras* pDoc = pView->GetDocument();
 double t;
 if (pDoc->GetTimeT(t)==0)
 {
/////     MessageBox("Идиот! Введи трассы .","Нечему летать",MB_OK);
     return;  // нет трасс
 }

}
