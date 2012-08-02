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
    BrfData clipboard; // also used for frames...
    BrfData brfdataBackup;

    bool editingRef;
    template<class BrfType> bool addNewGeneral();

 private slots:

    void notifyDataChanged();
    bool setEditingRef(bool mode);

    void closeEvent(QCloseEvent *event);
    void newFile();
    bool open();
    bool save();
    bool saveAs();
    bool openRecentFile();
    bool editRef();
    void about();
    void breakAni(int which, bool useIni);
    void reskeletonize();
    void onChangeMeshMaterial(QString newName);
    void onChangeFlags(QString flags); // of any object
    void onChangeTimeOfFrame(QString flags);

    void updateDataMaterial();
    void updateDataShader();
    void updateDataBody();

    void updateTextureAccessDup();
    void updateTextureAccessDel();
    void updateTextureAccessAdd();

    bool exportBrf();
    bool exportCollisionBody();
    bool exportStaticMesh();
    bool importStaticMesh();
    bool exportRiggedMesh();
    bool importRiggedMesh();
    bool exportMovingMesh();
    bool importMovingMesh();
    bool exportSkeletonMod();
    bool importSkeletonMod();
    bool exportSkeletonAndSkin();
    bool exportSkeleton();
    bool importSkeleton();
    bool importAnimation();
    bool exportAnimation();
    bool importCollisionBody();
    bool addNewMaterial();
    bool addNewTexture();
    bool addNewShader();

    bool importBrf();
    void moveUpSel();
    void moveDownSel();
    void renameSel();
    void deleteSel();
    void duplicateSel();
    void addToRef(); // add current selected item to ref
    void addToRefMesh(int);
    void editCut();
    void editCopy(bool deselect=true);
    void editPaste();
    void editCutFrame();
    void editCopyFrame();
    void editPasteFrame();
    void meshRecomputeNormalsAndUnify();



private:
    std::map< std::string, std::string > mapMT;// map material to textures
    GLWidget *glWidget;
    Selector *selector;
    QSettings *settings;
    GuiPanel *guiPanel;

    QString defpathTexture;

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
    QPair<int, int>  askRefBoneInt(); // ask user to specify a skel and bone
    QPair<int, int>  askRefSkel(int nbones, bool &asAFrame); // ask user to specify two skel
    int askRefSkin(); //  ask user to specify a skin
    int currentDisplaySkin(); // returns skin currently used as display
    int currentDisplaySkeleton(); // returns skeleton currently used as display
    int currentDisplayFrame(); // return v.a. frame currently used as dispalu
    int gimmeASkeleton(int nbones); // returns index of a skeleton with n bones. Maybe asks for one if more than one.
    void insert(const BrfMesh &m);
    void insert(const BrfSkeleton &s);
    void insert(const BrfAnimation &a);
    void insert(const BrfTexture &m);
    void insert(const BrfMaterial &m);
    void insert(const BrfShader &s);
    void insert(const BrfBody &s);
    template<class BrfType> void insert( vector<BrfType> &v, const BrfType &o);

    void saveOptions() const;
    void loadOptions();

    int afterMeshImport() const; // 0:nothing   1:merge   2:normal recompute and merge

    QString askExportFilename(QString);
    QString askImportFilename();

    QString strippedName(const QString &fullFileName);

    QString curFile;
    QString curFileBackup;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *recentFilesMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *editRefAct;
    QAction *separatorAct;
    QAction *editCutAct;
    QAction *editCopyAct;
    QAction *editPasteAct;

    QAction *editCutFrameAct;
    QAction *editCopyFrameAct;
    QAction *editPasteFrameAct;

    QAction *optionAfterMeshLoadMerge;
    QAction *optionAfterMeshLoadRecompute;
    QAction *optionAfterMeshLoadNothing;

    QAction
      *importStaticMeshAct,
      *importRiggedMeshAct,
      *importMovingMeshAct,
      *importSkeletonAct,
      *importAnimationAct,
      *importBodyAct,
      *addNewMaterialAct,
      *addNewShaderAct,
      *addNewTextureAct;

    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    bool maybeSave();
    void setModified(bool mod);
    void updateTitle();
    bool isModified;
    QString lastImpExpFormat;

};

#endif // MAINWINDOW_H
