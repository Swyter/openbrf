#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "brfdata.h"
#include "glwidgets.h"
#include "selector.h"
#include "guipanel.h"

//namespace Ui
//{
//    class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    BrfData brfdata;
    BrfData reference;

 private slots:

    void closeEvent(QCloseEvent *event);
    //void newFile();
    bool open();
    bool save();
    bool saveAs();
    bool openRecentFile();
    void about();
    void breakAni(int which, bool useIni);
    void onChangeMeshMaterial(QString newName);
    void onChangeMeshFlags(QString flags);
    bool exportBrf();
    bool exportPly();
    bool exportSkelMod();
    bool importSkelMod();
    bool importMeshPly();
    bool importBrf();
    void moveUpSel();
    void moveDownSel();
    void renameSel();
    void deleteSel();
    void duplicateSel();

private:
    GLWidget *glWidget;
    Selector *selector;
    QSettings *settings;
    GuiPanel *guiPanel;

    void createActions();
    void createMenus();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString askExportFilename(QString, QString ext );
    QString askImportFilename(QString ext);

    QString askExportFilename(QString);
    QString askImportFilename();

    QString strippedName(const QString &fullFileName);

    QString curFile;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *recentFilesMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    //QAction *aboutQtAct;
    QAction *separatorAct;

    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    bool maybeSave();
    void setModified(bool mod);
    void updateTitle();
    bool isModified;

};

#endif // MAINWINDOW_H
