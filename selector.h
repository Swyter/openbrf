#ifndef SELECTOR_H
#define SELECTOR_H

#include <QTabWidget>
#include <QItemSelection>
#include "BrfToken.h"

class BrfData;
class QListView;
class TableModel;

class Selector : public QTabWidget
{
  Q_OBJECT

public:
  Selector(QWidget *parent=0);
  void setup(const BrfData &data);
  void updateData(const BrfData &data);
  int currentTabName() const;
  int firstSelected() const;
  int lastSelected() const;
  int numSelected() const;
  int onlySelected(int kind) const; // return index of only selected object of given kind or -1
  QModelIndexList selectedList() const;
  void moveSel(int d);
  BrfData* reference;
  const BrfData* data;
  void selectOne(int kind, int i);

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
  QAction
    *goNextTabAct,
    *goPrevTabAct,

     // tools
    *breakAniAct,
    *breakAniWithIniAct,
    *meshRecomputeNormalsAndUnify,
    *meshUnify,
    *meshMerge,
    *meshMountOnBone,
    *meshRemoveBackfacing,
    *meshAddBackfacing,

    *renameAct,
    *removeAct,
    *moveUpAct,
    *moveDownAct,
    *duplicateAct,


    // exporter acts
    *exportImportMeshInfoAct,
    *exportStaticMeshAct,
    *exportRiggedMeshAct,
    *exportMovingMeshAct,
    *exportMeshGroupAct,
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


    // importer acts
    *importSkeletonModAct,

    *addToRefSkelAct,
    *addToRefAnimAct,
    *addToRefMeshAct[10];

};

#endif // SELECTOR_H
