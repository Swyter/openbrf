#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{

  Q_INIT_RESOURCE(resource);

  QString nextTranslator;

  while (1){
  QApplication app(argc, argv);

  QTranslator translator;

  if (nextTranslator.isEmpty()){
    QString loc;
    switch (MainWindow::getLanguageOption()) {
    default: loc = QLocale::system().name(); break;
    case 1: loc = QString("en");break;
    case 2: loc = QString("zh");break;
    }
    translator.load(QString(":/translations/openbrf_%1.qm").arg(loc));
  } else {
    translator.load(nextTranslator);
  }
  app.installTranslator(&translator);

  QTranslator qtTranslator;
  qtTranslator.load("qt");
  app.installTranslator(&qtTranslator);

  MainWindow w;
  w.show();

  if (app.exec()==101) {
    nextTranslator = w.getNextTranslatorFilename();
    continue; // just changed language! another run
  }
  break;
  }

  return 0;
}

