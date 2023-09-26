#include <QApplication>
#include "mainwindowTrace.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 ///   MainwindowTrace w;
///    w.show();
  extern  MainwindowTrace* pMW;
  pMW = new  MainwindowTrace;
  pMW->show();
  return a.exec();
}
