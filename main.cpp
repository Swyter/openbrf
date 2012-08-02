#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{

  //Q_INIT_RESOURCE(resource);


  while (1){
  QApplication app(argc, argv);
  int k = MainWindow::getLanguageOption();

  QTranslator translator;

  QString loc;
  switch (k) {
  default: loc = QLocale::system().name(); break;
  case 1: loc = QString("en");break;
  case 2: loc = QString("zh");break;
  }

  if  (translator.load(QString(":/openbrf_%1.qm").arg(loc)))
  app.installTranslator(&translator);


  QTranslator qtTranslator;
  qtTranslator.load("qt");
  app.installTranslator(&qtTranslator);


  MainWindow w;
  w.show();
  if (app.exec()==101) continue; // just changed language! another run!
  break;
  }

  return 0;
}

