#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
  //Q_INIT_RESOURCE(application);

  QApplication app(argc, argv);

  QTranslator translator;
  translator.load(QString("openbrf"));
  app.installTranslator(&translator);

  MainWindow w;
  w.show();
/*
  QMessageBox::information(NULL, QString("Open BRF"),
                              QString("Argc: %1. \n%2\n%3\n%4")
                              .arg(argc)
                              .arg((argc>0)?argv[0]:"")
                              .arg((argc>1)?argv[1]:"")
                              .arg((argc>2)?argv[2]:"")
                              );
*/
  if (argc>=2) w.loadFile(argv[1]);
  return app.exec();
}
