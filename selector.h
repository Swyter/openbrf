#ifndef SELECTOR_H
#define SELECTOR_H

#include <QTabWidget>
#include <QItemSelection>
#include "brfToken.h"

class BrfData;
class IniData;
class QListView;
class QMenu;
class TableModel;

class Selector : public QTabWidget
{
  Q_OBJECT

public:
  Selector(QWidget *parent=0);
  void setup(const BrfData &data);
  void updateData(const BrfData &data);
  void setIniData(const IniData* data, int fileIndex);
  int currentTabName() const;
  int firstSelected() const;
  int lastSelected() const;
  int numSelected() const;
  int onlySelected(int kind) const; // return index of only selected object of given kind or -1
  QModelIndexList selectedList() const;
  void moveSel(int d);
  BrfData* reference;
  const BrfData* data;
  const IniData* iniData;
  bool iniDataWaitsSaving;
  int iniFileIndex;
  void selectOne(int kind, int i);
  QMenu *contextMenu;


private slots:
  void onChanged();
  void onBreakAni();
  void onBreakAniWithIni();
  void addToRefMeshA();
  void addToRefMeshB();
  void addToRefMeshC();
  void addToRefMeshD();
  void addToRefMeshE();
  void addToRefMeshF();
  void addToRefMeshG();
  void addToRefMeshH();
  void addToRefMeshI();
  void addToRefMeshJ();
  void goNextTab();
  void goPrevTab();

signals:
  void setSelection(const QModelIndexList & newSel, int newToken);
  void addToRefMesh(int refIndex);
  void breakAni(int i, bool useIni);

private:
  void hideEmpty();
  template<class BrfType> void addBrfTab(const std::vector<BrfType> &x);
  QListView * tab[N_TOKEN];
  TableModel * tableModel[N_TOKEN];
  void contextMenuEvent(QContextMenuEvent *event);
  enum {MAX_USED_BY = 50};
  QAction
    *goNextTabAct,
    *goPrevTabAct,

     // tools
    *aniExtractIntervalAct,
    *aniRemoveIntervalAct,
    *aniMergeAct,

    *breakAniAct,
    *breakAniWithIniAct,
    *meshRecomputeNormalsAndUnify,
    *meshUnify,
    *meshMerge,
    *meshToBody,
    *meshMountOnBone,
    *meshRemoveBackfacing,
    *meshAddBackfacing,
    *meshRecolorAct,
    *meshRecomputeTangentsAct,
    *meshTuneColorAct,
    *meshComputeAoAct,
    *meshFemininizeAct,
    *meshComputeLodAct,


    *renameAct,
    *removeAct,
    *moveUpAct,
    *moveDownAct,
    *duplicateAct,
    *discardColAct,
    *discardRigAct,
    *discardTanAct,
    *discardAniAct,


    // exporter acts
    *exportImportMeshInfoAct,
    *exportStaticMeshAct,
    *exportRiggedMeshAct,
    *exportMovingMeshFrameAct,
    *exportMovingMeshAct,
    *exportMeshGroupAct,
    *exportMeshGroupManyFilesAct,

    *exportSkeletonModAct,
    *exportSkeletonAct,
    *exportSkinAct,
    *exportSkinForAnimationAct,
    *exportAnimationAct,
    *exportBodyAct,


    *reskeletonizeAct,
    *transferRiggingAct,
    *flipAct,
    *transformAct,
    *scaleAct,
    *shiftAniAct,
    *bodyMakeQuadDominantAct,
    *bodyMerge,


    *sortEntriesAct,


    // importer acts
    *importSkeletonModAct,

    *addToRefSkelAct,
    *addToRefAnimAct,
    *addToRefMeshAct[10],
    *usedByAct[MAX_USED_BY],
    *usedByComputeAct,
    *usedByNoneAct,
    *usedInCoreAct[2],
    *usedInNoTxtAct,
    *usedInAct[N_TXTFILES*2],
    *usedIn_NotInModule,
    *usedIn_SaveFirst;

};

#endif // SELECTOR_H
