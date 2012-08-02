#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <map>

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

    int GetFirstUnusedRefLetter() const;
private:
    BrfData brfdata;
    BrfData reference;
    BrfData brfdataBackup;

    bool editingRef;

 private slots:

    bool setEditingRef(bool mode);

    void closeEvent(QCloseEvent *event);
    //void newFile();
    bool open();
    bool save();
    bool saveAs();
    bool openRecentFile();
    bool editRef();
    void about();
    void breakAni(int which, bool useIni);
    void onChangeMeshMaterial(QString newName);
    void onChangeFlags(QString flags); // of any object
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
    void addToRef(); // add current selected item to ref
    void addToRefMesh(int);

private:
    std::map< std::string, std::string > mapMT;// map material to textures
    GLWidget *glWidget;
    Selector *selector;
    QSettings *settings;
    GuiPanel *guiPanel;

    bool scanBrfForMaterials(const QString fname);
    bool scanIniForMaterials(const QString fname);
    bool scanBrfDataForMaterials(const BrfData &d);
    void tryLoadMaterials();

    void createActions();
    void createMenus();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveReference();
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString askExportFilename(QString, QString ext );
    QString askImportFilename(QString ext);
    QPair<int, int>  askRefBoneInt();

    QString askExportFilename(QString);
    QString askImportFilename();

    QString strippedName(const QString &fullFileName);

    QString curFile;
    QString curFileBackup;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *recentFilesMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *editRefAct;
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
