

#include <QtGui>
#include "brfdata.h"
#include "selector.h"
#include "iniData.h"
#include "tablemodel.h"

char * tokenTabName[N_TOKEN] = {
 QT_TRANSLATE_NOOP("Selector", "&Mesh"),
 QT_TRANSLATE_NOOP("Selector", "Te&xture"),
 QT_TRANSLATE_NOOP("Selector", "&Shader"),
 QT_TRANSLATE_NOOP("Selector", "Mat&erial"),
 QT_TRANSLATE_NOOP("Selector", "S&keleton"),
 QT_TRANSLATE_NOOP("Selector", "&Animation"),
 QT_TRANSLATE_NOOP("Selector", "&Collision"),
};


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
  iniData = NULL;
  for (int i=0; i<N_TOKEN; i++) {
    tab[i]=NULL;
    //tabIndex[i]=-1;
  }
  iniDataWaitsSaving = false;

  contextMenu = new QMenu(this);
  connect(contextMenu, SIGNAL(triggered(QAction *)),parent, SLOT(onActionTriggered(QAction *)));


  connect(this, SIGNAL(currentChanged(int)),
          this, SLOT(onChanged()) );

  breakAniWithIniAct = new QAction(tr("Split via action.txt"), this);
  breakAniWithIniAct ->setStatusTip(tr("Split sequence following the action.txt file. A new \"action [after split].txt\" file is also produced, which use the new animation."));

  breakAniAct = new QAction(tr("Auto-split"), this);
  breakAniAct->setStatusTip(tr("Auto-split sequence into its separated chunks, separating it at lasge gaps in frames."));


  aniExtractIntervalAct = new QAction(tr("Extract interval..."), this);
  aniExtractIntervalAct->setStatusTip(tr("Extract an animation from an interval of times."));

  aniRemoveIntervalAct = new QAction(tr("Remove interval..."), this);
  aniRemoveIntervalAct->setStatusTip(tr("Remove an interval of times from the animation."));

  aniMergeAct = new QAction(tr("Merge animations"), this);
  aniMergeAct->setStatusTip(tr("Merge two animations into one -- intervals must be right!"));

  renameAct = new QAction(tr("Rename"), this);
  renameAct->setShortcut(QString("F2"));
  addAction(renameAct);

  removeAct = new QAction(tr("Remove"), this);
  removeAct->setShortcut(QString("delete"));
  addAction(removeAct);

  duplicateAct = new QAction(tr("Duplicate"), this);

  goNextTabAct = new QAction(tr("next tab"),this);
  goNextTabAct->setShortcut(QString("right"));
  addAction(goNextTabAct);

  goPrevTabAct = new QAction(tr("left tab"),this);
  goPrevTabAct->setShortcut(QString("left"));
  addAction(goPrevTabAct);

  moveUpAct = new QAction(tr("Move up"), this);
  moveUpAct->setShortcut(QString("Alt+up"));
  moveUpAct->setStatusTip(tr("Move this object upward in the list"));
  addAction(moveUpAct);

  moveDownAct = new QAction(tr("Move down"), this);
  moveDownAct->setShortcut(QString("Alt+down"));
  moveDownAct->setStatusTip(tr("Move this object one step down in the list"));
  addAction(moveDownAct);

  addToRefAnimAct = new QAction(tr("Add to reference animations"), this);
  addToRefAnimAct->setStatusTip(tr("Add this animation to reference animations (to use it later to display rigged meshes)."));

  addToRefSkelAct = new QAction(tr("Add to reference skeletons"), this);
  addToRefSkelAct->setStatusTip(tr("Add this animation to reference skeletons (to use it later for animations)."));

  for (int i=0; i<10; i++) {
    this->addToRefMeshAct[i]= new QAction(tr("set %1").arg(char('A'+i)), this);
    addToRefMeshAct[i]->setStatusTip(tr("Add this mesh to reference skins (to use it later to display animations)."));

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

  for (int i=0; i<MAX_USED_BY; i++){
    usedByAct[i] = new QAction("???",this);
    usedByAct[i]->setProperty("id",i);
    connect(usedByAct[i], SIGNAL(triggered()),  parent, SLOT(goUsedBy()));
  }

  QFont fontIta(QApplication::font());
  fontIta.setItalic(!fontIta.italic());

  usedByNoneAct = new QAction(tr("<none>"),this);
  usedByNoneAct->setFont( fontIta );

  for (int i=0; i<N_TXTFILES; i++){
    usedInAct[i]
        = new QAction(tr("mod file <%1>").arg(txtFileName[i]),this);
    usedInAct[i+N_TXTFILES]
        = new QAction(tr("mod file <%1> (indirectly)").arg(txtFileName[i]),this);
    usedInAct[i]->setFont( fontIta );
    usedInAct[i+N_TXTFILES]->setFont( fontIta );
  }
  usedInNoTxtAct = new QAction(tr("<no .txt file>"),this);
  usedInNoTxtAct->setFont(fontIta);
  usedInCoreAct[0] = new QAction(tr("<core engine>"),this);
  usedInCoreAct[0]->setFont(fontIta);
  usedInCoreAct[1] = new QAction(tr("<core engine> (indirectly)"),this);
  usedInCoreAct[1]->setFont(fontIta);

  usedIn_NotInModule = new QAction(tr("<not in module.ini>"),this);
  usedIn_NotInModule->setFont(fontIta);
  usedIn_SaveFirst = new QAction(tr("<save file to find out>"),this);
  usedIn_SaveFirst->setFont(fontIta);

  usedByComputeAct = new QAction(tr("(not computed: compute now)"),this);
  connect(usedByComputeAct, SIGNAL(triggered()),parent, SLOT(computeUsedBy()));


  exportBodyAct = new QAction(tr("Export"), this);

  exportImportMeshInfoAct = new QAction(tr("Info on mesh import/export"), this);

  exportStaticMeshAct = new QAction(tr("Export static mesh"), this);
  exportStaticMeshAct->setStatusTip(tr("Export this model (or this frame) as a 3D static mesh."));

  exportMovingMeshAct = new QAction(tr("Export vertex ani"), this);
  exportMovingMeshAct->setStatusTip(tr("Export this model as a mesh with vertex animation."));

  exportMeshGroupAct = new QAction(tr("Export combined mesh"), this);
  exportMeshGroupAct->setStatusTip(tr("Export this group of models in a single OBJ."));
  exportMeshGroupManyFilesAct = new QAction(tr("Export all meshes"), this);
  exportMeshGroupManyFilesAct->setStatusTip(tr("Export each of these models as separate OBJs."));

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

  transferRiggingAct = new QAction(tr("Transfer rigging"), this);
  transferRiggingAct->setStatusTip(tr("Copy rigging from one mesh to another"));


  flipAct = new QAction(tr("Mirror"), this);
  flipAct->setStatusTip(tr("Mirror this object on the X axis."));

  transformAct = new QAction(tr("Roto-translate-rescale"),this);
  flipAct->setStatusTip(tr("Apply a gemoetric transform."));

  scaleAct = new QAction(tr("Rescale"), this);
  scaleAct->setStatusTip(tr("Rescale this object."));

  shiftAniAct = new QAction(tr("Shift time interval"), this);
  shiftAniAct->setStatusTip(tr("Shift a time interval for this animation"));

  bodyMakeQuadDominantAct = new QAction(tr("Make quad-dominant"), this);
  bodyMakeQuadDominantAct->setStatusTip(tr("Try to merge most triangles into fewer quads (more efficient!)"));
  bodyMerge = new QAction(tr("Combine collision objects"), this);
  bodyMerge->setStatusTip(tr("Make a combined collision obj. unifying these objs."));

  meshRecomputeNormalsAndUnify = new QAction(tr("Recompute normals"), this);
  meshRecomputeNormalsAndUnify->setStatusTip(tr("Recompute normals for this model, and unify pos and vertices"));

  meshUnify = new QAction(tr("Unify vertices"), this);
  meshUnify->setStatusTip(tr("Unify identical vertices and pos."));

  meshMerge = new QAction(tr("Combine meshes"), this);
  meshMerge->setStatusTip(tr("Make a combined mesh unifying these meshes."));

  meshToBody = new QAction(tr("Make a collision object"), this);
  meshToBody->setStatusTip(tr("Turn this mesh(es) into a combined Collision object."));

  meshMountOnBone = new QAction(tr("Mount on one bone"), this);
  meshMountOnBone->setStatusTip(tr("Put this mesh on top of a single skeleton bone."));

  meshRemoveBackfacing = new QAction(tr("remove all"), this);
  meshRemoveBackfacing->setStatusTip(tr("Remove all faces that are backfacing (e.g. in beard meshes)."));
  meshAddBackfacing = new QAction(tr("add (x2 faces)"), this);
  meshAddBackfacing->setStatusTip(tr("Duplicate all faces: for each current face, add a backfacing face."));

  meshComputeAoAct = new QAction(tr("Color with Ambient Occlusion"), this);
  meshComputeAoAct->setStatusTip(tr("Set per vertex color as ambient occlusion (globlal lighting)"));

  meshFemininizeAct = new QAction(tr("Add femininized frame"),this);
  meshFemininizeAct->setStatusTip(tr("Build a feminine frame for this armour(s)"));

  meshRecolorAct = new QAction(tr("Color uniform"), this);
  meshRecolorAct->setStatusTip(tr("Set per vertex color as a uniform color"));

  meshTuneColorAct = new QAction(tr("Tune colors HSB"), this);
  meshTuneColorAct->setStatusTip(tr("Then Hue Saturation and Brightness of per-vertex colors"));

  discardColAct = new QAction(tr("per-vertex color"), this);
  discardRigAct = new QAction(tr("rigging"), this);
  discardAniAct = new QAction(tr("vertex animation"), this);
  discardAniAct->setStatusTip(tr("Discard vertex animation (keeps only current frame)"));
  //exportAnyBrfAct = new QAction(tr("in a BRF"), this);
  //exportAnyBrfAct->setStatusTip(tr("Export this object in a BRF file."));

  exportSkeletonModAct = new QAction(tr("Make a skeleton-modification mesh"), this);
  importSkeletonModAct = new QAction(tr("Modify from a skeleton-modification mesh"), this);

  connect(goNextTabAct, SIGNAL(triggered()),this,SLOT(goNextTab()));
  connect(goPrevTabAct, SIGNAL(triggered()),this,SLOT(goPrevTab()));

  connect(breakAniAct, SIGNAL(triggered()),this,SLOT(onBreakAni()));
  connect(aniExtractIntervalAct, SIGNAL(triggered()),parent,SLOT(aniExtractInterval()));
  connect(aniRemoveIntervalAct, SIGNAL(triggered()),parent,SLOT(aniRemoveInterval()));
  connect(aniMergeAct, SIGNAL(triggered()),parent,SLOT(aniMerge()));
  connect(breakAniWithIniAct, SIGNAL(triggered()),this,SLOT(onBreakAniWithIni()));
  connect(meshRecolorAct,SIGNAL(triggered()),parent,SLOT(meshRecolor()));
  connect(meshTuneColorAct,SIGNAL(triggered()),parent,SLOT(meshTuneColor()));
  connect(meshComputeAoAct, SIGNAL(triggered()), parent, SLOT(meshComputeAo()));
  connect(meshFemininizeAct,SIGNAL(triggered()),parent,SLOT(meshFemininize()));
  connect(meshRecomputeNormalsAndUnify,  SIGNAL(triggered()),parent,SLOT(meshRecomputeNormalsAndUnify()));
  connect(meshUnify,  SIGNAL(triggered()),parent,SLOT(meshUnify()));
  connect(meshMerge,  SIGNAL(triggered()),parent,SLOT(meshMerge()));
  connect(bodyMerge,  SIGNAL(triggered()),parent,SLOT(bodyMerge()));
  connect(meshToBody,SIGNAL(triggered()),parent,SLOT(meshToBody()));
  connect(meshMountOnBone,SIGNAL(triggered()),parent,SLOT(meshMountOnBone()));
  connect(meshRemoveBackfacing,SIGNAL(triggered()),parent,SLOT(meshRemoveBack()));
  connect(meshAddBackfacing,SIGNAL(triggered()),parent,SLOT(meshAddBack()));


  //connect(exportAnyBrfAct, SIGNAL(triggered()),parent,SLOT(exportBrf()));
  connect(exportStaticMeshAct, SIGNAL(triggered()),parent,SLOT(exportStaticMesh()));
  connect(exportRiggedMeshAct, SIGNAL(triggered()),parent,SLOT(exportRiggedMesh()));
  connect(exportMovingMeshAct, SIGNAL(triggered()),parent,SLOT(exportMovingMesh()));
  connect(exportMeshGroupAct, SIGNAL(triggered()),parent,SLOT(exportMeshGroup()));
  connect(exportMeshGroupManyFilesAct, SIGNAL(triggered()),parent,SLOT(exportMeshGroupManyFiles()));
  connect(exportSkeletonModAct, SIGNAL(triggered()),parent,SLOT(exportSkeletonMod()));
  connect(exportSkeletonAct, SIGNAL(triggered()),parent,SLOT(exportSkeleton()));
  connect(exportAnimationAct, SIGNAL(triggered()),parent,SLOT(exportAnimation()));
  connect(reskeletonizeAct, SIGNAL(triggered()),parent,SLOT(reskeletonize()));
  connect(transferRiggingAct, SIGNAL(triggered()),parent,SLOT(transferRigging()));

  connect(flipAct, SIGNAL(triggered()),parent,SLOT(flip()));
  connect(transformAct, SIGNAL(triggered()),parent,SLOT(transform()));
  connect(scaleAct, SIGNAL(triggered()),parent,SLOT(scale()));
  connect(exportBodyAct, SIGNAL(triggered()), parent, SLOT(exportCollisionBody()));
  connect(shiftAniAct, SIGNAL(triggered()),parent,SLOT(shiftAni()));
  connect(bodyMakeQuadDominantAct, SIGNAL(triggered()),parent,SLOT(bodyMakeQuadDominant()));

  connect(discardAniAct,SIGNAL(triggered()),parent,SLOT(meshDiscardAni()));
  connect(discardColAct,SIGNAL(triggered()),parent,SLOT(meshDiscardCol()));
  connect(discardRigAct,SIGNAL(triggered()),parent,SLOT(meshDiscardRig()));

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

    if (ti==MESH || ti==MATERIAL || ti==BODY || ti==TEXTURE || ti==SKELETON || ti==ANIMATION) {
      tab[ti]->setSelectionMode(QAbstractItemView::ExtendedSelection);
      tab[ti]->setStatusTip(QString(tr("[Right-Click]: tools for %1. Multiple selections with [Shift]-[Ctrl].")).arg(IniData::tokenFullName(ti)));
    }
    else
        tab[ti]->setStatusTip(QString(tr("[Right-Click]: tools for %1.")).arg(IniData::tokenFullName(ti)));

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

int Selector::lastSelected() const{
  if (!this->currentWidget()) return -1;
  unsigned int s =
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes().size();
  if (s==0) return -1;
  return
      (((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes())[s-1].row();
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

  setup(data);

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

    this->setCurrentWidget(c);
    c->setFocus();

  }
  onChanged();
}


void Selector::goNextTab(){
  int j=currentIndex();
  if (j<count()) setCurrentIndex(j+1);
}

void Selector::goPrevTab(){
  int j=currentIndex();
  if (j>0) setCurrentIndex(j-1);
}

void Selector::moveSel(int dx){
  QListView* c=(QListView*)this->currentWidget();

  if (c) {
    QModelIndex li=c->selectionModel()->selectedIndexes()[0];
    QModelIndex lj=li.sibling(li.row()+dx,li.column());
    if (lj.isValid()) {
      c->clearSelection();
      c->selectionModel()->select(lj,QItemSelectionModel::Select);
      c->selectionModel()->setCurrentIndex(lj,QItemSelectionModel::NoUpdate);
      c->setFocus();
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
   contextMenu->clear();

   if (!this->currentWidget()) { event->ignore(); return; }

   QModelIndexList sel=
       ((QListView*)(this->currentWidget()))->selectionModel()->selectedIndexes() ;

   bool onesel = sel.size()==1;
   bool mulsel = sel.size()>1;
   bool nosel = sel.size()==0;
   int seli = 0;
   if (!nosel) seli = sel[0].row();
   int t = currentTabName();


   QString title ("title");
   contextMenu->setTitle(title);

   contextMenu->addAction(removeAct);
   if (onesel) renameAct->setText(tr("Rename")); else renameAct->setText(tr("Group rename"));
   contextMenu->addAction(renameAct);
   if (onesel) {
     contextMenu->addAction(duplicateAct);
     //contextMenu->addSeparator();
     contextMenu->addAction(moveUpAct);
     contextMenu->addAction(moveDownAct);
   }

   if (onesel){

      //contextMenu->addSeparator();
      QMenu *m = contextMenu->addMenu(tr("Used by..."));
      if (iniDataWaitsSaving)
        m->addAction(usedIn_SaveFirst);
      else
      if (!iniData) {
          m->addAction(usedIn_NotInModule);

      } else {
        if (iniData->updated<4) {
          m->addAction( usedByComputeAct );
        } else {
          ObjCoord c(iniFileIndex,seli,t);
          const std::vector< ObjCoord > &s ( iniData->usedBy(c) );
          IniData::UsedInType ui = iniData->usedIn(c);
          for (int i=0; i<(int)N_TXTFILES; i++){
            if (ui.direct & bitMask(i)) m->addAction(usedInAct[i]); else
            if (ui.indirect & bitMask(i)) m->addAction(usedInAct[i+N_TXTFILES]);
          }
          if (ui.direct & bitMask(TXTFILE_CORE)) m->addAction(usedInCoreAct[0]); else
          if (ui.indirect & bitMask(TXTFILE_CORE)) m->addAction(usedInCoreAct[1]);
          if ((m->actions().size()==0) && (s.size()!=0)){
            m->addAction( usedInNoTxtAct );
          }
          for (unsigned int i=0; i<s.size(); i++) if (i<MAX_USED_BY){
            usedByAct[i]->setText(iniData->nameFull( s[i] ));
            m->addAction( usedByAct[i] );
          }
          if (m->actions().size()==0){
            m->addAction( usedByNoneAct );
          }
        }
      }

   }

   if (onesel) {

     contextMenu->addSeparator();

     if (t==MESH) {
       contextMenu->addAction(exportStaticMeshAct);
       if (data->mesh[ seli ].isRigged)
         contextMenu->addAction(exportRiggedMeshAct);
       if (data->mesh[ seli ].frame.size()>1)
          contextMenu->addAction(exportMovingMeshAct);
     }

     if (t==SKELETON) {
       contextMenu->addAction(exportSkeletonAct);
       contextMenu->addAction(exportSkinAct);
       contextMenu->addSeparator();
       contextMenu->addAction(exportSkeletonModAct);
       contextMenu->addAction(importSkeletonModAct);
     }
     if (t==ANIMATION){
       contextMenu->addAction(exportAnimationAct);
       contextMenu->addAction(exportSkinForAnimationAct);
     }
     if (t==BODY) {
       contextMenu->addAction(exportBodyAct);
     }
   } else {
     if (t==MESH) {
       contextMenu->addSeparator();
       contextMenu->addAction(exportMeshGroupAct);
       contextMenu->addAction(exportMeshGroupManyFilesAct);
     }
   }

   // tool section
   bool sep = false;
   if (!nosel) {
   if (t==MESH) {
     const BrfMesh &mesh(data->mesh[ seli ]);
     if (mesh.isRigged) {
       if (!sep) contextMenu->addSeparator(); sep=true;
       contextMenu->addAction(reskeletonizeAct);
       contextMenu->addAction(meshFemininizeAct);
     }
     //contextMenu->addAction(transferRiggingAct);

     if (!sep) contextMenu->addSeparator(); sep=true;
     contextMenu->addAction(flipAct);
     contextMenu->addAction(transformAct);
     //contextMenu->addAction(scaleAct);
     contextMenu->addAction(meshRecomputeNormalsAndUnify);
     contextMenu->addAction(meshUnify);
     if (!nosel)  contextMenu->addAction(meshToBody);
     if (!onesel && !nosel)  contextMenu->addAction(meshMerge);
     contextMenu->addAction(meshMountOnBone);
     QMenu *m = contextMenu->addMenu(tr("Backfacing faces"));
     m->addAction(meshRemoveBackfacing);
     m->addAction(meshAddBackfacing);


     m = contextMenu->addMenu(tr("Discard"));
     m->addAction(discardAniAct);
     discardAniAct->setEnabled(mulsel || (mesh.frame.size()>1));
     m->addAction(discardRigAct);
     discardRigAct->setEnabled(mulsel || mesh.isRigged);
     m->addAction(discardColAct);
     discardColAct->setEnabled(mulsel || mesh.hasVertexColor);

     contextMenu->addSeparator();

     contextMenu->addAction(meshRecolorAct);
     contextMenu->addAction(meshComputeAoAct);
     contextMenu->addAction(meshTuneColorAct);

   }

   if (t==BODY) {
     if (!sep) contextMenu->addSeparator(); sep=true;
     contextMenu->addAction(flipAct);
     contextMenu->addAction(transformAct);
     if (!onesel && !nosel) { contextMenu->addAction(bodyMerge); }
     contextMenu->addAction(bodyMakeQuadDominantAct);
   }
   if (t==ANIMATION) {
     if (onesel) {
       if (!sep) contextMenu->addSeparator(); sep=true;
       contextMenu->addAction(breakAniAct);
       contextMenu->addAction(breakAniWithIniAct);
       contextMenu->addAction(shiftAniAct);
       contextMenu->addAction(aniExtractIntervalAct);
       contextMenu->addAction(aniRemoveIntervalAct);
     }
     if (mulsel) contextMenu->addAction(aniMergeAct);
   }
   }


   // add to reference
   if (onesel && t==MESH) {
     contextMenu->addSeparator();
     QMenu* refMenu=contextMenu->addMenu(tr("Add to reference skins"));
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
     contextMenu->addSeparator();
     contextMenu->addAction(addToRefAnimAct);
   }
   if (onesel && t==SKELETON) {
     contextMenu->addSeparator();
     contextMenu->addAction(addToRefSkelAct);
   }




   //contextMenu->addAction(moveUp);
   //contextMenu->addAction(moveDown);

   contextMenu->exec(event->globalPos());
   event->accept();


 }


void Selector::setIniData(const IniData *data, int fi){
  iniData = data;
  iniFileIndex = fi;
}

template<class BrfType>
void Selector::addBrfTab(const vector<BrfType>  &v){

  int ti = BrfType::tokenIndex();

  tableModel[ti]->clear();
  if (v.size()!=0) {

    for (unsigned int k=0; k<v.size(); k++) {
      tableModel[ti]->vec.push_back( QString( v[k].name ) );

    }
    if (iniData && (iniData->updated>=4) && (!iniDataWaitsSaving)) {
      for (unsigned int k=0; k<v.size(); k++) {
        ObjCoord oc(iniFileIndex,k,ti);
        int h=-1;
        if (iniData->usedIn(oc).directOrIndirect()!=0) h=1;
        else if (iniData->usedBy(oc).size()==0) h=-2;
        tableModel[ti]->vecUsed.push_back( h );
      }
    } else {
      tableModel[ti]->vecUsed.resize(v.size(),0);
    }
    //tab[ti]->setModel(tableModel[ti]);

    tableModel[ti]->updateChanges();

    //tab[ti]->viewport()->update();

    if (tab[ti]->selectionModel()->selectedIndexes().size()==0)
    tab[ti]->selectionModel()->select(
        tableModel[ti]->pleaseCreateIndex(0,0),
        QItemSelectionModel::Select
    );


  } else {
  }

  /*QModelIndexList list = tab[ti]->selectionModel()->selectedIndexes();
  for (int i=0; i<list.size(); i++){
    if (list[i].row()>=tableModel[ti]->size()) tab[ti]->selectionModel()->select(
        tableModel[ti]->pleaseCreateIndex(i,0),QItemSelectionModel::Deselect);
  }*/
 //tab[ti]->selectionModel()->setCurrentIndex(tableModel[ti]->pleaseCreateIndex(0,0),QItemSelectionModel::NoUpdate);

}

void Selector::hideEmpty(){

  int n=0;
  for (int i=0; i<N_TOKEN; i++){
    if (tableModel[i]->size()) n++;
  }

  int reorder[N_TOKEN] = {
    BODY,
    MESH,
    MATERIAL,
    TEXTURE,
    SHADER,
    ANIMATION,
    SKELETON,
  };

  int prev = 0;
  for (int ii=0; ii<N_TOKEN; ii++) {
    int i = reorder[ii];
    int v = tableModel[i]->size();
    if (v==0) {
      int j = indexOf( tab[i] );
      if (j>=0) removeTab(j);
      //tab[i]->setVisible(false);

      //int j = indexOf( tab[i] );
      //if (j>=0) this->removeTab(  j );
    } else {
        QString title(tr(tokenTabName[i]));
      if (n>3) title.truncate(3);
      else if (n>2) title.truncate(4);
      if (n>3)
        title = QString("%1%2").arg( title ).arg(v);
      else title = QString("%1(%2)").arg( title ).arg(v);
      int j = indexOf( tab[i] );
      if (j<0) insertTab(prev,tab[i], title);
      else setTabText(j,title);
      tab[i]->setVisible(true);
      prev++;

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


  //emit setSelection(empty , NONE );

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
