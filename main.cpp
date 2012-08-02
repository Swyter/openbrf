#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char* argv[])
{

  Q_INIT_RESOURCE(resource);

  QString nextTranslator;


  char* argv_2[]={"OpenBrf"}; int argc_2=1;
  QApplication app(argc_2,argv_2); //argc, argv);
  QStringList arguments = QCoreApplication::arguments();


  while (1){

  QTranslator translator;
  QTranslator qtTranslator;

  if (nextTranslator.isEmpty()){
    QString loc;
    switch (MainWindow::getLanguageOption()) {
    default: loc = QLocale::system().name(); break;
    case 1: loc = QString("en");break;
    case 2: loc = QString("zh_CN");break;
    case 3: loc = QString("es");break;
    }
    translator.load(QString(":/translations/openbrf_%1.qm").arg(loc));
    qtTranslator.load(QString(":/translations/qt_%1.qm").arg(loc));
  } else {
    translator.load(nextTranslator);
  }
  app.installTranslator(&translator);

  app.installTranslator(&qtTranslator);

  MainWindow w;
  w.show();

  if (arguments.size()>=2) w.loadFile(arguments[1]); arguments.clear();

  if (app.exec()==101) {
    nextTranslator = w.getNextTranslatorFilename();
    continue; // just changed language! another run
  }
  break;
  }

  return 0;
}

