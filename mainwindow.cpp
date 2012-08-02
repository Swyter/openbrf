#include "brfdata.h"
#include "glwidgets.h"
#include "selector.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
   // : QWidget(parent)//, ui(new Ui::MainWindow)
{

    settings = new QSettings("mtarini", "OpenBRF");

    printf("LET'S GO...\n");

    glWidget = new GLWidget;
    selector = new Selector(this);

    /*
    connect(selector,SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            glWidget,SLOT(selectionChanged(QItemSelection, QItemSelection)));


    connect(selector, SIGNAL(tabChanged(int)),
            glWidget, SLOT(tabChanged(int)) );
            */
    connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
             glWidget,  SLOT(setSelection(QModelIndexList,int)) );

    //selector->glw = glWidget;

    //ui->setupUi(this);

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QWidget* main = new QWidget();
    mainLayout->addWidget(selector);
    mainLayout->addWidget(glWidget);
    main->setLayout(mainLayout);

    //reference.Load("skeletons.brf",0,SKELETON);
    //reference.Merge(  BrfData("horse_skeleton.brf",0,SKELETON) );
    reference.Load("reference.brf");

    setCentralWidget(main);

    setWindowTitle(tr("Open BRF"));

    createActions();
    createMenus();

    glWidget->selected=1;
    glWidget->data = &brfdata;
    glWidget->reference = &reference;

    this->setAcceptDrops(true);

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // accept just text/uri-list mime format
    //if (event->mimeData()->hasFormat("text/uri-list"))
    //{
        event->acceptProposedAction();
    //}
}

void MainWindow::about()
//! [13] //! [14]
{

   QMessageBox::about(this, tr("Open-Brf"),
            tr("<b>ver 0.0.3 pre-alpha</b><br>"
               "(%1)<br>"
               "by mtarini --- Marco Tarini ").arg(__DATE__) );
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList;
    QString fName;
    QFileInfo info;

    if (event->mimeData()->hasUrls())
    {
        urlList = event->mimeData()->urls(); // returns list of QUrls

        // if just text was dropped, urlList is empty (size == 0)
        if ( urlList.size() > 0) // if at least one QUrl is present in list
        {
            fName = urlList[0].toLocalFile(); // convert first QUrl to local path
            info.setFile( fName ); // information about file
            if ( info.isFile() ) loadFile( fName ); // if is file, setText
        }
    }
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();

    //menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Info"));
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

void MainWindow::loadFile(const QString &fileName)
{
  setCurrentFile(fileName);
  if (!brfdata.Load(fileName.toAscii().data())) {
        QMessageBox::information(this, tr("Open BRF"),
                                 tr("Cannot load %1.").arg(fileName));

  }   {
    selector->setup(brfdata);
    //glWidget->selectNone();
    selector->setCurrentIndex(100); // for some reason, if I set the 0 message is not sent

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded!"), 2000);
  }
}

void MainWindow::saveFile(const QString &fileName)
{
  setCurrentFile(fileName);
  if (!brfdata.Save(fileName.toAscii().data())) {
        QMessageBox::information(this, tr("Open BRF"),
                                 tr("Cannot write file %1.").arg(fileName));

  }
  statusBar()->showMessage(tr("File saved!"), 2000);
}


void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File") ,
        settings->value("LastOpenPath").toString(),
        tr("Resource (*.brf)")
     );
       // QDir::currentPath());

    if (!fileName.isEmpty()) loadFile(fileName);
}

void MainWindow::save()
{
    if (curFile.isEmpty())
        saveAs();
    else
        saveFile(curFile);
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
         tr("Open File") ,
        settings->value("LastOpenPath").toString(),
        tr("Resource (*.brf)")
    );
    if (fileName.isEmpty())
        return;

    saveFile(fileName);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    if (curFile.isEmpty())
        setWindowTitle(tr("Recent Files"));
    else
        setWindowTitle(tr("%2 - %1").arg(curFile)
                                    .arg(tr("OpenBrf")));

    QStringList files = settings->value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings->setValue("recentFileList", files);
    settings->setValue("lastOpenPath",QFileInfo(fileName).absolutePath());

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

MainWindow::~MainWindow()
{
    //delete ui;
}


void MainWindow::createActions()
{
    //newAct = new QAction(tr("&New"), this);
    //newAct->setShortcuts(QKeySequence::New);
    //newAct->setStatusTip(tr("Create a new file"));
    //connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Alt+F4"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(tr("F1"));
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    //aboutQtAct = new QAction(tr("About &Qt"), this);
    //aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    //connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::updateRecentFileActions()
{
    QStringList files = settings->value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
