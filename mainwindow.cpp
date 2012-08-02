#include "brfdata.h"
#include "glwidgets.h"
#include "selector.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_guipanel.h"
#include "vcgmesh.h"
#include "askBoneDialog.h"
#include "askSkelDialog.h"
#include "askTexturenameDialog.h"
#include "ioSMD.h"
#include <QtGui>

bool MainWindow::scanBrfDataForMaterials(const BrfData& tmp){
  for (unsigned int i=0; i<tmp.material.size(); i++){
    mapMT[tmp.material[i].name] = tmp.material[i].diffuseA;
  }
  return true;
}

bool MainWindow::scanBrfForMaterials(const QString fname){
  char st[500];
  sprintf(st,fname.toAscii().data());
  FILE *f =fopen(fname.toAscii().data(),"rb");
  if (!f) return false;
  BrfData tmp;
  //tmp.LoadMat(f);
  tmp.Load(f);
  scanBrfDataForMaterials(tmp);
  return true;
}

void MainWindow::notifyDataChanged(){
  setModified(false);
}

bool MainWindow::maybeSave()
{
    if (isModified) {

      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(this, tr("OpenBrf"),
                     tr("%1 been modified.\n"
                        "Save changes?").arg((editingRef)?"Internal reference objects have":"The dataset has"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      if (ret == QMessageBox::Save) {
        if (editingRef)
          return saveReference();
        else
          return save();
      }
      else if (ret == QMessageBox::Cancel)
        return false;
    }
    setModified(false);
    return true;
}

void MainWindow::onChangeMeshMaterial(QString st){
  if (!glWidget) return;
  int n=0;
  st.truncate(254);
  char* sta=st.toAscii().data();
  for (unsigned int i=0; i<GLWidget::MAXSEL; i++) {
    if (i>=brfdata.mesh.size()) break;
    if (glWidget->selGroup[i]) {
      sprintf(brfdata.mesh[i].material,"%s",sta);
      n++;
    }
  }
  statusBar()->showMessage( tr("Set %1 mesh materials to \"%2\"").arg(n).arg(sta) );
  glWidget->update();
  setModified(true);
}


static void setFlags(unsigned int &f, const QString q){
  bool ok;
  f= q.trimmed().replace("0x","").toUInt(&ok,16);
}
static void setFlags(unsigned int &f, const QLineEdit *q){
  setFlags(f,q->text());
}
static void setUInt(unsigned int &f, const QLineEdit *q){
  f= q->text().trimmed().toUInt();
}
static void setInt(int &f, const QLineEdit *q){
  f= q->text().trimmed().toInt();
}
static void setFloat(float &f, const QLineEdit *q){
  f= q->text().trimmed().toFloat();
}
static void setSign(int &s, const QLineEdit *q){
  int i =q->text().trimmed().toInt();
  if (i>=0) s=+1; else s=-1;
}
static void setString(char* st, QString s){
  s.truncate(254);
  sprintf(st,"%s",s.trimmed().toAscii().data());
}
static void setString(char* st, const QLineEdit *q){
  setString(st, q->text());
}

template<class T>
static void _setFlag(vector<T> &v, int i, QString st){
  assert (i>0 && i<(int)v.size());
  setFlags(v[i].flags , st);
}

template< class T >
static bool _swap(vector<T> &t, int i, int j){
  if (i<0 || j<0 || i>=(int)t.size() || j>=(int)t.size()) return false;
  T tmp; tmp=t[i]; t[i]=t[j]; t[j]=tmp; return true;
}
template< class T >
static bool _dup(vector<T> &t, int i){
  if (i<0 || i>=(int)t.size()) return false;
  t.insert(t.begin()+i,t.begin()+i,t.begin()+i+1);
  sprintf(t[i+1].name,"%s_copy",t[i].name);
  return true;
}
template< class T >
static bool _dupNN(vector<T> &t, int i){
  if (i<0 || i>=(int)t.size()) return false;
  t.insert(t.begin()+i,t.begin()+i,t.begin()+i+1);
  return true;
}
template< class T >
static bool _del(vector<T> &t, int i){
  if (i<0 || i>=(int)t.size()) return false;

  t.erase(t.begin()+i,t.begin()+i+1);
  return true;
}
template< class T >
static char* _name(T &t, int i){
  if (i<0 || i>=(int)t.size()) return NULL;
  return t[i].name;
}
template< class T >
static bool _copy(vector<T> &t, int i, vector<T> &d){
  if (i<0 || i>=(int)t.size()) return false;
  d.push_back(t[i]);
  return true;
}


void MainWindow::updateTextureAccessDup(){
  int i = selector->firstSelected();
  int j = guiPanel->getCurrentSubpieceIndex(SHADER);
  if (i>=0 && j>=0) {
    _dupNN( brfdata.shader[i].opt, j );
    guiPanel->updateShaderTextaccSize();
    setModified(true);
  }
}
void MainWindow::updateTextureAccessDel(){
  int i = selector->firstSelected();
  int j = guiPanel->getCurrentSubpieceIndex(SHADER);
  if (i>=0 && j>=0) {
    _del( brfdata.shader[i].opt, j );
    guiPanel->updateShaderTextaccSize();
    setModified(true);
  }
}
void MainWindow::updateTextureAccessAdd(){
   int i = selector->firstSelected();
  if (i>=0) {
    BrfShaderOpt o;
    brfdata.shader[i].opt.push_back(o);
    guiPanel->updateShaderTextaccSize();
    setModified(true);
  }
}

void MainWindow::updateDataShader(){
  int sel=selector->firstSelected();
  if (sel>(int)brfdata.shader.size()) return;
  if (sel<0) return;
  BrfShader &s(brfdata.shader[sel]);
  Ui::GuiPanel* u = guiPanel->ui;
  setFlags(s.flags, u->leShaderFlags);
  setString(s.technique, u->leShaderTechnique);
  setUInt(s.requires, u->leShaderRequires);
  setString(s.fallback, u->leShaderFallback);

  int ta =guiPanel->getCurrentSubpieceIndex(SHADER);

  if (ta>=0 && ta<(int)s.opt.size()) {
    setUInt(s.opt[ta].colorOp, u->leShaderTaColorOp);
    setUInt(s.opt[ta].alphaOp, u->leShaderTaAlphaOp);
    setFlags(s.opt[ta].flags, u->leShaderTaFlags);
    setInt(s.opt[ta].map, u->leShaderTaMap);
  }
  setModified(true);
}

void MainWindow::updateDataBody(){
  int sel=selector->firstSelected();
  if (sel>(int)brfdata.body.size()) return;
  if (sel<0) return;
  BrfBody &b(brfdata.body[sel]);
  Ui::GuiPanel* ui = guiPanel->ui;

  int ta =guiPanel->getCurrentSubpieceIndex(SHADER);
  if (ta>=0 && ta<(int)b.part.size()) {
    BrfBodyPart &p(b.part[ta]);
    setFlags(p.flags, ui->leBodyFlags);
    setSign(p.ori, ui->leBodySign);
    switch (p.type)
    {
    case BrfBodyPart::CAPSULE:
      // fallthrough
      setFloat(p.dir.X(),ui->leBodyBX);
      setFloat(p.dir.Y(),ui->leBodyBY);
      setFloat(p.dir.Z(),ui->leBodyBZ);
    case BrfBodyPart::SPHERE:
      setFloat(p.center.X(),ui->leBodyAX);
      setFloat(p.center.Y(),ui->leBodyAY);
      setFloat(p.center.Z(),ui->leBodyAZ);
      setFloat(p.radius,ui->leBodyRad);
      break;
    default: break;
    }
  }
  setModified(true);
  glWidget->update();
}

void MainWindow::updateDataMaterial(){

  int sel=selector->firstSelected();
  if (sel>(int)brfdata.material.size()) return;
  if (sel<0) return;
  BrfMaterial &m(brfdata.material[sel]);
  Ui::GuiPanel* u = guiPanel->ui;

  setFlags(m.flags, u->leMatFlags);
  setString(m.bump, u->leMatBump);
  setString(m.diffuseA, u->leMatDifA);
  setString(m.diffuseB, u->leMatDifB);
  setString(m.enviro, u->leMatEnv);
  setFloat(m.r, u->leMatR);
  setFloat(m.g, u->leMatG);
  setFloat(m.b, u->leMatB);
  setFloat(m.specular, u->leMatCoeff);
  setString(m.spec, u->leMatSpec);
  setString(m.shader, u->leMatShader);

  mapMT[m.name] = m.diffuseA;
  setModified(true);
}

void MainWindow::onChangeTimeOfFrame(QString time){
  if (!glWidget) return;
  if (!selector) return;
  assert(selector->currentTabName()==MESH);
  int fl  =  time.toInt();
  int i=selector->firstSelected();
  int j=currentDisplayFrame();
  if (i>=0 && j>=0 && i<(int)brfdata.mesh.size() && j<(int)brfdata.mesh[i].frame.size()) {
    brfdata.mesh[i].frame[j].time=fl;
    statusBar()->showMessage( tr("Set time to \"%1\"").arg(fl) );
    setModified(true);
  }

}

void MainWindow::onChangeFlags(QString st){
  if (!glWidget) return;
  int n=1;
  //unsigned int fl  =  st.toUInt();
  st.truncate(254);
  int i=selector->firstSelected();

  switch(selector->currentTabName()) {
    case MESH:
      n=0;
      for (unsigned int j=0; j<GLWidget::MAXSEL; j++) if (glWidget->selGroup[j]) {
        _setFlag(brfdata.mesh, j, st);
        n++;
      }
    break;
    case MATERIAL: _setFlag(brfdata.material,i,st); break;
    case TEXTURE: _setFlag(brfdata.texture,i,st); break;
    case SKELETON: _setFlag(brfdata.skeleton,i,st); break;
    //case ANIMATION: _setFlag(brfdata.animation,i,fl); break;
    default: assert(0);
  }
  //statusBar()->showMessage( tr("Set flag(s) to \"%1\"").arg(fl) );
  setModified(true);
}

void MainWindow::tryLoadMaterials(){
  // tmp, hacked funciton

  QString s = settings->value("LastOpenPath").toString();
  QString path[2];
  path[0].clear();
  path[1].clear();

  QDir d = QFileInfo(s).dir();

  d.cd(QFileInfo(s).fileName());
  path[0]=d.path();
  path[1].clear();

  d.cdUp(); d.cd("textures");
  defpathTexture = d.path();
  glWidget->texturePath=defpathTexture;

  if (d.cdUp() && d.cdUp() && d.cd("CommonRes") ) path[1]=d.path();
  for (int i=0; i<2; i++) {
    if (!path[i].isEmpty()) {
      scanBrfForMaterials(path[i]+"/materials.brf");
      scanBrfForMaterials(path[i]+"/core_materials.brf");
    }
  }
  //scanBrfForMaterials("C:/games/Mount&Blade1011/CommonRes/body_meshes.brf");
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{


  settings = new QSettings("mtarini", "OpenBRF");

  glWidget = new GLWidget(this,&mapMT);
  selector = new Selector(this);
  selector->reference=&reference;

  isModified=false;

  createActions();
  createMenus();
  setEditingRef(false);

  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           glWidget,  SLOT(setSelection(QModelIndexList,int)) );

  connect( selector, SIGNAL(breakAni(int,bool)),
           this, SLOT(breakAni(int,bool)));

  QSplitter* main = new QSplitter();
  guiPanel = new GuiPanel( this, &mapMT);

  main->addWidget(selector);
  main->addWidget(guiPanel);
  main->addWidget(glWidget);


  connect(importStaticMeshAct,SIGNAL(triggered()),this,SLOT(importStaticMesh()));
  connect(importRiggedMeshAct,SIGNAL(triggered()),this,SLOT(importRiggedMesh()));
  connect(importMovingMeshAct,SIGNAL(triggered()),this,SLOT(importMovingMesh()));
  connect(importSkeletonAct,  SIGNAL(triggered()),this,SLOT(importSkeleton()));
  connect(importAnimationAct, SIGNAL(triggered()),this,SLOT(importAnimation()));
  connect(importBodyAct, SIGNAL(triggered()),this,SLOT(importCollisionBody()));
  connect(addNewMaterialAct, SIGNAL(triggered()),this,SLOT(addNewMaterial()));
  connect(addNewShaderAct, SIGNAL(triggered()),this,SLOT(addNewShader()));
  connect(addNewTextureAct, SIGNAL(triggered()),this,SLOT(addNewTexture()));

  connect(guiPanel->ui->cbLighting        ,SIGNAL(stateChanged(int)),glWidget,SLOT(setLighting(int)));
  connect(guiPanel->ui->cbTexture         ,SIGNAL(stateChanged(int)),glWidget,SLOT(setTexture(int)));
  connect(guiPanel->ui->cbFloor           ,SIGNAL(stateChanged(int)),glWidget,SLOT(setFloor(int)));
  connect(guiPanel->ui->cbWireframe       ,SIGNAL(stateChanged(int)),glWidget,SLOT(setWireframe(int)));
  connect(guiPanel->ui->rbNocolor         ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerWhite()));
  connect(guiPanel->ui->rbRiggingcolor    ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerRig()));
  connect(guiPanel->ui->rbVertexcolor     ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerVert()));
  connect(guiPanel->ui->buPlay            ,SIGNAL(clicked()),glWidget,SLOT(setPlay()));
  connect(guiPanel->ui->buPause           ,SIGNAL(clicked()),glWidget,SLOT(setPause()));
  connect(guiPanel->ui->buStop            ,SIGNAL(clicked()),glWidget,SLOT(setStop()));
  connect(guiPanel->ui->buStepF           ,SIGNAL(clicked()),glWidget,SLOT(setStepon()));
  connect(guiPanel->ui->buStepB           ,SIGNAL(clicked()),glWidget,SLOT(setStepback()));
  connect(guiPanel->ui->rbAlphaColor      ,SIGNAL(clicked()),glWidget,SLOT(showAlphaPurple()));
  connect(guiPanel->ui->rbAlphaNo         ,SIGNAL(clicked()),glWidget,SLOT(showAlphaNo()));
  connect(guiPanel->ui->rbAlphaTransparent,SIGNAL(clicked()),glWidget,SLOT(showAlphaTransparent()));
  connect(guiPanel->ui->boxMaterial    ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeMeshMaterial(QString)));
  connect(guiPanel->ui->boxMaterial    ,SIGNAL(textEdited(QString)),
          guiPanel,SLOT(updateMaterial(QString)));

  // edit material
  connect(guiPanel->ui->leMatR,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatG,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatB,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatCoeff,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatBump,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatDifA,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatDifB,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatEnv,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatShader,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatSpec,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->textureAccessAdd,SIGNAL(triggered()), this, SLOT(updateTextureAccessAdd()));
  connect(guiPanel->textureAccessDup,SIGNAL(triggered()), this, SLOT(updateTextureAccessDup()));
  connect(guiPanel->textureAccessDel,SIGNAL(triggered()), this, SLOT(updateTextureAccessDel()));

  // make material texture selection select rendererd texture too
  connect(guiPanel->ui->leMatBump,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialBump()));
  connect(guiPanel->ui->leMatDifA,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialDiffuseA()));
  connect(guiPanel->ui->leMatDifB,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialDiffuseB()));
  connect(guiPanel->ui->leMatEnv ,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialEnviro()));
  connect(guiPanel->ui->leMatSpec,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialSpecular()));

  // edit body
  connect(guiPanel->ui->leBodyAX,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyAY,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyAZ,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyBX,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyBY,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyBZ,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyRad,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodySign,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));


  // edit shader
  connect(guiPanel->ui->leShaderFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderFallback,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderRequires,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTechnique,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaAlphaOp,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaColorOp,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaMap,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));

  connect(guiPanel->ui->boxFlags       ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeFlags(QString)));
  connect(guiPanel->ui->timeOfFrame    ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeTimeOfFrame(QString)));
  //connect(guiPanel->ui->boxAnimationFlags,SIGNAL(textEdited(QString)),
  //        this,SLOT(onChangeFlags(QString)));
  connect(guiPanel->ui->boxTextureFlags,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeFlags(QString)));
  connect(guiPanel->ui->cbRefani,SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setRefAnimation(int)) );
  connect(guiPanel->ui->cbRefani,SIGNAL(currentIndexChanged(int)), guiPanel, SLOT(setRefAnimation(int)) );
  connect(guiPanel->ui->cbRefSkel,SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setRefSkeleton(int)) );
  connect(guiPanel->ui->cbSkin,SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setRefSkin(int)) );
  connect(guiPanel->ui->frameNumber,SIGNAL(valueChanged(int)),guiPanel,SLOT(updateFrameNumber(int)));

  connect(guiPanel->ui->frameNumber,SIGNAL(valueChanged(int)),glWidget,SLOT(setFrameNumber(int)));
  connect(glWidget,SIGNAL(signalFrameNumber(int)),guiPanel,SLOT(updateFrameNumber(int)));

  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           guiPanel,  SLOT(setSelection(QModelIndexList,int)) );

  //connect( selector,SIGNAL(addToRefMesh(int)) ,
  //         this    ,  SLOT(addToRefMesh(int)) );

  //main->setLayout(mainLayout);

  reference.Load("reference.brf");
  setCentralWidget(main);

  updateTitle();

  glWidget->selected=1;
  glWidget->data = guiPanel->data = &brfdata;
  glWidget->reference = &reference;

  guiPanel->setReference(&reference);

  tryLoadMaterials();

  this->setAcceptDrops(true);

  loadOptions();


}



bool MainWindow::setEditingRef(bool mode){
  editingRef = mode;
  if (editingRef) {
    editRefAct->setText("Stop editing reference data");
    editRefAct->setStatusTip(tr("Stop editing \"reference\" skeletons, animations & meshes, that OpenBrf uses to display data."));
  } else {
    editRefAct->setText("Edit reference data");
    editRefAct->setStatusTip(tr("Edit \"reference\" skeletons, animations & meshes, that OpenBrf uses to display data."));
  }
  glWidget->setEditingRef(mode);
  return true;
}

bool MainWindow::importBrf(){
  QString fn = askImportFilename("BRF file (*.brf)");
  if (fn.isEmpty()) return false;
  BrfData tmp;
  if (!tmp.Load(fn.toAscii().data())) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import file %1\n\n")
    );
    return false;
  }
  brfdata.Merge(tmp);
  selector->updateData(brfdata);
  setModified(true);
  return true;
}

void MainWindow::insert(const BrfMesh &o){ insert( brfdata.mesh, o); }
void MainWindow::insert(const BrfSkeleton &o){ insert( brfdata.skeleton, o); }
void MainWindow::insert(const BrfAnimation &o){ insert( brfdata.animation, o); }
void MainWindow::insert(const BrfTexture &o){   insert( brfdata.texture, o); }
void MainWindow::insert(const BrfMaterial &o){  insert( brfdata.material, o); }
void MainWindow::insert(const BrfShader &o){   insert( brfdata.shader, o); }
void MainWindow::insert(const BrfBody &o){   insert( brfdata.body, o); }

template<class BrfType> void MainWindow::insert( vector<BrfType> &v, const BrfType &o){
  if (selector->currentTabName()!=BrfType::tokenIndex() ) {
    v.push_back(o);
  } else {
    int i = selector->firstSelected()+1;
    if (i<0 || i>=(int)v.size()) i=v.size()-1;
    v.insert( v.begin()+i, o);
  }
}

void _setName(char* st, QString s){
  s.truncate(254);
  sprintf(st, "%s", s.toAscii().data());
}



template<class T>
bool MainWindow::addNewGeneral(){


  int tok = T::tokenIndex();
  QString newName;
  AskTexturenameDialog d(this, tok==MATERIAL);
  if (tok==MATERIAL || tok==TEXTURE)
    d.setBrowsable(defpathTexture);
  d.setLabel( "Name:" );
  d.setWindowTitle( QString("New %1").arg(tokenFullName[tok]) );
  d.setDef(QString("new_%1").arg(tokenFullName[tok]).toLower());
  int ok=d.exec();
  if (ok) {
    newName=d.getRes();
    T m;
    _setName(m.name,newName);
    m.SetDefault();
    insert(m);
    if (d.alsoAdd()) {
      BrfTexture t;
      _setName(t.name,newName);
      t.SetDefault();
      insert(t);
    }
    selector->updateData(brfdata);
    setModified(true);
    return true;
  }

  return false;
}
bool MainWindow::addNewMaterial(){
  if (addNewGeneral<BrfMaterial>()){
    int i=selector->currentIndex();
    if (i>=0 && i<(int)brfdata.material.size()) {
      BrfMaterial &m(brfdata.material[i]);
      mapMT[m.name] = m.diffuseA;
    }
    glWidget->showMaterialDiffuseA();
    return true;
  }
  return false;
}
bool MainWindow::addNewTexture(){
  return addNewGeneral<BrfTexture>();
}
bool MainWindow::addNewShader(){
  return addNewGeneral<BrfShader>();
}

bool MainWindow::importCollisionBody(){
  QString fn = askImportFilename(
    tr("mesh file (*.obj)")
  );
  if (fn.isEmpty()) return false;

  BrfBody b;
  if (!b.ImportOBJ(fn.toAscii().data())) {
    QMessageBox::information(this, tr("Open Brf"),
      tr("Cannot import file %1\n").arg(fn)
    );
    return false;
  }

  QString name=QFileInfo(fn).baseName();
  name.truncate(254);

  sprintf( b.name, "%s", name.toAscii().data());

  insert( b );

  selector->updateData(brfdata);
  setModified(true);
  return true;
}


bool MainWindow::importStaticMesh(){
  QString fn = askImportFilename(
    tr("mesh file ("
      "*.ply "
      "*.off "
      "*.obj "
      "*.stl "
      //"*.smf "
      "*.dae)"
      //"*.asc "
      //"*.vmi "
      //"*.raw "
      //"*.ptx)"
    )
  );
  if (fn.isEmpty()) return false;
  if (!VcgMesh::load(fn.toAscii().data())) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import file %1\n\n"
         "(error: %2)").arg(fn).arg(VcgMesh::lastErrString())
    );
    return false;
  }
  BrfMesh m = VcgMesh::toBrfMesh();
  QString meshname=QFileInfo(fn).baseName();
  meshname.truncate(254);

  sprintf( m.name, "%s", meshname.toAscii().data());
  if (selector->currentTabName()==MESH && selector->numSelected()==1) {
    if (!VcgMesh::gotMaterialName())
      sprintf(m.material,brfdata.mesh[ selector->firstSelected() ].material);
    brfdata.mesh[ selector->firstSelected() ] = m;
  }
  else insert( m );

  selector->updateData(brfdata);
  setModified(true);

  switch (this->afterMeshImport()) {
    case 1:
      m.UnifyPos();
      m.UnifyVert(true);
      break;
    case 2:
      m.UnifyPos();
      m.UnifyVert(false);
      m.ComputeNormals();
      break;
  }
  statusBar()->showMessage(tr("Imported mesh \"%1\"--- normals:%2 colors:%3 texture_coord:%4")
    .arg( m.name )
    .arg((VcgMesh::gotNormals()?"[ok]":"[recomputed]"))
    .arg((VcgMesh::gotColor()?"[ok]":"[NO]"))
    .arg((VcgMesh::gotTexture()?"[ok]":"[NO]"))
  );
  return true;
}

bool MainWindow::importAnimation(){
  QString brfFormat("Old BrfEdit style SMD (*.SMD)");
  QString fn = askImportFilename(QString("Studiomdl Data (*.SMD);;")+brfFormat);
  if (fn.isEmpty()) return false;
  BrfSkeleton s;
  BrfAnimation a;

  bool backComp=(lastImpExpFormat==brfFormat);
  int res = ioSMD::Import(fn.toAscii().data(), a, s);
  if (res!=0) {
     QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import animation:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  }


  QString name=QFileInfo(fn).baseName();
  name.truncate(254);
  sprintf( a.name, "%s", name.toAscii().data());


  if (backComp) {
    //int j = gimmeASkeleton( s.bone.size() );
    //if (j>=0)
    //  a.Reskeletonize( s, reference.skeleton[j] );
    if (a.frame.size()) a.frame.pop_back(); // remove last frame
  }
  insert(a);

  setModified(true);
  selector->updateData(brfdata);

  return true;

}

int MainWindow::gimmeASkeleton(int bs){
  return reference.getOneSkeleton(bs, glWidget->getRefSkeleton() );
}

bool MainWindow::importSkeleton(){

  QString fn = askImportFilename("Studiomdl Data (*.SMD)");
  if (fn.isEmpty()) return false;
  BrfSkeleton s;
  BrfMesh m;
  int res = ioSMD::Import(fn.toAscii().data(), m, s);
  if (res!=0) {
     QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import skeleton:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  }

  QString name=QFileInfo(fn).baseName();
  name.truncate(254);
  sprintf( s.name, "%s", name.toAscii().data());

  insert(s);
  setModified(true);
  selector->updateData(brfdata);

  statusBar()->showMessage(tr("Imported skeleton \"%1\"--- nbones:%2")
    .arg( s.name )
    .arg( s.bone.size() )
  );
  return true;
}

bool MainWindow::importRiggedMesh(){
  QString fn = askImportFilename("Studiomdl Data (*.SMD)");
  if (fn.isEmpty()) return false;
  BrfSkeleton s;
  BrfMesh m;
  int res = ioSMD::Import(fn.toAscii().data(), m, s);

  if (res!=0) {
     QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import rigged mesh:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  }

  QString name=QFileInfo(fn).baseName();
  name.truncate(254);
  sprintf( m.name, "%s", name.toAscii().data());


  statusBar()->showMessage(tr("Imported rigged mesh \"%1\"")
    .arg( m.name )
  );

  insert(m);
  selector->updateData(brfdata);
  setModified(true);
  return true;
}

int MainWindow::askRefSkin(){
  QStringList items;
  int n = reference.GetFirstUnusedLetter();
  if (n==0) {
    QMessageBox::information(this,
        tr("Open Brf"),
        tr("Oops... no skin is currently available...\n")
      );
    return -1;
  }
  for (int i=0; i<n; i++)
    items.append(tr("Skin %1").arg( char('A'+i) ));

  bool ok;
  QString resSt = QInputDialog::getItem(
      this, tr("Select a skin"),
      tr("Select a skin:"), items, 0, false, &ok
  );

  if (ok && !resSt.isEmpty())
     return resSt.toAscii().data()[5]-'A';
  else return -1;
}

int MainWindow::currentDisplaySkin(){
  return glWidget->getRefSkin();
}
int MainWindow::currentDisplaySkeleton(){
  return glWidget->getRefSkeleton();
}
int MainWindow::currentDisplayFrame(){
  return glWidget->getFrameNumber();
}

void MainWindow::meshRecomputeNormalsAndUnify(){
  int i = selector->firstSelected();
  if (i<0) return;
  if (i>(int)brfdata.mesh.size()) return;
  BrfMesh &m (brfdata.mesh[i]);
 // m.UnifyPos();
  //m.UnifyVert(false);
  m.ComputeNormals();
  glWidget->update();
  statusBar()->showMessage(tr("Normals recomputed."), 2000);
  setModified(true);
}

void MainWindow::reskeletonize(){
  int i = selector->firstSelected();
  if (i<0) return;
  if (i>(int)brfdata.mesh.size()) return;

  BrfMesh m = brfdata.mesh[i];
  m.KeepOnlyFrame( currentDisplayFrame() );
  bool asFrame=false;
  QPair<int,int> res = askRefSkel( m.maxBone, asFrame);
  int a=res.first; int b=res.second;
  if (a==-1) return;
  if (a==b) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Same skeleton:\nreskeletonization canceled.\n")
    );
    return;
  }
  if (reference.skeleton[a].bone.size()!=reference.skeleton[b].bone.size()) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Different number of bones:\nreskeletonization canceled.\n")
    );
    return;
  }

  m.Reskeletonize( reference.skeleton[a], reference.skeleton[b]);
  if (!asFrame) {
    sprintf(m.name,"%s_%s",m.name,reference.skeleton[b].name);
    insert(m);
  } else {
    brfdata.mesh[i].AddFrame(m);
  }
  selector->updateData(brfdata);
  setModified(true);

}

bool MainWindow::exportRiggedMesh(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.mesh.size()) return false;
  int si = currentDisplaySkeleton();
  if (si<0) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export animation without a proper skeleton!\n")
    );
    return false;
  }
  const BrfMesh &m(brfdata.mesh[i]);
  const BrfSkeleton& s(reference.skeleton[si]);

  QString fn = askExportFilename(brfdata.mesh[ i ].name,"Studiomdl Data rigged mesh (*.SMD)");
  if (fn.isEmpty()) return false;

  int res = ioSMD::Export(fn.toAscii().data(), m, s, currentDisplayFrame() );

  if (res) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export rest-pose:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  } else {
    return true;
  }
}

bool MainWindow::exportSkeletonAndSkin(){

  BrfSkeleton s;
  int skinNumber=-1;

  if (selector->currentTabName()==SKELETON) {
    int i = selector->firstSelected();
    assert(i>=0 && i<(int)brfdata.skeleton.size());
    s = brfdata.skeleton[i];
  }
  else if (selector->currentTabName()==ANIMATION) {
    int i = currentDisplaySkeleton();
    if (i<0) {
      QMessageBox::information(this,
        tr("Open Brf"),
        tr("Cannot export animation without a proper skeleton!\n")
      );
      return false;
    }    
    assert(i<(int)reference.skeleton.size());
    s = reference.skeleton[i];
    skinNumber = currentDisplaySkin();
  }
  else assert(0);


  if (skinNumber<0) skinNumber = askRefSkin();
  if (skinNumber<0) return false;

  BrfMesh m = reference.GetCompleteSkin(skinNumber);

  QString fn = askExportFilename(
    QString(s.name)+"_skin_"+char(skinNumber+'A'),
    "Studiomdl Data skinned rest-pose (*.SMD)"
  );
  if (fn.isEmpty()) return false;

  int res = ioSMD::Export(fn.toAscii().data(), m, s, 0);
  if (res) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export rest-pose:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  } else {
    return true;
  }
}

bool MainWindow::exportAnimation(){
  int i = selector->firstSelected();
  assert(selector->currentTabName()==ANIMATION);
  assert(i>=0 && i<(int)brfdata.animation.size());
  BrfAnimation &a(brfdata.animation[ i ]);
  int si = currentDisplaySkeleton();
  if (si<0) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export animation without a proper skeleton!\n")
    );
    return false;
  }
  BrfSkeleton const &s( reference.skeleton[ si ] );

  QString fn = askExportFilename(brfdata.animation[ i ].name,"Studiomdl Data Animation (*.SMD)");
  if (fn.isEmpty()) return false;

  int res = ioSMD::Export(fn.toAscii().data(), a, s);
  if (res) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export animation:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  } else {
    return true;
  }
}




bool MainWindow::exportSkeleton(){
  int i = selector->firstSelected();
  assert(selector->currentTabName()==SKELETON);
  assert(i>=0 && i<(int)brfdata.skeleton.size());

  BrfSkeleton const &s( brfdata.skeleton[ i ] );
  BrfMesh m;

  int mi = currentDisplaySkin();
  if (mi<0) s.BuildDefaultMesh(m);
  else {
    m = reference.GetCompleteSkin( mi );
  }

  QString fn = askExportFilename(brfdata.skeleton[ i ].name,"Studiomdl Data Skeleton (*.SMD)");
  if (fn.isEmpty()) return false;

  BrfData tmp; tmp.mesh.push_back(m); tmp.Save((fn+"_tmp.brf").toAscii().data());

  int res = ioSMD::Export(fn.toAscii().data(), m, s, 0);
  if (res) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export skeleton:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  } else {
    return true;
  }

}

bool MainWindow::exportSkeletonMod(){
  int i = selector->firstSelected();
  assert(selector->currentTabName()==SKELETON);
  assert(i>=0 && i<(int)brfdata.skeleton.size());
  QString fn = askExportFilename(brfdata.skeleton[ i ].name);
  if (fn.isEmpty()) return false;

  VcgMesh::add(brfdata.skeleton[ i ]);
  if (!VcgMesh::save(fn.toAscii().data())){
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export control mesh in file \n\"%1\"\n\n").arg(fn)
    );
    return false;
  }
  return true;
}

bool MainWindow::importSkeletonMod(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.skeleton.size()) return false;

  QString fn = askImportFilename(tr("mesh file ("
      "*.ply "
      "*.off "
      "*.obj "
      "*.stl "
      //"*.smf "
      "*.dae)"
      //"*.asc "
      //"*.vmi "
      //"*.raw "
      //"*.ptx)"
    ));
  if (fn.isEmpty()) return false;
  //VcgMesh::clear();
  BrfSkeleton s = brfdata.skeleton[i];
  if (!VcgMesh::load(fn.toAscii().data())) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot read mesh!")
    );
    return false;
  }
  if (!VcgMesh::modifyBrfSkeleton(s)) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Modification of skeleton with mesh: fail!")
    );
    return false;
  }

  QString name=QFileInfo(fn).baseName();
  name.truncate(254);
  sprintf( s.name, "%s", name.toAscii().data());
  insert(s);
  setModified(true);
  selector->updateData(brfdata);
  return true;
}

bool MainWindow::exportMovingMesh(){
  QMessageBox::information(this,
    tr("Open Brf"),
    tr("Export vertex animation:: to do")
  );
  return false;
}


bool MainWindow::importMovingMesh(){
  QMessageBox::information(this,
    tr("Open Brf"),
    tr("Import vertex animation:: to do")
  );
  return false;
}

bool MainWindow::exportCollisionBody(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.body.size()) return false;
  if (selector->currentTabName()!=BODY) return false;
  QString fn = askExportFilename(brfdata.body[ i ].name,"Wavefront Object Files (*.obj)");
  if (fn.isEmpty()) return false;
  if (!brfdata.body[i].ExportOBJ(fn.toAscii().data())){
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot write file?")
    );
    return false;
  }
  return true;
}

bool MainWindow::exportStaticMesh(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.mesh.size()) return false;
  
  QString fn = askExportFilename(brfdata.mesh[ i ].name);
  if (fn.isEmpty()) return false;
  switch (selector->currentTabName()) {
    case MESH:
      // save mesh as Ply
      if (fn.isEmpty()) return false;
      VcgMesh::add(brfdata.mesh[ i ], this->currentDisplayFrame() );
      return VcgMesh::save(fn.toAscii().data());
    break;
    default: assert(0); // how was this signal sent?!
  }
  return false;
}


void MainWindow::moveUpSel(){
  int i = selector->firstSelected();
  int j = i-1;
  switch (selector->currentTabName()) {
    case MESH: _swap(brfdata.mesh, i, j); break;
    case TEXTURE: _swap(brfdata.texture, i, j); break;
    case SHADER: _swap(brfdata.shader, i,j); break;
    case MATERIAL:  _swap(brfdata.material, i,j); break;
    case SKELETON: _swap(brfdata.skeleton, i,j); break;
    case ANIMATION: _swap(brfdata.animation, i,j); break;
    case BODY: _swap(brfdata.body, i,j); break;
    default: assert(0);
  }
  selector->updateData(brfdata);
  selector->moveSel(-1);
  setModified(true);
}
void MainWindow::moveDownSel(){
  int i = selector->firstSelected();
  int j = i+1;
  switch (selector->currentTabName()) {
    case MESH: _swap(brfdata.mesh, i, j); break;
    case TEXTURE: _swap(brfdata.texture, i, j); break;
    case SHADER: _swap(brfdata.shader, i,j); break;
    case MATERIAL:  _swap(brfdata.material, i,j); break;
    case SKELETON: _swap(brfdata.skeleton, i,j); break;
    case ANIMATION: _swap(brfdata.animation, i,j); break;
    case BODY: _swap(brfdata.body, i,j); break;
    default: assert(0);
  }
  selector->updateData(brfdata);
  selector->moveSel(+1);
  setModified(true);
}

void MainWindow::renameSel(){
  int i = selector->firstSelected();
  char* name;
  TokenEnum t=(TokenEnum)selector->currentTabName();
  switch (t) {
    case MESH: name=_name(brfdata.mesh,i); break;
    case TEXTURE: name=_name(brfdata.texture,i); break;
    case SHADER: name=_name(brfdata.shader,i); break;
    case MATERIAL:  name=_name(brfdata.material, i ); break;
    case SKELETON: name=_name(brfdata.skeleton, i); break;
    case ANIMATION: name=_name(brfdata.animation,i); break;
    case BODY: name=_name(brfdata.body, i); break;
    default: assert(0);
  }
  if (name) {
    bool ok;
    QString newName = QInputDialog::getText(
      this, tr("Rename %1").arg(tokenBrfName[t]),
      tr("New name:"), QLineEdit::Normal,
      QString(name), &ok
    );
    if (ok) {
      _setName(name,newName);
      selector->updateData(brfdata);
      setModified(true);
    }
  }
}
void MainWindow::deleteSel(){
  int i = selector->firstSelected();
  switch (selector->currentTabName()) {
    case MESH: _del(brfdata.mesh, i); break;
    case TEXTURE: _del(brfdata.texture, i); break;
    case SHADER: _del(brfdata.shader, i); break;
    case MATERIAL:  _del(brfdata.material, i); break;
    case SKELETON: _del(brfdata.skeleton, i); break;
    case ANIMATION: _del(brfdata.animation, i); break;
    case BODY: _del(brfdata.body, i); break;
    default: assert(0);
  }
  selector->updateData(brfdata);
  setModified(true);
}



void MainWindow::editCutFrame(){
  if (selector->currentTabName()!=MESH) return;
  int i = selector->firstSelected();
  if (i<0 || i>=(int)brfdata.mesh.size()) return;
  BrfMesh &m(brfdata.mesh[i]);
  if (m.frame.size()<=1) editCut(); // last frame... cut entire mesh!
}

void MainWindow::editCopyFrame(){
  if (selector->currentTabName()!=MESH) return;
  int i = selector->firstSelected();
  if (i<0 || i>=(int)brfdata.mesh.size()) return;
  BrfMesh &m(brfdata.mesh[i]);
  int j = guiPanel->getCurrentSubpieceIndex(MESH);
  if (j<0 || j>=(int)m.frame.size()) return;

  clipboard.Clear();
  clipboard.mesh.push_back(m);
  clipboard.mesh[0].KeepOnlyFrame(j);

  editPasteFrameAct->setEnabled(true);
}

void MainWindow::editPasteFrame(){
  if (clipboard.mesh.size()==1 && clipboard.mesh[0].frame.size()==1) {
    int i = selector->firstSelected();
    if (selector->currentTabName()!=MESH || i < 0 || i>=(int)brfdata.mesh.size()) {
      editPaste(); // no mesh selected: paste new mesh...
      return;
    }
    int j=guiPanel->getCurrentSubpieceIndex(MESH);
    if (j<0) j=0;
    BrfMesh &m(brfdata.mesh[i]);
    //if (m.InsertFrame(clipboard.mesh[0].frame[0],j)) {
    //}
  }

}

void MainWindow::editCut(){
  editCopy(false);
  deleteSel();
}

void MainWindow::editCopy(bool deselect){
  clipboard.Clear();
  int i = selector->firstSelected();
  if (i<0) return;
  editPasteAct->setEnabled(true);
  switch (selector->currentTabName()) {
    case MESH: _copy(brfdata.mesh, i, clipboard.mesh);  break;
    case TEXTURE: _copy(brfdata.texture, i, clipboard.texture); break;
    case SHADER: _copy(brfdata.shader, i, clipboard.shader); break;
    case MATERIAL:  _copy(brfdata.material, i, clipboard.material); break;
    case SKELETON: _copy(brfdata.skeleton, i, clipboard.skeleton); break;
    case ANIMATION: _copy(brfdata.animation, i, clipboard.animation); break;
    case BODY: _copy(brfdata.body, i, clipboard.body); break;
    default: assert(0);
  }

  // maybe it was a single frame mesh?
  editPasteFrameAct->setEnabled(
    (selector->currentTabName()==MESH) &&
    (i<(int)clipboard.mesh.size()) &&
    (clipboard.mesh[0].frame.size()==1)
  );


}

void MainWindow::editPaste(){
  //deleteSel();
  for (int i=0; i<(int)clipboard.body.size(); i++) insert(clipboard.body[i]);
  for (int i=0; i<(int)clipboard.mesh.size(); i++) insert(clipboard.mesh[i]);
  for (int i=0; i<(int)clipboard.texture.size(); i++) insert(clipboard.texture[i]);
  for (int i=0; i<(int)clipboard.material.size(); i++) insert(clipboard.material[i]);
  for (int i=0; i<(int)clipboard.skeleton.size(); i++) insert(clipboard.skeleton[i]);
  for (int i=0; i<(int)clipboard.animation.size(); i++) insert(clipboard.animation[i]);
  for (int i=0; i<(int)clipboard.shader.size(); i++) insert(clipboard.shader[i]);
  //brfdata.Merge(clipboard);
  selector->updateData(brfdata);
  setModified(true);
}

void MainWindow::duplicateSel(){
  int i = selector->firstSelected();
  switch (selector->currentTabName()) {
    case MESH: _dup(brfdata.mesh, i); break;
    case TEXTURE: _dup(brfdata.texture, i); break;
    case SHADER: _dup(brfdata.shader, i); break;
    case MATERIAL:  _dup(brfdata.material, i); break;
    case SKELETON: _dup(brfdata.skeleton, i); break;
    case ANIMATION: _dup(brfdata.animation, i); break;
    case BODY: _dup(brfdata.body, i); break;
    default: assert(0);
  }
  selector->updateData(brfdata);
  selector->moveSel(+1);
  setModified(true);
}

void MainWindow::addToRef(){
  int i = selector->firstSelected();
  assert(i>=0);
  switch (selector->currentTabName()){
    case ANIMATION:
      reference.animation.push_back(brfdata.animation[i]);
      saveReference();
      break;
    case SKELETON:
      reference.skeleton.push_back(brfdata.skeleton[i]);
      saveReference();
      break;
    default: assert(0);
  }

}

typedef QPair<int, int> Pair;

Pair MainWindow:: askRefSkel(int nbones, bool &asAFrame){
  if (reference.skeleton.size()<2) return Pair(-1,-1);

  AskSkelDialog d(this,reference.skeleton);
  int res=d.exec();
  if (res==QDialog::Accepted) {
    asAFrame = d.asFrame();
    return Pair(d.getSkelFrom (),d.getSkelTo());
  }
  else
    return Pair(-1,-1);
}

Pair MainWindow::askRefBoneInt(){
  if (!reference.skeleton.size()) return Pair(-1,-1);
  AskBoneDialog d(this,reference.skeleton);
  int res=d.exec();
  if (res==QDialog::Accepted) return Pair(d.getSkel(),d.getBone());
  else
  return Pair(-1,-1);
}



void MainWindow::addToRefMesh(int k){
  int i=selector->firstSelected();
  assert (selector->currentTabName()==MESH);
  assert(i<(int)brfdata.mesh.size());
  BrfMesh m = brfdata.mesh[i];
  if (!m.isRigged) {
    m.KeepOnlyFrame(m.frame.size()-1); // TODO: if vertex ani, only one frame
    Pair p = askRefBoneInt();

    if (p.first==-1) {
      statusBar()->showMessage(tr("Canceled."), 2000);
      return;
    }

    m.Apply(
      BrfSkeleton::adjustCoordSystHalf(
        reference.skeleton[p.first].GetBoneMatrices()[p.second].transpose()
      ).transpose()
    );
    m.SetUniformRig(p.second);

  }
  char ch =char('A'+k);
  sprintf(m.name, "Skin%c_%s", ch , brfdata.mesh[i].name);
  reference.mesh.push_back(m);
  saveReference();
  statusBar()->showMessage(tr("Added mesh %1 to set %2.").arg(m.name).arg(ch), 4000);
}

template <class T> const char* add(vector<T>& v, const vector<T>& t, int i){
  v.push_back(t[i]);
  return t[i].name;
}

bool MainWindow::exportBrf(){
  BrfData tmp;
  int i = selector->firstSelected();
  const char * objName="";
  switch (selector->currentTabName()) {
    case MESH:
      objName = add(tmp.mesh, brfdata.mesh, i);
      // todo: multiple mesh?
      break;
    case TEXTURE:
      objName = add(tmp.texture, brfdata.texture, i) ;
      break;
    case SHADER:
      objName = add(tmp.shader, brfdata.shader,i) ;
      break;
    case MATERIAL:
      objName = add(tmp.material, brfdata.material,i) ;
      break;
    case SKELETON:
      objName = add(tmp.skeleton, brfdata.skeleton,i );
      break;
    case ANIMATION:
      objName = add(tmp.animation, brfdata.animation,i );
      break;
    case BODY:
      objName = add(tmp.body, brfdata.body,i) ;
      break;
    default: assert(0);
  }

  QString fileName = askExportFilename(objName,"brf");
  if (fileName.isEmpty()) return false;
  if (!tmp.Save(fileName.toAscii().data())){
    QMessageBox::information(this, tr("Export BRF"),
                              tr("Cannot save into %1.").arg(fileName));
    return false;
  }
  return true;
}

QString MainWindow::askImportFilename(QString ext){
  QString path = settings->value("LastExpImpPath").toString();
  if (path.isEmpty()) path = QDir::currentPath();

  QString fileName = QFileDialog::getOpenFileName(
    this,
    tr("Import file") ,
    path,
    ext,
    &lastImpExpFormat
   );
   if (fileName.isEmpty()) {
     statusBar()->showMessage(tr("Import canceled."), 2000);
   } else
   settings->setValue("LastExpImpPath",QFileInfo(fileName).absolutePath());
   return fileName;
}

QString MainWindow::askExportFilename(QString filename){
  return askExportFilename(filename, QString(
  "Polygon File Format (*.ply);;" // ok
  "Collada (*.dae);;"
  "AutoCad (*.dxf);;"  // n:C n:N
  "Object File Format (*.off);;" // e:N e:C
  "Wavefront Object Files (*.obj);;" // e:C n:N
  "Stereolithography 3D system(*.stl);;" // n:C
  //"3D studio Max (*.3ds);;"
  //"VRML (*.vrml);;"
  //"Intermediate Data Text File (*.idtf);;"
  //"Open Inventor (*.iv);;"
  //"(*.smf);;"
  //"Universal 3D (*.u3d);;"
  //"VMI (*.vmi);;"
  )
  );
}

QString MainWindow::askExportFilename(QString filename,QString ext){
  QString path = settings->value("LastExpImpPath").toString();
  if (path.isEmpty()) path = QDir::currentPath();

  QString fileName = QFileDialog::getSaveFileName(
    this,
    tr("Export file") ,
    tr("%1\\%2").arg(path).arg(filename),
    ext
   );
   if (fileName.isEmpty()) {
     statusBar()->showMessage(tr("Export canceled."), 2000);
   } else
   settings->setValue("LastExpImpPath",QFileInfo(fileName).absolutePath());
   return fileName;
}

void MainWindow::breakAni(int which, bool useIni){
  if ((int)brfdata.animation.size()>which) {

    BrfAnimation ani = brfdata.animation[which];

    if (!useIni) {
      int res = ani.Break(brfdata.animation);
      if (res>0) {
        selector->updateData(brfdata);
        setModified(true);
        //selector->setCurrentIndex(100);

        statusBar()->showMessage(tr("Animation %2 split in %1 chunks!").arg(res).arg(ani.name), 2000);
      } else {
        statusBar()->showMessage(tr("Animation could be auto-split (frames are too conescutive)"), 10000);
      }
    } else {
      QString path = settings->value("LastModulePath").toString();
      if (path.isEmpty()) path = settings->value("LastOpenPath").toString();
      QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Select an \"actions.txt\" file (hint: it's in the module dir)") ,
        tr("%1\\actions.txt").arg(path),
        tr("Txt file(*.txt)")
      );
      if (fileName.isEmpty()) {
        statusBar()->showMessage(tr("Split canceled."), 2000);
        return;
      }
      settings->setValue("LastModulePath",QFileInfo(fileName).absolutePath());
      int res = ani.Break(brfdata.animation, fileName.toAscii().data() );

      if (res==0) statusBar()->showMessage(tr("Nothing to split (or could not split)."));
      else {
        selector->updateData(brfdata);
        setModified(true);
        //selector->setCurrentIndex(2);

        statusBar()->showMessage(tr("Animation %2 split in %1 chunks!").arg(res).arg(ani.name), 2000);
      }
    }
  }
}



void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // accept just text/uri-list mime format
    //if (event->mimeData()->hasFormat("text/uri-list"))
    //{
        event->acceptProposedAction();
    //}
}

void MainWindow::about()
{

   QMessageBox::about(this, tr("Open-Brf"),
            tr("<b>ver 0.0.11 alpha</b><br>"
               "(%1)<br>"
               "by mtarini --- Marco Tarini ").arg(__DATE__) );
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList;
    QString fName;
    QFileInfo info;

    if (event->mimeData()->hasUrls())
    {
        urlList = event->mimeData()->urls(); // returns list of QUrls

        // if just text was dropped, urlList is empty (size == 0)
        if ( urlList.size() > 0) // if at least one QUrl is present in list
        {
            fName = urlList[0].toLocalFile(); // convert first QUrl to local path
            info.setFile( fName ); // information about file
            if ( info.isFile() ) loadFile( fName ); // if is file, setText
        }
    }
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();

    //menuBar()->addSeparator();
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editCutAct);
    editMenu->addAction(editCopyAct);
    editMenu->addAction(editPasteAct);
    //editMenu->addSeparator();
    //editMenu->addAction(editCutFrameAct);
    //editMenu->addAction(editCopyFrameAct);
    //editMenu->addAction(editPasteFrameAct);

    QMenu* importMenu=menuBar()->addMenu("&Import");

    importMenu->addAction(importStaticMeshAct);
    importMenu->addAction(importRiggedMeshAct);
    //importMenu->addAction(importMovingMeshAct);
    importMenu->addSeparator();
    importMenu->addAction(importAnimationAct);
    importMenu->addSeparator();
    importMenu->addAction(importSkeletonAct);
    importMenu->addSeparator();
    //importMenu->addAction(importBodyAct);
    //importMenu->addSeparator();
    importMenu->addAction(addNewMaterialAct);
    importMenu->addAction(addNewTextureAct);
    importMenu->addAction(addNewShaderAct);

    QMenu* optionMenu=menuBar()->addMenu("&Options");
    QMenu* onImport = optionMenu->addMenu("On import meshes");


    optionAfterMeshLoadMerge = new QAction("merge vertices and pos",this);
    optionAfterMeshLoadMerge->setCheckable(true);
    optionAfterMeshLoadRecompute = new QAction("recompute normals and merge",this);
    optionAfterMeshLoadRecompute->setCheckable(true);
    optionAfterMeshLoadNothing = new QAction("do nothing",this);
    optionAfterMeshLoadNothing->setCheckable(true);

    QActionGroup* group=new QActionGroup(this);
    group->addAction(optionAfterMeshLoadNothing);
    group->addAction(optionAfterMeshLoadMerge);
    group->addAction(optionAfterMeshLoadRecompute);
    group->setExclusive(true);

    onImport->addActions(group->actions());

    //onImport.addAction(optionAfterMeshLoadMerge);


    helpMenu = menuBar()->addMenu(tr("T&ools"));
    helpMenu->addAction(editRefAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

void MainWindow::saveOptions() const {
  int k=afterMeshImport();
  settings->setValue("afterMeshImport",k);
}

void MainWindow::loadOptions(){
  int k = 0;
  k = settings->value("afterMeshImport").toInt();
  optionAfterMeshLoadRecompute->setChecked(k==2);
  optionAfterMeshLoadMerge->setChecked(k==1);
  optionAfterMeshLoadNothing->setChecked(k==0);
}

int MainWindow::afterMeshImport() const{
  if (optionAfterMeshLoadRecompute->isChecked()) return 2;
  if (optionAfterMeshLoadMerge->isChecked()) return 1;
  return 0;
}


bool MainWindow::saveReference(){
  guiPanel->setReference(&reference);
  if ((int)reference.animation.size()>=glWidget->selRefAnimation) glWidget->selRefAnimation=-1;
  if (reference.GetFirstUnusedLetter()>=glWidget->selRefSkin) glWidget->selRefSkin=-1;
  if (!reference.Save("reference.brf")) {
    QMessageBox::information(this, tr("Open BRF"),
                              tr("Cannot save reference file!"));
  }
  setModified(false);
  return true;
}

int MainWindow::GetFirstUnusedRefLetter() const{
  return reference.GetFirstUnusedLetter();
}

bool MainWindow::editRef()
{
  if (editingRef) {
    reference = brfdata;
    if (!maybeSave()) return false;
    curFile = curFileBackup;
    brfdata = brfdataBackup;
    setEditingRef(false);
    updateTitle();
    selector->setup(brfdata);

    return true;
  } else {
    if (!maybeSave()) return false;
    curFileBackup = curFile;
    brfdataBackup = brfdata;
    curFile = QDir::currentPath()+"\\"+QString("reference.brf");
    brfdata = reference;
    setEditingRef(true);
    selector->setup(brfdata);
    statusBar()->showMessage(tr("Editing reference file..."), 2000);
    updateTitle();
    return true;
  }
}

bool MainWindow::loadFile(const QString &fileName)
{
  if (!maybeSave()) return false;
  setEditingRef(false);
  setCurrentFile(fileName);
  if (!brfdata.Load(fileName.toAscii().data())) {
     QMessageBox::information(this, tr("Open BRF"),
                              tr("Cannot load %1.").arg(fileName));

     return false;
  } else  {
    tryLoadMaterials();
    selector->setup(brfdata);
    //glWidget->selectNone();
    selector->setCurrentIndex(100); // for some reason, if I set the 0 message is not sent

    scanBrfDataForMaterials(brfdata);

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded!"), 2000);
    setModified(false);
    return true;
  }
}

bool MainWindow::saveFile(const QString &fileName)
{
  //setCurrentFile(fileName);
  if (!brfdata.Save(fileName.toAscii().data())) {
     QMessageBox::information(this, tr("Open BRF"),
                                 tr("Cannot write file %1.").arg(fileName));
     return false;
   } else {
     statusBar()->showMessage(tr("File saved!"), 2000);
     setModified(false);
     return true;
   }
}


bool MainWindow::open()
{
  if (maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File") ,
        settings->value("LastOpenPath").toString(),
        tr("Resource (*.brf)")
     );
       // QDir::currentPath());

    if (!fileName.isEmpty())
    if (!loadFile(fileName)) return false;
    setModified(false);
    return true;

  }
  return false;
}

bool MainWindow::save()
{
  if (editingRef) {
    reference = brfdata;
    statusBar()->showMessage(tr("Reference file saved!"), 4000);
    return saveReference();
  } else {
    if (curFile.isEmpty()) return saveAs();
    else return saveFile(curFile);
  }
}

bool MainWindow::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save File") ,
    (curFile.isEmpty())?
      settings->value("LastOpenPath").toString():
      curFile,
    tr("Resource (*.brf)")
  );
  if (fileName.isEmpty()) return false;

  setEditingRef(false);
  saveFile(fileName);
  setCurrentFile(fileName);
  setModified(false);
  return true;
}

void MainWindow::updateTitle(){
  QString maybestar = (isModified)?QString("(*)"):QString("");
  QString tit("OpenBrf");
  if (!editingRef) {
    if (curFile.isEmpty())
      setWindowTitle(tr("%1%2").arg(tit).arg(maybestar));
    else
      setWindowTitle(tr("%1 - %2%3").arg(tit).arg(curFile).arg(maybestar));
  } else
    setWindowTitle(tr("%1 - editing internal reference data%2").arg(tit).arg(maybestar));
}

void MainWindow::setModified(bool mod){
  isModified=mod;
  updateTitle();
}


void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    updateTitle();
    QStringList files = settings->value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings->setValue("recentFileList", files);
    settings->setValue("lastOpenPath",QFileInfo(fileName).absolutePath());

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

MainWindow::~MainWindow()
{
  saveOptions();
    //delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave()) {
    event->accept();
  } else {
    event->ignore();
  }
}

void MainWindow::newFile(){
  if (maybeSave()) {
    brfdata.Clear();
    curFile.clear();
    selector->setup( brfdata );
    setEditingRef(false);
    updateTitle();
  }

}
void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    editCutAct = new QAction(tr("Cut"), this);
    editCutAct->setShortcuts(QKeySequence::Cut);
    editCopyAct = new QAction(tr("Copy"), this);
    editCopyAct->setShortcuts(QKeySequence::Copy);
    editPasteAct = new QAction(tr("Paste"), this);
    editPasteAct->setShortcuts(QKeySequence::Paste);
    editPasteAct->setEnabled(false);

    editCutFrameAct = new QAction(tr("Cut frame"), this);
    editCutFrameAct->setShortcut(QString("alt+X"));
    editCopyFrameAct = new QAction(tr("Copy frame"), this);
    editCopyFrameAct->setShortcut(QString("alt+C"));
    editPasteFrameAct = new QAction(tr("Paste frame"), this);
    editPasteFrameAct->setShortcut(QString("alt+V"));
    editPasteFrameAct->setEnabled(false);

    connect(editCutAct, SIGNAL(triggered()), this, SLOT(editCut()));
    connect(editCopyAct, SIGNAL(triggered()), this, SLOT(editCopy()));
    connect(editPasteAct, SIGNAL(triggered()), this, SLOT(editPaste()));

    connect(editCutFrameAct, SIGNAL(triggered()), this, SLOT(editCutFrame()));
    connect(editCopyFrameAct, SIGNAL(triggered()), this, SLOT(editCopy()));
    connect(editPasteFrameAct, SIGNAL(triggered()), this, SLOT(editPasteFrame()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Alt+F4"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(tr("F1"));
    aboutAct->setStatusTip(tr("About OpenBRF"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    editRefAct = new QAction(tr("_"), this);

    connect(editRefAct, SIGNAL(triggered()), this, SLOT(editRef()));


    importStaticMeshAct = new QAction(tr("Static mesh"), this);
    importRiggedMeshAct = new QAction(tr("Rigged mesh"), this);
    importMovingMeshAct = new QAction(tr("Vertex-animated mesh"), this);
    importSkeletonAct = new QAction(tr("Skeleton"), this);
    importAnimationAct = new QAction(tr("Skeletal animation"), this);
    importBodyAct = new QAction(tr("Collision body"),this);
    addNewMaterialAct = new QAction(tr("New Material"),this);
    addNewTextureAct = new QAction(tr("New Texture"),this);
    addNewShaderAct = new QAction(tr("New Shader"),this);
}


bool MainWindow::openRecentFile()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action)
      return loadFile(action->data().toString());
  return false;
}

void MainWindow::updateRecentFileActions()
{
    QStringList files = settings->value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
