

#include <QtGui>
#include "brfdata.h"
#include "selector.h"

#include "tablemodel.h"



void Selector::addToRefMeshA(){ emit(addToRefMesh(0)); }
void Selector::addToRefMeshB(){ emit(addToRefMesh(1)); }
void Selector::addToRefMeshC(){ emit(addToRefMesh(2)); }
void Selector::addToRefMeshD(){ emit(addToRefMesh(3)); }
void Selector::addToRefMeshE(){ emit(addToRefMesh(4)); }
void Selector::addToRefMeshF(){ emit(addToRefMesh(5)); }
void Selector::addToRefMeshG(){ emit(addToRefMesh(6)); }
void Selector::addToRefMeshH(){ emit(addToRefMesh(7)); }
void Selector::addToRefMeshI(){ emit(addToRefMesh(8)); }
void Selector::addToRefMeshJ(){ emit(addToRefMesh(9)); }

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
  moveUpAct->setShortcut(QString("Alt+up"));
  addAction(moveUpAct);

  moveDownAct = new QAction(tr("Move down"), this);
  moveDownAct->setShortcut(QString("Alt+down"));
  addAction(moveDownAct);

  addToRefAnimAct = new QAction(tr("Add to reference animations"), this);
  addToRefAnimAct->setStatusTip(tr("Add this animation to reference animations (which OpenBrf can use to display rigged meshes)."));

  addToRefSkelAct = new QAction(tr("Add to reference skeletons"), this);
  addToRefSkelAct->setStatusTip(tr("Add this animation to reference skeletons."));

  for (int i=0; i<10; i++) {
    this->addToRefMeshAct[i]= new QAction(tr("set %1").arg(char('A'+i)), this);
    addToRefMeshAct[i]->setStatusTip(tr("Add this mesh to reference skins (which OpenBrf can use to display animation)."));

  }
  connect(addToRefMeshAct[0], SIGNAL(triggered()), this, SLOT(addToRefMeshA()));
  connect(addToRefMeshAct[1], SIGNAL(triggered()), this, SLOT(addToRefMeshB()));
  connect(addToRefMeshAct[2], SIGNAL(triggered()), this, SLOT(addToRefMeshC()));
  connect(addToRefMeshAct[3], SIGNAL(triggered()), this, SLOT(addToRefMeshD()));
  connect(addToRefMeshAct[4], SIGNAL(triggered()), this, SLOT(addToRefMeshE()));
  connect(addToRefMeshAct[5], SIGNAL(triggered()), this, SLOT(addToRefMeshF()));
  connect(addToRefMeshAct[6], SIGNAL(triggered()), this, SLOT(addToRefMeshG()));
  connect(addToRefMeshAct[7], SIGNAL(triggered()), this, SLOT(addToRefMeshH()));
  connect(addToRefMeshAct[8], SIGNAL(triggered()), this, SLOT(addToRefMeshI()));
  connect(addToRefMeshAct[9], SIGNAL(triggered()), this, SLOT(addToRefMeshJ()));
  connect(this, SIGNAL(addToRefMesh(int)), parent, SLOT(addToRefMesh(int)));

  exportBodyAct = new QAction(tr("Export"), this);

  exportImportMeshInfoAct = new QAction(tr("Info on mesh import/export"), this);

  exportStaticMeshAct = new QAction(tr("Export static mesh"), this);
  exportStaticMeshAct->setStatusTip(tr("Export this model (or this frame) as a 3D static mesh."));

  exportMovingMeshAct = new QAction(tr("Export vertex ani"), this);
  exportMovingMeshAct->setStatusTip(tr("Export this model as a mesh with vertex animation."));

  exportRiggedMeshAct = new QAction(tr("Export rigged mesh"), this);
  exportRiggedMeshAct->setStatusTip(tr("Export this model (or this frame) as a rigged mesh."));

  exportSkeletonAct = new QAction(tr("Export (nude) skeleton"), this);
  exportSkeletonAct ->setStatusTip(tr("Export this skeleton (as a set of nude bones)."));
  exportSkinAct = new QAction(tr("Export skeleton with skin"), this);
  exportSkinAct->setStatusTip(tr("Export this skeleton (as a rigged skin)."));
  exportSkinForAnimationAct     = new QAction(tr("Export a skin for this ani"), this);
  exportSkinForAnimationAct->setStatusTip(tr("Export a rigged skin which can be used for this animation."));

  exportAnimationAct = new QAction(tr("Export animation"), this);
  exportAnimationAct->setStatusTip(tr("Export this animation."));

  reskeletonizeAct = new QAction(tr("Reskeletonize"), this);
  reskeletonizeAct->setStatusTip(tr("Adapt this rigged mesh to a new skeleton"));

  meshRecomputeNormalsAndUnify = new QAction(tr("Recompute normals"), this);
  meshRecomputeNormalsAndUnify->setStatusTip(tr("Recompute normals for this model, and unify pos and vertices"));

  meshUnify = new QAction(tr("Unify vertices"), this);
  meshUnify->setStatusTip(tr("Merges together identical vertices and pos."));



  //exportAnyBrfAct = new QAction(tr("in a BRF"), this);
  //exportAnyBrfAct->setStatusTip(tr("Export this object in a BRF file."));

  exportSkeletonModAct = new QAction(tr("Make a skeleton-modification mesh"), this);
  importSkeletonModAct = new QAction(tr("Modify from a skeleton-modification mesh"), this);

  connect(breakAniAct, SIGNAL(triggered()),this,SLOT(onBreakAni()));
  connect(breakAniWithIniAct, SIGNAL(triggered()),this,SLOT(onBreakAniWithIni()));
  connect(meshRecomputeNormalsAndUnify,  SIGNAL(triggered()),parent,SLOT(meshRecomputeNormalsAndUnify()));
  connect(meshUnify,  SIGNAL(triggered()),parent,SLOT(meshUnify()));


  //connect(exportAnyBrfAct, SIGNAL(triggered()),parent,SLOT(exportBrf()));
  connect(exportStaticMeshAct, SIGNAL(triggered()),parent,SLOT(exportStaticMesh()));
  connect(exportRiggedMeshAct, SIGNAL(triggered()),parent,SLOT(exportRiggedMesh()));
  connect(exportMovingMeshAct, SIGNAL(triggered()),parent,SLOT(exportMovingMesh()));
  connect(exportSkeletonModAct, SIGNAL(triggered()),parent,SLOT(exportSkeletonMod()));
  connect(exportSkeletonAct, SIGNAL(triggered()),parent,SLOT(exportSkeleton()));
  connect(exportAnimationAct, SIGNAL(triggered()),parent,SLOT(exportAnimation()));
  connect(reskeletonizeAct, SIGNAL(triggered()),parent,SLOT(reskeletonize()));
  connect(exportBodyAct, SIGNAL(triggered()), parent, SLOT(exportCollisionBody()));

  connect(exportSkinAct, SIGNAL(triggered()), parent, SLOT(exportSkeletonAndSkin()));
  connect(exportSkinForAnimationAct, SIGNAL(triggered()), parent, SLOT(exportSkeletonAndSkin()));

  connect(importSkeletonModAct, SIGNAL(triggered()),parent,SLOT(importSkeletonMod()));

  connect(moveUpAct, SIGNAL(triggered()), parent, SLOT(moveUpSel()));
  connect(moveDownAct, SIGNAL(triggered()), parent, SLOT(moveDownSel()));
  connect(removeAct, SIGNAL(triggered()), parent, SLOT(deleteSel()));
  connect(duplicateAct, SIGNAL(triggered()), parent, SLOT(duplicateSel()));
  //
  connect(renameAct, SIGNAL(triggered()), parent, SLOT(renameSel()));


  connect(addToRefAnimAct, SIGNAL(triggered()), parent, SLOT(addToRef()));
  connect(addToRefSkelAct, SIGNAL(triggered()), parent, SLOT(addToRef()));
  this->setMinimumWidth(200);

  // prepare all tabs (w/o attaching them for now)
  for (int ti=0; ti<N_TOKEN; ti++) {

    tab[ti] =// new QListWidget;
    new QListView;

    tableModel[ti] = new TableModel(this);

    tab[ti]->setModel(tableModel[ti]);

    if (ti==MESH || ti==MATERIAL ) tab[ti]->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(tab[ti]->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(onChanged()) );

  }
}

/*
void Selector::onRenameSel(){
  QListView* tab = (QListView*)currentWidget();
  if (!tab) return;
  QModelIndexList li = tab->selectionModel()->selectedIndexes();
  if (li.size()!=1) return;
  QModelIndex i = li[0];
  //tab->e
  tab->edit(i);
}
*/

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

QModelIndexList Selector::selectedList() const{
  if (!this->currentWidget()) return QModelIndexList();
  return
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes();
}

int Selector::onlySelected(int kind) const{
  if (!currentWidget()) return -1;
  if (currentTabName()!=kind) return -1;
  if (numSelected()!=1) return -1;
  int i= ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes()[0].row();
  return i;
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

  c=(QListView*)this->currentWidget();
  if (c) li=c->selectionModel()->selectedIndexes();

  TokenEnum w=TokenEnum(this->currentTabName());
  setup(data);


  if (w==NONE) return;
  if (tab[w]) setCurrentWidget(tab[w]);

}


void Selector::selectOne(int kind, int i){

  assert(kind>=0 && kind<N_TOKEN);
  QListView* c=tab[kind];
  if (c) {
    c->clearSelection();
    QModelIndex li = tableModel[kind]->pleaseCreateIndex(i,0);
    c->selectionModel()->setCurrentIndex(li,QItemSelectionModel::NoUpdate);
    c->selectionModel()->select(li,QItemSelectionModel::Select);
    c->scrollTo(li,QAbstractItemView::PositionAtCenter);

  }
  this->setCurrentWidget(tab[kind]);
  onChanged();
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

   if (!this->currentWidget()) { event->ignore(); return; }

   QModelIndexList sel=
       ((QListView*)(this->currentWidget()))->selectionModel()->selectedIndexes() ;

   bool onesel = sel.size()==1;
   bool nosel = sel.size()==0;
   int seli = 0;
   if (!nosel) seli = sel[0].row();
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

     if (t==MESH) {
       menu.addAction(exportStaticMeshAct);
       if (data->mesh[ seli ].isRigged)
         menu.addAction(exportRiggedMeshAct);
       //if (data->mesh[ seli ].frame.size()>0)
       //  menu.addAction(exportMovingMeshAct);
     }

     if (t==SKELETON) {
       menu.addAction(exportSkeletonAct);
       menu.addAction(exportSkinAct);
       menu.addSeparator();
       menu.addAction(exportSkeletonModAct);
       menu.addAction(importSkeletonModAct);
     }
     if (t==ANIMATION){
       menu.addAction(exportAnimationAct);
       menu.addAction(exportSkinForAnimationAct);
     }
     if (t==BODY) {
       menu.addAction(exportBodyAct);
     }
   }

   // tool section
   bool sep = false;
   if (t==MESH) {
     if (data->mesh[ seli ].isRigged) {
       if (!sep) menu.addSeparator(); sep=true;
       menu.addAction(reskeletonizeAct);
     }
     if (!sep) menu.addSeparator(); sep=true;
     menu.addAction(meshRecomputeNormalsAndUnify);
     menu.addAction(meshUnify);
   }
   if (t==ANIMATION) {
     if (onesel) {
       if (!sep) menu.addSeparator(); sep=true;
       menu.addAction(breakAniAct);
       menu.addAction(breakAniWithIniAct);
     }
   }

   // add to reference
   if (onesel && t==MESH) {
     menu.addSeparator();
     QMenu* refMenu=menu.addMenu("Add to reference skins");
     int N=reference->GetFirstUnusedLetter();
     for (int i=0; i<N; i++) {
       addToRefMeshAct[i]->setText(tr("to Skin Set %1").arg(char('A'+i)) );
       refMenu->addAction(addToRefMeshAct[i]);
     }
     if (N<10) {
       addToRefMeshAct[N]->setText(tr("to Skin Set %1 [new set]").arg(char('A'+N)) );
       refMenu->addAction(addToRefMeshAct[N]);
     }
   }
   if (onesel && t==ANIMATION) {
     menu.addSeparator();
     menu.addAction(addToRefAnimAct);
   }
   if (onesel && t==ANIMATION) {
     menu.addSeparator();
     menu.addAction(addToRefSkelAct);
   }




   //menu.addAction(moveUp);
   //menu.addAction(moveDown);

   menu.exec(event->globalPos());
   event->accept();


 }


template<class BrfType>
void Selector::addBrfTab(const vector<BrfType>  &v){

  int ti = BrfType::tokenIndex();

  tableModel[ti]->clear();
  if (v.size()!=0) {

    for (unsigned int k=0; k<v.size(); k++) {
      tableModel[ti]->vec.push_back( QString( v[k].name ) );
    }
    //tab[ti]->setModel(tableModel[ti]);

    tableModel[ti]->updateChanges();

    tab[ti]->viewport()->update();

    if (tab[ti]->selectionModel()->selectedIndexes().size()==0)
    tab[ti]->selectionModel()->select(
        tableModel[ti]->pleaseCreateIndex(0,0),
        QItemSelectionModel::Select
    );


  } else {
  }

}

void Selector::hideEmpty(){

  int n=0;
  for (int i=0; i<N_TOKEN; i++){
    if (tableModel[i]->size()) n++;
  }

  for (int i=0; i<N_TOKEN; i++) {
    int v = tableModel[i]->size();
    if (v==0) {
      int j = indexOf( tab[i] );
      if (j>=0) this->removeTab(  j );
    } else {
      QString title(tokenTabName[i]);
      if (n>3) title.truncate(3);
      else if (n>2) title.truncate(4);
      if (n>3)
        title = QString("%1%2").arg( title ).arg(v);
      else title = QString("%1(%2)").arg( title ).arg(v);
      addTab(tab[i], title);
    }
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




void Selector::setup(const BrfData &_data){


  addBrfTab<BrfMesh> (_data.mesh);
  addBrfTab<BrfShader> (_data.shader);
  addBrfTab<BrfTexture> (_data.texture);
  addBrfTab<BrfMaterial> (_data.material);
  addBrfTab<BrfSkeleton> (_data.skeleton);
  addBrfTab<BrfAnimation> (_data.animation);
  addBrfTab<BrfBody> (_data.body);
  data = &_data;

  hideEmpty();
  onChanged();

  //static QModelIndexList *empty = new QModelIndexList();
  //emit setSelection(*empty , NONE );

}
