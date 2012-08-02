#ifndef SELECTOR_H
#define SELECTOR_H

#include <QTabWidget>
#include <QItemSelection>
#include "BrfToken.h"

class BrfData;
class QListView;

class Selector : public QTabWidget
{
  Q_OBJECT

public:
  Selector(QWidget *parent=0);
  void setup(const BrfData &data);
  void updateData(const BrfData &data);
  int currentTabName() const;
  int firstSelected() const;
  int numSelected() const;
  void moveSel(int d);

private slots:
  void onChanged();
  void onBreakAni();
  void onBreakAniWithIni();
  void onRenameSel();

signals:
  //void selectionChanged (const QItemSelection &selected, const QItemSelection &now);
  void setSelection(const QModelIndexList & newSel, int newToken);
  void breakAni(int i, bool useIni);

private:
  template<class BrfType> void addBrfTab(const std::vector<BrfType> &x);
  QListView * tab[N_TOKEN];
  void contextMenuEvent(QContextMenuEvent *event);
  QAction
    *breakAniAct,
    *breakAniWithIniAct,
    *renameAct,
    *removeAct,
    *moveUpAct,
    *moveDownAct,
    *duplicateAct,
    // exporter acts
    *exportImportMeshInfoAct,
    *exportStaticMeshAct,
    *exportSkeletonModAct,
    *exportAnyBrfAct,
    // importer acts
    *importStaticMeshAct,
    *importSkeletonModAct,
    *importAnyBrfAct;
;
};

#endif // SELECTOR_H
