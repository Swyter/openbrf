#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
//#include <map>

#include "brfdata.h"
#include "inidata.h"
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

    bool loadModAndDump(QString modpath, QString file);

    int GetFirstUnusedRefLetter() const;
    bool loadFile(const QString &fileName);

    static int getLanguageOption();
    QString getNextTranslatorFilename(){return nextTranlationFilename;};

private:
    BrfData brfdata;
    BrfData reference;
    BrfData clipboard; // also used for frames...
    BrfData brfdataBackup;
    IniData inidata;

    bool editingRef;
    template<class BrfType> bool addNewGeneral(QStringList def);

 private slots:

    void notifyCheckboardChanged();
    void notifyDataChanged();
    bool setEditingRef(bool mode);

    void closeEvent(QCloseEvent *event);
    void newFile();
    bool open();
    bool save();
    bool saveAs();
    bool openRecentFile();
    bool editRef();

    void registerExtension();

    void aniExtractInterval();
    void aniRemoveInterval();
    void aniMerge();


    void about();
    void aboutCheckboard();
    void breakAni(int which, bool useIni);
    void shiftAni();
    void bodyMakeQuadDominant();
    void bodyMerge();
    void reskeletonize();
    void transferRigging();
    void flip();
    void scale();
    void transform();
    void onChangeMeshMaterial(QString newName);
    void onChangeFlags(QString flags); // of any object
    void onChangeTimeOfFrame(QString flags);

    void setSelection(const QModelIndexList &l,int);



    void updateDataMaterial();
    void updateDataShader();
    void updateDataBody();

    void updateTextureAccessDup();
    void updateTextureAccessDel();
    void updateTextureAccessAdd();

    //bool exportBrf();
    bool exportCollisionBody();
    bool exportMeshGroup();
    bool exportMeshGroupManyFiles();
    bool exportStaticMesh();
    bool importStaticMesh();
    bool exportRiggedMesh();
    bool importRiggedMesh();
    bool importMovingMeshFrame();
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
    bool addNewUiPicture();
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
    void editAddToCopy();
    void editPaste();
    void editCutFrame();
    void editCopyFrame();
    void editPasteFrame();
    void editPasteRigging();
    void editPasteMod();
    void editPasteTimings();
    void sortEntries();
    void meshRecomputeNormalsAndUnifyDoIt();
    void meshRecomputeNormalsAndUnify_onSlider(int i);
    void meshRecomputeNormalsAndUnify_onCheckbox(bool i);
    void meshRecomputeNormalsAndUnify();
    void meshUnify();
    void meshMerge();
    void meshToBody();
    void meshMountOnBone();
    void meshRemoveBack();
    void meshAddBack();
    void meshDiscardRig();
    void meshDiscardCol();
    void meshDiscardAni();
    void meshRecolor();
    void meshTuneColor();
    void meshComputeAo();
    void meshTuneColorDo(int,int,int,int);

    void setFlagsMaterial();

    void onClipboardChange();

    bool navigateLeft();
    bool navigateRight();
    bool navigateUp();
    bool navigateDown();
    bool searchBrf();
    bool refreshIni();
    bool checkIni();
    bool searchIni();
    void openModuleIniFile();
    void optionAutoFixTextureUpdated();
    void optionAutoFixTextureShowInfo();

    void optionLanguageSet0();
    void optionLanguageSet1();
    void optionLanguageSet2();
    void optionLanguageSet3();
    void optionLanguageSet4();
    void optionLanguageSetCustom();

    void mab2tldHead();
    void tld2mabHead();
    void mab2tldArmor();
    void tld2mabArmor();
    void tldHead(float verse);
    void tldMakeDwarfSlim();
    void tldMakeDwarfBoots();
    void tldShrinkAroundBones();

    void goUsedBy();
    void computeUsedBy();
    void selectBrfData();
    void showUnrefTextures();
    void showModuleStats();
    void moduleSelect();
    void exportNames();


public slots:
    void displayInfo(QString st, int howlong);

private:
    bool goTo(ObjCoord o);
    //std::map< std::string, std::string > mapMT;// map material to textures
    GLWidget *glWidget;
    Selector *selector;
    QSettings *settings;
    GuiPanel *guiPanel;

    bool guessPaths(QString fn);
    void updatePaths();
    bool loadIni(int lvl);
    QString mabPath;
    QString modName;
    QString modPath() const;
    QString lastSearchString;

    int curLanguage; // 0 = use default
    QString nextTranlationFilename; // which file to load next time (post mortem msg)
    void setLanguage(int k);

    bool scanBrfForMaterials(const QString fname);
    bool scanIniForMaterials(const QString fname);
    bool scanBrfDataForMaterials(const BrfData &d);
    void tryLoadMaterials();

    void createActions();
    void createMenus();
    void createMiniViewOptions();
    void createConnections();
    bool saveFile(const QString &fileName);
    bool saveReference();
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString askExportFilename(QString, QString ext );
    QString askImportFilename(QString ext);
    QStringList askImportFilenames(QString ext);
    QPair<int, int>  askRefBoneInt(bool sayNotRigged, bool &isOri); // ask user to specify a skel and bone
    QPair<int, int>  askRefSkel(int nbones, int &method, int &res); // ask user to specify two skel
    int askRefSkin(); //  ask user to specify a skin
    int currentDisplaySkin(); // returns skin currently used as display
    int currentDisplaySkeleton(); // returns skeleton currently used as display
    int currentDisplayFrame(); // return v.a. frame currently used as dispalu
    int gimmeASkeleton(int nbones); // returns index of a skeleton with n bones. Maybe asks for one if more than one.
    void cancelNavStack();
    void insert(const BrfMesh &m);
    void insert(const BrfSkeleton &s);
    void insert(const BrfAnimation &a);
    void insert(const BrfTexture &m);
    void insert(const BrfMaterial &m);
    void insert(const BrfShader &s);
    void insert(const BrfBody &s);
    void insertOrReplace(const BrfMesh &m);
    void insertOrReplace(const BrfSkeleton &s);
    void insertOrReplace(const BrfAnimation &a);
    void insertOrReplace(const BrfTexture &m);
    void insertOrReplace(const BrfMaterial &m);
    void insertOrReplace(const BrfShader &s);
    void insertOrReplace(const BrfBody &s);
    void selectOne(int kind, int i);

    void loadSystemClipboard();
    void saveSystemClipboard();

    template<class BrfType> void insert( vector<BrfType> &v, const BrfType &o);
    template<class BrfType> void insertOrReplace( vector<BrfType> &v, const BrfType &o);
    template<class BrfType> void objectMergeSelected(vector<BrfType> &v);
    void saveOptions() const;
    void loadOptions();

    void inidataChanged(); // call me when brf content changed
    void updateGl();
    void updateGui();
    void updateSel();
    void findCurFileInIni();

    int afterMeshImport() const; // 0:nothing   1:merge   2:normal recompute and merge
    int assembleAniMode() const; // 0:trust vertex order   1:trust vertex coords
    int currAoBrightnessLevel() const;
    int currAoFromAboveLevel() const;

    void applyAfterMeshImport(BrfMesh &m);

    QString askExportFilename(QString);
    QString askImportFilename();

    QString strippedName(const QString &fullFileName);

    QString curFile;
    QString curFileBackup;
    int curFileIndex; // in module.ini, or -1

    QWidget *comboViewmodeSelector;
    QButtonGroup *comboViewmodeBG;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *recentFilesMenu;
    QAction *newAct;
    QAction *registerMime;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *sortEntriesAct;
    QAction *aboutCheckboardAct;
    QAction *aboutAct;
    QAction *editRefAct;
    QAction *separatorAct;
    QAction *editCutAct;
    QAction *editCopyAct;
    QAction *editAddToCopyAct;
    QAction *editPasteAct;
    QAction *editPasteRiggingAct;
    QAction *editPasteTimingsAct;
    QAction *editPasteModificationAct;
    QAction *editCutFrameAct;
    QAction *editCopyFrameAct;
    QAction *editPasteFrameAct;

    QAction *searchBrfAct;
    QAction *navigateLeftAct;
    QAction *navigateRightAct;
    QAction *navigateUpAct;
    QAction *navigateDownAct;
    QAction *refreshIniAct;
    QAction *computeUsedByAct;
    QAction *selectBrfDataAct;
    QAction *showUnrefTexturesAct;
    QAction *showModuleStatsAct;
    QAction *moduleSelectAct;
    QAction *exportNamesAct;

    QAction *mab2tldHeadAct;
    QAction *tld2mabHeadAct;
    QAction *mab2tldArmorAct;
    QAction *tld2mabArmorAct;
    QAction *tldMakeDwarfSlimAct;
    QAction *tldShrinkAroundBonesAct;

    QAction *checkIniAct;
    QAction *searchIniAct;

    QPair<ObjCoord , QString > navigationStack[3];

    int navigationStackPos;

    QAction *optionAfterMeshLoadMerge;
    QAction *optionAfterMeshLoadRecompute;
    QAction *optionAfterMeshLoadNothing;
    QAction *optionAssembleAniMatchVert;
    QAction *optionAssembleAniMatchTc;
    QAction *optionAssembleAniQuiverMode;
    QAction *optionAutoFixTextureOn;
    QAction *optionAutoFixTextureOff;
    QAction *optionAutoFixTextureInfo;
    QAction *optionAutoZoomUseGlobal;
    QAction *optionAutoZoomUseSelected;
    QAction *optionLanguage[6];
    QAction *optionLanguageCustom;
    QAction *optionInferMaterialOn;
    QAction *optionInferMaterialOff;
    QAction *optionAoBrightness[5];
    QAction *optionAoFromAbove[2];

    QAction *tldMenuAction;

    QAction
      *importStaticMeshAct,
      *importRiggedMeshAct,
      *importMovingMeshFrameAct,
      *importMovingMeshAct,
      *importSkeletonAct,
      *importAnimationAct,
      *importBodyAct,
      *importBrfAct,
      *addNewMaterialAct,
      *addNewShaderAct,
      *addNewTextureAct,
      *addNewUiPictureAct;

    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    bool maybeSave();
    void setModified(bool mod);
    void updateTitle();
    bool isModified;
    QString lastImpExpFormat;

    QLabel* modStatus; // widget in status bar

    // to tune colors
    void meshTuneColorUndo(bool storeUndo);

    bool easterTLD; // if true, use easteregg
    bool _importStaticMesh(QString s, vector<BrfMesh> &m, vector<bool> &wasMultiple);

};

#endif // MAINWINDOW_H
