

#include <QtGui>
#include "brfdata.h"
#include "selector.h"

#include "tablemodel.h"

Selector::Selector(QWidget *parent)
   : QTabWidget(parent)
{
  for (int i=0; i<N_TOKEN; i++) {
    tab[i]=NULL;
    //tabIndex[i]=-1;
  }
  connect(this, SIGNAL(currentChanged(int)),
          this, SLOT(onChanged()) );

  breakAniWithIniAct = new QAction(tr("Split via action.txt"), this);
  breakAniWithIniAct ->setStatusTip(tr("Split sequence following the action.txt file. A new \"action [after split].txt\" file is also produced, which use the new animation."));

  breakAniAct = new QAction(tr("Auto-split"), this);
  breakAniAct->setStatusTip(tr("Auto-split sequence into its separated chunks, separating it at lasge gaps in frames."));

  renameAct = new QAction(tr("Rename"), this);
  removeAct = new QAction(tr("Remove"), this);
  duplicateAct = new QAction(tr("Duplicate"), this);
  moveUpAct = new QAction(tr("Move up"), this);
  //moveUpAct->setShortcut(QString("u"));
  //moveUpAct->setShortcutContext(Qt::ApplicationShortcut);
  //this->setShortcutEnabled();
  moveDownAct = new QAction(tr("Move down"), this);
  //moveDownAct->setShortcut(QString("Alt+d"));

  exportImportMeshInfoAct = new QAction(tr("Info on mesh import/export"), this);
  exportStaticMeshAct = new QAction(tr("as a mesh"), this);
  exportStaticMeshAct->setStatusTip(tr("Export this model as a 3D static mesh (common formats)."));
  exportSkeletonModAct = new QAction(tr("as skeletal-modification-mesh"), this);

  exportAnyBrfAct = new QAction(tr("in a BRF"), this);
  exportAnyBrfAct->setStatusTip(tr("Export this object in a BRF file."));

  importAnyBrfAct = new QAction(tr("from a BRF"), this);
  importStaticMeshAct = new QAction(tr("from a mesh"), this);
  importSkeletonModAct = new QAction(tr("modify with a skeletal-modification-mesh"), this);



  connect(breakAniAct, SIGNAL(triggered()),this,SLOT(onBreakAni()));
  connect(breakAniWithIniAct, SIGNAL(triggered()),this,SLOT(onBreakAniWithIni()));

  connect(exportAnyBrfAct, SIGNAL(triggered()),parent,SLOT(exportBrf()));
  connect(exportStaticMeshAct, SIGNAL(triggered()),parent,SLOT(exportPly()));
  connect(exportSkeletonModAct, SIGNAL(triggered()),parent,SLOT(exportSkelMod()));

  connect(importStaticMeshAct, SIGNAL(triggered()),parent,SLOT(importMeshPly()));
  connect(importAnyBrfAct, SIGNAL(triggered()),parent,SLOT(importBrf()));
  connect(importSkeletonModAct, SIGNAL(triggered()),parent,SLOT(importSkelMod()));

  connect(moveUpAct, SIGNAL(triggered()), parent, SLOT(moveUpSel()));
  connect(moveDownAct, SIGNAL(triggered()), parent, SLOT(moveDownSel()));
  connect(removeAct, SIGNAL(triggered()), parent, SLOT(deleteSel()));
  connect(duplicateAct, SIGNAL(triggered()), parent, SLOT(duplicateSel()));
  //
  connect(renameAct, SIGNAL(triggered()), parent, SLOT(renameSel()));
  this->setMinimumWidth(200);

}

void Selector::onRenameSel(){
  QListView* tab = (QListView*)currentWidget();
  if (!tab) return;
  QModelIndexList li = tab->selectionModel()->selectedIndexes();
  if (li.size()!=1) return;
  QModelIndex i = li[0];
  //tab->e
  tab->edit(i);
}

int Selector::firstSelected() const{
  if (!this->currentWidget()) return -1;
  unsigned int s =
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes().size();
  if (s==0) return -1;
  return
      (((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes())[0].row();
}

int Selector::numSelected() const{
  if (!this->currentWidget()) return 0;
  return
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes().size();
}


void Selector::updateData(const BrfData &data){

  QListView* c;
  QModelIndexList li;
  TokenEnum w=TokenEnum(this->currentTabName());
  //int i=this->currentIndex();
  c=(QListView*)this->currentWidget();
  if (c) li=c->selectionModel()->selectedIndexes();

  setup(data);

  if (w==NONE) return;
  if (tab[w]) setCurrentWidget(tab[w]);
  c=(QListView*)this->currentWidget();
  if (c) {
    c->clearSelection();
    for (int i=0; i<li.size(); i++)
    c->selectionModel()->select(li[i],QItemSelectionModel::Select);
    c->setFocus();
  }

}

void Selector::moveSel(int dx){
  QListView* c=(QListView*)this->currentWidget();

  if (c) {
    QModelIndex li=c->selectionModel()->selectedIndexes()[0];
    QModelIndex lj=li.sibling(li.row()+dx,li.column());
    if (lj.isValid()) {
      c->clearSelection();
      c->selectionModel()->select(lj,QItemSelectionModel::Select);
    }
  }
}

int Selector::currentTabName() const{
  for (int i=0; i<N_TOKEN; i++)
    if (this->currentWidget()==tab[i]) return i;
  return NONE;
}

void Selector::contextMenuEvent(QContextMenuEvent *event)
{

   QMenu menu(this);

   if (!this->currentWidget()) {
     return;
   }
   if (!this->currentWidget()) { event->ignore(); return; }

   QModelIndexList sel=
       ((QListView*)(this->currentWidget()))->selectionModel()->selectedIndexes() ;

   bool onesel = sel.size()==1;
   bool nosel = sel.size()==0;
   int t = currentTabName();


   QString title ("title");
   menu.setTitle(title);

   if (onesel) {
     menu.addAction(renameAct);
     menu.addAction(removeAct);
     menu.addAction(duplicateAct);
     menu.addSeparator();
     menu.addAction(moveUpAct);
     menu.addAction(moveDownAct);
   }

   if (onesel) {

     menu.addSeparator();
     QMenu* exportMenu=menu.addMenu("Export");
     exportMenu->addAction(exportAnyBrfAct);
     if (t==MESH) exportMenu->addAction(exportStaticMeshAct);
     if (t==SKELETON) exportMenu->addAction(exportSkeletonModAct);
     //exportMenu->addSeparator();
     //if (t==MESH)
     //  exportMenu->addAction(exportImportMeshInfoAct);
   }
   if (onesel || nosel) {
     QMenu* importMenu=menu.addMenu("Import");
     importMenu->addAction(importAnyBrfAct);
     if (t==MESH) {
       importMenu->addAction(importStaticMeshAct);
       //importMenu->addSeparator();
       //importMenu->addAction(exportImportMeshInfoAct);
     }
     if (onesel) if (t==SKELETON) importMenu->addAction(importSkeletonModAct);
   }


   if (t==ANIMATION)
     if (onesel) {
       menu.addSeparator();
       menu.addAction(breakAniAct);
       menu.addAction(breakAniWithIniAct);
     }

   //menu.addAction(moveUp);
   //menu.addAction(moveDown);

   menu.exec(event->globalPos());
   event->accept();


 }


template<class BrfType>
void Selector::addBrfTab(const vector<BrfType>  &v){

  int ti = BrfType::tokenIndex();

  if (v.size()!=0) {
    TableModel *tableModel = new TableModel(this);
    for (unsigned int k=0; k<v.size(); k++) {
      tableModel->vec.push_back( QString( v[k].name ) );
    }

    if (tab[ti]) {
      delete tab[ti]; tab[ti]=NULL;
    }
    tab[ti] =// new QListWidget;
    new QListView;

    tab[ti]->setModel(tableModel);


    if (ti==MESH) tab[ti]->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(tab[ti]->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(onChanged()) );

    if (v.size()>0) tab[ti]->selectionModel()->select(
        tableModel->pleaseCreateIndex(0,0), QItemSelectionModel::Select
    );

    //tabIndex[ti]=
    addTab(tab[ti], tr("%1 (%2)")
           .arg( tokenTabName[ti] )
           .arg(v.size())
           );
  } else {
    //if (tabIndex[ti]>=0)
    //this->removeTab(tabIndex[ti]);
  }

}

void Selector::onBreakAniWithIni(){
  int s = tab[ANIMATION]->selectionModel()->selectedIndexes().constBegin()->row();
  emit breakAni(s,true);
}

void Selector::onBreakAni(){
  int s = tab[ANIMATION]->selectionModel()->selectedIndexes().constBegin()->row();
  emit breakAni(s,false);
}

void Selector::onChanged(){
  static QModelIndexList empty;
  for(int ti=0; ti<N_TOKEN; ti++) if (tab[ti]) {
    //if (tab[ti]) tab[ti]->clearSelection();

    if (this->currentWidget()==tab[ti]) {
      QItemSelectionModel * tmp = tab[ti]->selectionModel();
      assert(tmp);
      emit setSelection(
          tmp->selectedIndexes()
          , ti );
      return;
    }
  }
  emit setSelection(empty , NONE );

}



void Selector::setup(const BrfData &data){

  this->clear();

  addBrfTab<BrfMesh> (data.mesh);
  addBrfTab<BrfShader> (data.shader);
  addBrfTab<BrfTexture> (data.texture);
  addBrfTab<BrfMaterial> (data.material);
  addBrfTab<BrfSkeleton> (data.skeleton);
  addBrfTab<BrfAnimation> (data.animation);
  addBrfTab<BrfBody> (data.body);

  //static QModelIndexList *empty = new QModelIndexList();
  //emit setSelection(*empty , NONE );

}
