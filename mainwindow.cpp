#include "brfdata.h"
#include "glwidgets.h"
#include "selector.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_guipanel.h"
#include "askBoneDialog.h"
#include "askSkelDialog.h"
#include "askTexturenameDialog.h"
#include "askModErrorDialog.h"
#include "askTransformDialog.h"
#include "askCreaseDialog.h"

#include <QtGui>
#include <QDebug>

typedef QPair<int, int> Pair;

/*
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
      scanBrfForMaterials(path[i]+"/materials_face_gen.brf");
    }
  }
  //scanBrfForMaterials("C:/games/Mount&Blade1011/CommonRes/body_meshes.brf");
}

*/

void MainWindow::notifyDataChanged(){
  setModified(false);
}

void MainWindow::notifyCheckboardChanged(){
  aboutCheckboardAct->setVisible( (glWidget->lastMatErr.type!=0) );
}

void MainWindow::displayInfo(QString st,int howlong){
  statusBar()->showMessage(st,howlong);
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


static void setFlags(unsigned int &f, const QString q){
  bool ok;
  f= q.trimmed().replace("0x","").toUInt(&ok,16);
}
static void setFlags(unsigned int &f, QLineEdit *q){
  if (!q->hasFrame() && q->text().isEmpty()) return;
  q->setFrame(true);
  setFlags(f,q->text());
}
static void setUInt(unsigned int &f, const QLineEdit *q){
  f= q->text().trimmed().toUInt();
}
static void setInt(int &f, const QLineEdit *q){
  f= q->text().trimmed().toInt();
}
static void setFloat(float &f, QLineEdit *q){
  if (!q->hasFrame() && q->text().isEmpty()) return;
  q->setFrame(true);
  f= q->text().trimmed().toFloat();
}

static void setInt(int &f, QSpinBox *q){
  if (!q->hasFrame() && q->text().isEmpty()) return;
  q->setFrame(true);
  f= q->value();
}
static void setSign(int &s, const QLineEdit *q){
  int i =q->text().trimmed().toInt();
  if (i>=0) s=+1; else s=-1;
}
static void setString(char* st, QString s){
  s.truncate(254);
  sprintf(st,"%s",s.trimmed().toAscii().data());
}
static void setString(char* st, QLineEdit *q){
  if (!q->hasFrame() && q->text().isEmpty()) return;
  q->setFrame(true);
  setString(st, q->text());
}

template<class T>
static void _setFlag(vector<T> &v, int i, QString st){
  assert (i>=0 && i<(int)v.size());
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

static unsigned int _del(vector<T> &t, const QModelIndexList &l){
  for (int k=l.size()-1; k>=0; k--) {
    int i=l[k].row();
    if (i<0 || i>=(int)t.size()) continue;

    t.erase(t.begin()+i,t.begin()+i+1);
  }
  return t.size();
}
template< class T >
static char* _name(T &t, int i){
  if (i<0 || i>=(int)t.size()) return NULL;
  return t[i].name;
}
template< class T >
static bool _copy(vector<T> &t, const QModelIndexList &l, vector<T> &d){
  for (int k=0; k<l.size(); k++) {

    int i=l[k].row();
    if (i<0 || i>=(int)t.size()) continue;
    d.push_back(t[i]);
    QString name(t[i].name);
    if (T::tokenIndex()==TEXTURE) {
      // remove ".dds"
      name.truncate(name.lastIndexOf("."));
    }

    QApplication::clipboard()->setText(name);
  }

  return true;
}

template< class T >
bool _compareName(const T &ta, const T &tb){
  return strcmp(ta.name,tb.name)<0;
}

template< class T >
static bool _sort(vector<T> &t){
  std::sort(t.begin(),t.end(),_compareName<T>);
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
    _del( brfdata.shader[i].opt, selector->selectedList() );
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

  int ta =guiPanel->getCurrentSubpieceIndex(BODY);
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
    setModified(true);
    glWidget->update();
  }
}

void MainWindow::onChangeMeshMaterial(QString st){
  if (!glWidget) return;
  int n=0;

  QModelIndexList list=selector->selectedList();
  Ui::GuiPanel* u = guiPanel->ui;

  for (int i=0; i<(int)list.size(); i++) {
    int sel = list[i].row();
    if (sel<0 || sel>=(int)brfdata.mesh.size()) continue;
    setString(brfdata.mesh[sel].material, u->boxMaterial);
    n++;
  }
  statusBar()->showMessage( tr("Set %1 mesh materials to \"%2\"")
                            .arg(n).arg(u->boxMaterial->text()),5000 );
  glWidget->update();
  setModified(true);
}


void MainWindow::updateDataMaterial(){
  QModelIndexList list=selector->selectedList();
  Ui::GuiPanel* u = guiPanel->ui;

  for (int i=0; i<(int)list.size(); i++) {
    int sel = list[i].row();
    if (sel<0 || sel>=(int)brfdata.material.size()) continue;

    BrfMaterial &m(brfdata.material[sel]);

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
    setInt(m.renderOrder, u->leMatRendOrd);

    //mapMT[m.name] = m.diffuseA;
  }
  glWidget->update();
  setModified(true);
}

void MainWindow::onChangeTimeOfFrame(QString time){
  if (!glWidget) return;
  if (!selector) return;
  int i=selector->firstSelected();
  int j=currentDisplayFrame();
  int fl  =  time.toInt();

  if (selector->currentTabName()==MESH) {
    if (i>=0 && j>=0 && i<(int)brfdata.mesh.size() && j<(int)brfdata.mesh[i].frame.size()) {
      brfdata.mesh[i].frame[j].time=fl;
    }
  } else if (selector->currentTabName()==ANIMATION) {
    if (i>=0 && j>=0 && i<(int)brfdata.animation.size() && j<(int)brfdata.animation[i].frame.size()) {
      brfdata.animation[i].frame[j].index=fl;
      guiPanel->ui->boxAniMinFrame->display( brfdata.animation[i].FirstIndex() );
      guiPanel->ui->boxAniMaxFrame->display( brfdata.animation[i].LastIndex() );
    }
  } else assert(0);
  statusBar()->showMessage( tr("Set time of frame %1 to %2").arg(j).arg(fl),3000 );
  setModified(true);
  guiPanel->frameTime[j]=fl;

}

void MainWindow::tld2mabArmor(){
}

void MainWindow::tldMakeDwarfBoots(){
  int sdi=reference.Find("skel_dwarf",SKELETON);
  int shi=reference.Find("skel_human",SKELETON);
  if (sdi==-1 || shi==-1) {
    QMessageBox::information(this,
      tr("Open Brf"),tr("CAnnot find skel_human, skel_dwarf and skel_orc in reference data.\n")
    );
    return;
  }
  BrfSkeleton &sd (reference.skeleton[sdi]);
  BrfSkeleton &sh (reference.skeleton[shi]);
  //vector<BrfMesh> res;
  for (int ii=0; ii<selector->selectedList().size(); ii++) {
    int i = selector->selectedList()[ii].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;

    BrfMesh &m (brfdata.mesh[i]);

    m.ReskeletonizeHuman( sh, sd , 0.05);
    m.Scale(1.00,1.00,1,1,0.9,0.95);
    //m.TowardZero(0.008,0,0);

    QString tmp= QString("%1").arg(m.name);

    int indof = tmp.indexOf(".",0); if (indof == -1) indof = tmp.length();
    QString tmp2 = tmp.left(indof)+"_dwarf"+tmp.right(tmp.length()-indof);
    sprintf(m.name,"%s",tmp2.toAscii().data());

    //res.push_back(m);
    setModified(true);
  }
/*  for (unsigned int k=0; k<res.size(); k++) {
    insert(res[k]);
  }*/
  updateGui();
  updateSel();
}

void MainWindow::tldMakeDwarfSlim(){
  int sdi=reference.Find("skel_dwarf",SKELETON);
  if (sdi==-1) {
    QMessageBox::information(this,
      tr("Open Brf"),tr("CAnnot find skel_human, skel_dwarf and skel_orc in reference data.\n")
    );
    return;
  }
  BrfSkeleton &sd (reference.skeleton[sdi]);
  for (int ii=0; ii<selector->selectedList().size(); ii++) {
    int i = selector->selectedList()[ii].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;
    BrfMesh &m(brfdata.mesh[i]);
    if (m.frame.size()<4) return;

    m.frame[1].MakeSlim(0.95,0.95,&sd);
    setModified(true);
    updateGl();
  }

}

void MainWindow::mab2tldArmor(){

  //int shi=reference.Find("skel_orc_tall",SKELETON);
  int shi=reference.Find("skel_human",SKELETON);
  int sdi=reference.Find("skel_dwarf",SKELETON);
  int soi=reference.Find("skel_orc",SKELETON);
  if (shi==-1 || sdi==-1  || soi==-1) {
    QMessageBox::information(this,
      tr("Open Brf"),tr("Cannot find skel_human, skel_dwarf and skel_orc in reference data.\n")
    );
    return;
  }
  BrfSkeleton &sh (reference.skeleton[shi]);
  BrfSkeleton &sd (reference.skeleton[sdi]);
  BrfSkeleton &so (reference.skeleton[soi]);

  for (int ii=0; ii<selector->selectedList().size(); ii++) {
    int i = selector->selectedList()[ii].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;

    BrfMesh &m(brfdata.mesh[i]);
    BrfMesh fem;
    int lst = m.frame.size()-1;
    bool usefem = false;
    if (m.frame.size()==3) {
      usefem = true;
      fem=m;
      fem.KeepOnlyFrame(1);
      lst = 2;
    }
    if (m.frame.size()==5) {
      usefem = true;
      fem=m;
      fem.KeepOnlyFrame(3);
      lst = 4;
    }

    BrfMesh ml = m;  // last frame
    ml.KeepOnlyFrame(lst);// or 0
    m.KeepOnlyFrame( 0 );

    BrfMesh md = m; // dwarf mesh
    md.ReskeletonizeHuman( sh, sd , 0.05); // 0.05 = big arms!
    float t[16]={1,0,0,0, 0,1,0,0, 0,0,1.25,0, 0,0,0,1};
    md.Transform(t); // fat dwarf!
    m.AddFrameDirect(md);

    BrfMesh mo = m; // orc mesh
    mo.ReskeletonizeHuman( sh, so , 0.00);
    m.AddFrameDirect(mo);

    if (usefem) m.AddFrameDirect(fem); // feminine mesh

    m.AddFrameDirect(ml); // last frame



    m.AdjustNormDuplicates();
    setModified(true);
  }
  selector->updateData(brfdata);

}

void  MainWindow::mab2tldHead(){
  tldHead(1);
}
void  MainWindow::tld2mabHead(){
  tldHead(-1);
}
void  MainWindow::tldHead(float verse){
  bool changed = false;
  //vcg::Point3f tldHead(0,0,0);//0.0370, -0.01);
  //vcg::Point3f tldOrcHead(0,0.054033, -0.064549); // from horiz neck to straing neck
  vcg::Point3f tldOrcHead(0,-0.0390, 0.01); // from human to orc... almost halfway OLD


  //vcg::Point3f tldOrcHead(0,-0.0490, 0.01); // from human to orc... almost halfway
  vcg::Point3f tldOrcHeadS(0,tldOrcHead[2], -tldOrcHead[1]);
  vcg::Point3f zero(0,0,0);
  //int torax = 8;
  int head = 9;
  if (selector->currentTabName()==SKELETON) {
    int j = selector->firstSelected();
    if (j<0) return;


    //BrfAnimation &a(reference.animation[0]);
    BrfSkeleton &s(brfdata.skeleton[j]);

    //VcgMesh::add(s);
    //VcgMesh::moveBoneInSkelMesh(head,vcg::Point3f(0,tldOrcHead[2],tldOrcHead[1]));
    //VcgMesh::modifyBrfSkeleton(s);

    //Point3f v = tldHead*verse;
    //if (QString(s.name).contains("orc"));
    Point3f v= tldOrcHead*verse;
    //v = BrfSkeleton::adjustCoordSyst(v);

    //Matrix44f mat = (
    //  BrfSkeleton::adjustCoordSystHalf(
    //    s.GetBoneMatrices()[torax].transpose()
    //  ).transpose()
    //);

    //mat = vcg::Inverse(mat);
    //mat.transposeInPlace();

    //v = mat*v - mat*zero;
    v = BrfSkeleton::adjustCoordSyst(v);
    s.bone[head].t += v;
    changed=true;
  }
  if (selector->currentTabName()==MESH) {
    for (int j=0; j<selector->selectedList().size(); j++) {
      int i  =(selector->selectedList()[j]).row();
      BrfMesh &m(brfdata.mesh[i]);
      m.Translate(-tldOrcHead*verse);
      changed=true;
    }
  }

  if (changed) {
    setModified(true);
    glWidget->update();
  }

}

void MainWindow::onChangeFlags(QString st){
  if (!glWidget) return;
  //int n=1;
  //unsigned int fl  =  st.toUInt();
  st.truncate(254);
  int i=selector->firstSelected();

  switch(selector->currentTabName()) {
    case MESH:
      for (int j=0; j<selector->selectedList().size(); j++) {
        _setFlag(brfdata.mesh, (selector->selectedList()[j]).row(), st);
      }
    break;
    case TEXTURE:
      //_setFlag(brfdata.texture,i,st);
      for (int j=0; j<selector->selectedList().size(); j++) {
        _setFlag(brfdata.texture, (selector->selectedList()[j]).row(), st);
      }
      break;
    case SKELETON: _setFlag(brfdata.skeleton,i,st); break;
    //case ANIMATION: _setFlag(brfdata.animation,i,fl); break;
    default: assert(0);
  }
  //statusBar()->showMessage( tr("Set flag(s) to \"%1\"").arg(fl) );
  setModified(true);
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),inidata(brfdata)
{
  setWindowIcon(QIcon(":/openBrf.ico"));
  settings = new QSettings("mtarini", "OpenBRF");

  glWidget = new GLWidget(this,inidata);
  selector = new Selector(this);
  selector->reference=&reference;

  isModified=false;

  createMiniViewOptions();
  createActions();
  createMenus();
  setEditingRef(false);

  QSplitter* main = new QSplitter();
  guiPanel = new GuiPanel( this, inidata);

  main->addWidget(selector);
  main->addWidget(guiPanel);
  main->addWidget(glWidget);

  cancelNavStack();

  createConnections();


  reference.Load(QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("reference.brf").toAscii().data());
  setCentralWidget(main);

  updateTitle();

  glWidget->selected=1;
  glWidget->data = guiPanel->data = &brfdata;
  glWidget->reference = &reference;

  guiPanel->setReference(&reference);

  //tryLoadMaterials();

  this->setAcceptDrops(true);

  loadOptions();

  updatePaths();
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

void MainWindow::insert(const BrfMesh &o){ insert( brfdata.mesh, o); }
void MainWindow::insert(const BrfSkeleton &o){ insert( brfdata.skeleton, o); }
void MainWindow::insert(const BrfAnimation &o){ insert( brfdata.animation, o); }
void MainWindow::insert(const BrfTexture &o){   insert( brfdata.texture, o); }
void MainWindow::insert(const BrfMaterial &o){  insert( brfdata.material, o); }
void MainWindow::insert(const BrfShader &o){   insert( brfdata.shader, o); }
void MainWindow::insert(const BrfBody &o){   insert( brfdata.body, o); }

template<class BrfType> void MainWindow::insert( vector<BrfType> &v, const BrfType &o){
  int newpos;
  if (selector->currentTabName()!=BrfType::tokenIndex() ) {
    v.push_back(o);
    newpos=v.size()-1;
  } else {
    int i = selector->lastSelected()+1;
    if (i<0 || i>=(int)v.size()) i=v.size();
    if (i==(int)v.size()) v.push_back(o); else
    v.insert( v.begin()+i, o);
    newpos=i;
  }
  selector->updateData(brfdata);
  selectOne(BrfType::tokenIndex(), newpos);
}

void _setName(char* st, QString s){
  s.truncate(254);
  sprintf(st, "%s", s.toAscii().data());
}




template<class T>
bool MainWindow::addNewGeneral(){


  int tok = T::tokenIndex();

  AskTexturenameDialog d(this, tok==MATERIAL);
  if (tok==MATERIAL || tok==TEXTURE)
    d.setBrowsable(mabPath+"/Modules/"+modName+"/Textures");
  d.setLabel( "Name:" );
  d.setWindowTitle( QString("New %1").arg(tokenFullName[tok]) );
  d.setDef(QString("new_%1").arg(tokenFullName[tok]).toLower());
  int ok=d.exec();
  if (ok) {
    QStringList newName=d.getRes();

    for (int i=0; i<newName.size(); i++) {
      T m;
      _setName(m.name,newName[i]);
      m.SetDefault();
      insert(m);
      if (d.alsoAdd()) {
        BrfTexture t;
        _setName(t.name,newName[i]);
        t.SetDefault();
        insert(t);
      }
      //selector->updateData(brfdata);
    setModified(true);
    }
    return true;

  }

  return false;
}
bool MainWindow::addNewMaterial(){
  if (addNewGeneral<BrfMaterial>()){
    /*
    int i=selector->currentIndex();
    if (i>=0 && i<(int)brfdata.material.size()) {
      BrfMaterial &m(brfdata.material[i]);
      mapMT[m.name] = m.diffuseA;
    }*/
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

void MainWindow::applyAfterMeshImport(BrfMesh &m){
  switch (this->afterMeshImport()) {
    case 1:
      m.UnifyPos();
      m.UnifyVert(true,0.5);
      break;
    case 2:
      m.UnifyPos();
      m.UnifyVert(false);
      m.ComputeNormals();
      break;
  }
}

int MainWindow::gimmeASkeleton(int bs){
  return reference.getOneSkeleton(bs, glWidget->getRefSkeleton() );
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
  if (selector->currentTabName()==MESH)
  return guiPanel->ui->frameNumber->value(); //glWidget->getFrameNumber();
  if (selector->currentTabName()==ANIMATION)
  return guiPanel->ui->frameNumberAni->value()-1;
  assert(0);
  return 0;
}

void MainWindow::meshUnify(){

  for (int k=0; k<selector->selectedList().size(); k++) {
    int i= selector->selectedList()[k].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;

    BrfMesh &m (brfdata.mesh[i]);
    m.UnifyPos();
    m.UnifyVert(true,0.95);

  }
  updateGui();
  updateGl();
  setModified(true);
  statusBar()->showMessage(tr("Vertex unified."), 2000);
}

void MainWindow::meshMerge(){
  BrfMesh res;
  bool first=true;
  for (int i=0; i<selector->selectedList().size(); i++)
  {
    int j=selector->selectedList()[i].row();
    if (j<0 || j>=(int)brfdata.mesh.size()) continue;
    if (first) res = brfdata.mesh[j]; else {
      if (!res.Merge( brfdata.mesh[j] )) {
        QMessageBox::information(this,
          tr("Open Brf"),
          tr("Cannot merge these meshes\n (different number of frames,\n or rigged VS not rigged).\n")
        );
        return;
      }
    }
    first=false;
  }
  insert(res);
  setModified(true);
}

void MainWindow::updateGl(){
  glWidget->update();
}
void MainWindow::updateGui(){
  guiPanel->setSelection( selector->selectedList(),selector->currentTabName());
}
void MainWindow::updateSel(){
  selector->updateData(brfdata);
}

void MainWindow::meshRecomputeNormalsAndUnify(int crease){
  //int i = selector->firstSelected();

  for (int k=0; k<selector->selectedList().size(); k++) {
    int i= selector->selectedList()[k].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) return;

    BrfMesh &m (brfdata.mesh[i]);

    m.UnifyPos();

    m.DivideVert();
    m.ComputeNormals();
    m.UnifyVert(true,1-crease/100.0f*2);
    m.ComputeNormals();
  }
  updateGui();
  updateGl();
  statusBar()->showMessage(tr("Normals recomputed with %1% hard edges.").arg(crease), 2000);
}

void MainWindow::meshRecomputeNormalsAndUnify(){
  int i = selector->firstSelected();
  if (i<0) return;
  if (i>(int)brfdata.mesh.size()) return;

  AskCreaseDialog d(this);
  connect(d.slider(),SIGNAL(valueChanged(int)),this,SLOT(meshRecomputeNormalsAndUnify(int)));
  //d.exec();
  meshRecomputeNormalsAndUnify(d.slider()->value());
  d.exec();
  //d.setModal();
  disconnect(d.slider(),SIGNAL(valueChanged(int)),this,SLOT(meshRecomputeNormalsAndUnify(int)));

  setModified(true);
}


void MainWindow::flip(){
  int i =  selector->firstSelected();
  switch (selector->currentTabName()) {
  case MESH:
    brfdata.mesh[i].Flip();
    break;
  case BODY:
    brfdata.body[i].Flip();
    break;
  default: return;
  }
  updateGl();
  setModified(true);
}

void MainWindow::scale(){
  QModelIndexList list= selector->selectedList();
  if (selector->currentTabName()==MESH && list.size()>0) {
    float sca;
    bool ok;
    sca =  QInputDialog::getInteger(this,"Set Scale Factor","[in %, >100 means larger]",100,1,1000,5,&ok);
    if (ok) {
      for (int j=0; j<list.size(); j++){
        brfdata.mesh[list[j].row()].Scale(sca/100.0);
      }
      setModified(true);
    }
  }
  glWidget->update();
}


void MainWindow::shiftAni(){
  int j = selector->firstSelected();
  if (selector->currentTabName()==ANIMATION) {
    bool ok;
    int a=brfdata.animation[j].FirstIndex();
    int b=brfdata.animation[j].LastIndex();
    int k =  QInputDialog::getInteger(this,
      tr("Shift animation timings"),
      tr("Current Interval: [%1 %2]\nNew interval: [%1+k %2+k]\n\nSelect k:")
        .arg(a).arg(b),
      0,-a,1000,1,&ok);
    if (ok) {
      brfdata.animation[j].ShiftIndexInterval(k);
      selectOne(ANIMATION,j);
      setModified(true);
    }
  }
  updateGl();
}

void MainWindow::meshDiscardRig(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    m.Unskeletonize(reference.skeleton[9]);//gimmeASkeleton(20)]);

    m.DiscardRigging();
    setModified(true);
  }
  updateGui();
  updateGl();
}
void MainWindow::meshDiscardCol(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    m.ColorAll(0xFFFFFFFF);
    setModified(true);
  }
  updateGui();
  updateGl();
}
void MainWindow::meshDiscardAni(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);

    m.KeepOnlyFrame(guiPanel->getCurrentSubpieceIndex(MESH));
    setModified(true);
  }
  updateGui();
  updateGl();
}


void MainWindow::transform(){
  QModelIndexList list= selector->selectedList();
  if (list.size()>0) {

    AskTransformDialog *d = new AskTransformDialog(this);
    d->matrix = glWidget->extraMatrix;
    connect(d,SIGNAL(changed()),glWidget,SLOT(update()));
    bool ok = d->exec() == QDialog::Accepted;

    if (ok) {
      if (selector->currentTabName()==MESH)
      for (int j=0; j<list.size(); j++){
        brfdata.mesh[list[j].row()].Transform(d->matrix);
      }
      if (selector->currentTabName()==BODY)
      for (int j=0; j<list.size(); j++){
        brfdata.body[list[j].row()].Transform(d->matrix);
      }
      setModified(true);
    }
  }
  glWidget->clearExtraMatrix();
  glWidget->update();
}


void MainWindow::transferRigging(){
  int i = selector->firstSelected();
  QModelIndexList list= selector->selectedList();
  if (list.size()<2 || !brfdata.mesh[i].isRigged) {
    QMessageBox::information(this,
      tr("Transfer Rigging"),
      tr("Transfer rigging:\nselect a rigged mesh first,\nthen all target meshes.\n")
    );

  } else {
    vector<BrfMesh> mv;
    mv.push_back(brfdata.mesh[i]);
    for (int j=1; j<list.size(); j++){
      brfdata.mesh[list[j].row()].TransferRigging(mv,0,0);
    }
  }

  selector->updateData(brfdata);
  setModified(true);
}

void MainWindow::reskeletonize(){
int k=0;

  BrfMesh m2 = brfdata.mesh[selector->firstSelected()];
  int method=0, output=0;
  QPair<int,int> res = askRefSkel( m2.maxBone,  method, output);

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

for (int ii=0; ii<selector->selectedList().size(); ii++) {
  int i = selector->selectedList()[ii].row()+k;
  if (i<0) continue;
  if (i>(int)brfdata.mesh.size()) continue;

  BrfMesh m = brfdata.mesh[i];
  m.KeepOnlyFrame( currentDisplayFrame() );

  if (method==1)
    m.ReskeletonizeHuman( reference.skeleton[a], reference.skeleton[b]);
  else
    m.Reskeletonize( reference.skeleton[a], reference.skeleton[b]);
  if (output==1) {
    sprintf(m.name,"%s_%s",m.name,reference.skeleton[b].name);
    insert(m);
    k++;
  }
  if (output==0) {  
    brfdata.mesh[i].KeepOnlyFrame(0);
    brfdata.mesh[i].AddFrameDirect(m);
    brfdata.mesh[i].AddFrameDirect(brfdata.mesh[i]);
  }
  if (output==2)  {
    brfdata.mesh[i] = m;
  }
  setModified(true);
}
selector->updateData(brfdata);

}


void MainWindow::moveUpSel(){
  int i = selector->firstSelected();
  int j = i-1; if (j<0) return;
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
  bool res;
  switch (selector->currentTabName()) {
    case MESH: res=_swap(brfdata.mesh, i, j); break;
    case TEXTURE: res=_swap(brfdata.texture, i, j); break;
    case SHADER: res=_swap(brfdata.shader, i,j); break;
    case MATERIAL:  res=_swap(brfdata.material, i,j); break;
    case SKELETON: res=_swap(brfdata.skeleton, i,j); break;
    case ANIMATION: res=_swap(brfdata.animation, i,j); break;
    case BODY: res=_swap(brfdata.body, i,j); break;
    default: assert(0);
  }
  if (res) {
    selector->updateData(brfdata);
    selector->moveSel(+1);
    setModified(true);
  }
}
static void _findCommonPrefix(QString& a, QString b){
  for (int i=a.size(); i>=0; i--) {
    QString a0=a;
    QString b0=b;
    a0.truncate(i);
    b0.truncate(i);
    a = a0;
    if (a0==b0) break;
  }
}

void MainWindow::renameSel(){
  QString commonPrefix;
  int n = 0, max = selector->selectedList().size();
  TokenEnum t=(TokenEnum)selector->currentTabName();

  if (!max) return;
  for (int j=0; j<max; j++) {
    int i = selector->selectedList()[j].row();
    char* name=NULL;
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
      if (!n)  commonPrefix= QString("%1").arg(name);
      else  _findCommonPrefix( commonPrefix , name);
      n++;
    }
  }

  if (n>0) {
    bool ok;
    QString newPrefix;
    if (n==1) {
      newPrefix = QInputDialog::getText(
        this,
        tr("OpenBrf"),
        tr("Renaming %1...\nnew name:").arg(tokenFullName[t]),
        QLineEdit::Normal,
        QString(commonPrefix), &ok
      );
    }
    else {
      int ps = commonPrefix.size();
      newPrefix = QInputDialog::getText(
        this,
        tr("OpenBrf"),
        tr("%3 common prefix for %1 %2...\nnew prefix:").arg(n).arg(tokenPlurName[t]).arg((ps)?"Changing the":"Adding a"),
        QLineEdit::Normal,
        commonPrefix, &ok
      );
    }
    if (ok) {
      for (int j=0; j<max; j++) {
      int i = selector->selectedList()[j].row();
      char* name=NULL;

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
      QString newName = QString("%1").arg(name);
      int ps = commonPrefix.size();
      newName =newPrefix + newName.remove( 0,ps );
      _setName(name,newName);
      }
      setModified(true);
      selector->updateData(brfdata);
    }
  }
}
void MainWindow::deleteSel(){
  int i = selector->lastSelected();
  unsigned int res=0;
  switch (selector->currentTabName()) {
    case MESH: res=_del(brfdata.mesh, selector->selectedList()); break;
    case TEXTURE: res=_del(brfdata.texture,  selector->selectedList()); break;
    case SHADER: res=_del(brfdata.shader,  selector->selectedList()); break;
    case MATERIAL:  res=_del(brfdata.material,  selector->selectedList()); break;
    case SKELETON: res=_del(brfdata.skeleton,  selector->selectedList()); break;
    case ANIMATION: res=_del(brfdata.animation,  selector->selectedList()); break;
    case BODY: res=_del(brfdata.body,  selector->selectedList()); break;
    default: return; // no selection, no tab
  }
  if (res>0) {
    if (i>=(int)res) i=res-1;
    selector->selectOne(selector->currentTabName(),i);
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
  else {
    editCopyFrame();
    int j = guiPanel->getCurrentSubpieceIndex(MESH);
    m.frame.erase( m.frame.begin()+j,m.frame.begin()+j+1);
    m.AdjustNormDuplicates();
    selector->selectOne(MESH,i);
    setModified(true);
    //guiPanel->ui->frameNumber->setMaximum(m.frame.size()-1);
  }
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

  editPasteAct->setEnabled(true);
  editPasteFrameAct->setEnabled(true);

  editPasteRiggingAct->setEnabled(m.isRigged);

}


void MainWindow::editPasteRigging(){

  QModelIndexList list= selector->selectedList();
  bool allRigged=true;
  for (unsigned int i=0; i<clipboard.mesh.size(); i++)
    if (!clipboard.mesh[i].isRigged) allRigged = false;

  if (list.size()<1 || !allRigged || !clipboard.mesh.size() || selector->currentTabName()!=MESH) {
    QMessageBox::information(this,
      tr("Copy Rigging into another mesh"),
      tr("Copy Rigging into another mesh:\n"
         "- select one or more sample rigged mesh first,\n"
         "- copy them (ctrl-C)\n"
         "- then select one or more target meshes (rigged or otherwise),\n"
         "- then paste rigging.\n"
         "\n"
         "(works best if sample mesh is similar to target meshes)\n"
       )
    );

  } else {
    for (int j=0; j<list.size(); j++){
      brfdata.mesh[list[j].row()].TransferRigging(clipboard.mesh,0,0);
    }
  }

  selector->updateData(brfdata);
  setModified(true);
}

void MainWindow::editPasteFrame(){
  if (clipboard.mesh.size()==1 ) { //&& clipboard.mesh[0].frame.size()==1) {
    int i = selector->firstSelected();
    if (selector->currentTabName()!=MESH || i < 0 || i>=(int)brfdata.mesh.size()) {
      editPaste(); // no mesh selected: paste new mesh...
      return;
    }
    int j=guiPanel->getCurrentSubpieceIndex(MESH);
    if (j<0) j=0;
    BrfMesh &m(brfdata.mesh[i]);
    bool res=false;
    if (assembleAniMode()==0) {
      res = m.AddFrameMatchVert(clipboard.mesh[0],j);
      if (!res) statusBar()->showMessage(tr("Vertex number mismatch... using texture-coord matching instead of vertex-ordering"),7000);
    }
    if (assembleAniMode()==2) {
      res = m.AddFrameMatchPosOrDie(clipboard.mesh[0],j);
      if (!res) statusBar()->showMessage(tr("Vertex number mismatch... using texture-coord matching instead"),7000);
    }
    if (!res) m.AddFrameMatchTc(clipboard.mesh[0],j);
    statusBar()->showMessage(tr("Added frame %1").arg(j+1),2000);
    setModified(true);
    selector->selectOne(MESH,i);
  }

}

void MainWindow::editCut(){
  editCopy(false);
  deleteSel();
}


void MainWindow::sortEntries(){
  switch (selector->currentTabName()) {
    case MESH:     _sort(brfdata.mesh);  break;
    case TEXTURE:  _sort(brfdata.texture); break;
    case SHADER:   _sort(brfdata.shader); break;
    case MATERIAL: _sort(brfdata.material); break;
    case SKELETON: _sort(brfdata.skeleton); break;
    case ANIMATION:_sort(brfdata.animation); break;
    case BODY:     _sort(brfdata.body); break;
    default: return ; //assert(0);
  }
  setModified(true);
  updateSel();
  updateGui();
}

void MainWindow::editCopy(bool deselect){
  clipboard.Clear();
  editPasteAct->setEnabled(true);
  switch (selector->currentTabName()) {
    case MESH:     _copy(brfdata.mesh,     selector->selectedList(), clipboard.mesh);  break;
    case TEXTURE:  _copy(brfdata.texture,  selector->selectedList(), clipboard.texture); break;
    case SHADER:   _copy(brfdata.shader,   selector->selectedList(), clipboard.shader); break;
    case MATERIAL: _copy(brfdata.material, selector->selectedList(), clipboard.material); break;
    case SKELETON: _copy(brfdata.skeleton, selector->selectedList(), clipboard.skeleton); break;
    case ANIMATION:_copy(brfdata.animation,selector->selectedList(), clipboard.animation); break;
    case BODY:     _copy(brfdata.body,     selector->selectedList(), clipboard.body); break;
    default: return ; //assert(0);
  }

  if (selector->currentTabName()==MESH) {
    // maybe it was just rigged meshes?
    bool allRigged=true;
    for (unsigned int i=0; i<clipboard.mesh.size(); i++)
      if (!clipboard.mesh[i].isRigged) allRigged = false;
    editPasteRiggingAct->setEnabled(allRigged && clipboard.mesh.size()>0);

    // maybe it was a single frame mesh?
    editPasteFrameAct->setEnabled((clipboard.mesh.size()==1) && (clipboard.mesh[0].frame.size()==1));

    editPasteModificationAct->setEnabled(true);

  } else {
    editPasteRiggingAct->setEnabled(false);
    editPasteFrameAct->setEnabled(false);
    editPasteModificationAct->setEnabled(false);
  }


}

void MainWindow::editPasteMod(){
  int max = selector->selectedList().size();
  TokenEnum t=(TokenEnum)selector->currentTabName();


  if (clipboard.mesh.size()!=1 || clipboard.mesh[0].frame.size()!=2
      || t!=MESH || max <1 ) {
    QMessageBox::information(this,tr("OpenBrf"),tr("To use paste modification mesh: first"
                "copy a 2 frames mesh. Then, select one or more destination meshes, and \"paste modification\""
                "any vertex in any frame of the destination mesh that are in the same pos of frame 0,"
                "will be moved on the position of frame 1."));
  } else {
    for (int j=0; j<max; j++) {
      int i = selector->selectedList()[j].row();
      brfdata.mesh[i].CopyModification(clipboard.mesh[0]);
    }
    updateGl();
    updateGui();
    setModified(true);
  }
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
  //selector->updateData(brfdata);
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


Pair MainWindow:: askRefSkel(int nbones,  int &method, int &output){
  if (reference.skeleton.size()<2) return Pair(-1,-1);

  static int lastA=0, lastB=0;
  static int from =0, to=1;
  AskSkelDialog d(this,reference.skeleton,from,to, lastA, lastB);
  int res=d.exec();
  if (res==QDialog::Accepted) {
    lastA = method = d.getMethodType();
    lastB = output = d.getOutputType();
    return Pair(from=d.getSkelFrom (),to=d.getSkelTo());
  }
  else
    return Pair(-1,-1);
}

Pair MainWindow::askRefBoneInt(bool sayNotRigged, bool &isAtOrigin){
  if (!reference.skeleton.size()) return Pair(-1,-1);
  AskBoneDialog d(this,reference.skeleton);
  d.sayNotRigged(sayNotRigged);
  int res=d.exec();
  isAtOrigin = d.pieceAtOrigin();

  if (res==QDialog::Accepted) return Pair(d.getSkel(),d.getBone());
  else
  return Pair(-1,-1);
}

void MainWindow::setSelection(const QModelIndexList &l, int k){
  comboViewmodeSelector->setVisible(l.size()>1);
}

void MainWindow::meshRemoveBack(){
  int k=0;
  for (int j=0; j<selector->selectedList().size(); j++) {
    int i = selector->selectedList()[j].row();
    if (i<0 || i>=(int)brfdata.mesh.size()) continue;
    BrfMesh &m(brfdata.mesh[i]);
    m.RemoveBackfacingFaces();
    k++;
  }
  if (k>0) {
    setModified(true);
    guiPanel->setSelection(selector->selectedList(),MESH);
    glWidget->update();
  }
}

void MainWindow::meshAddBack(){
  int k=0;
  for (int j=0; j<selector->selectedList().size(); j++) {
    int i = selector->selectedList()[j].row();
    if (i<0 || i>=(int)brfdata.mesh.size()) continue;
    BrfMesh &m(brfdata.mesh[i]);
    m.AddBackfacingFaces();
    k++;
  }
  if (k>0) {
    setModified(true);
    guiPanel->setSelection(selector->selectedList(),MESH);
    glWidget->update();
  }
}

void MainWindow::meshMountOnBone(){
  bool isOri;
  Pair p = askRefBoneInt(false, isOri);

  if (p.first==-1) {
    statusBar()->showMessage(tr("Canceled."), 2000);
    return;
  }

  int k=0;
  for (int j=0; j<selector->selectedList().size(); j++) {
    int i = selector->selectedList()[j].row();
    if (i<0 || i>=(int)brfdata.mesh.size()) continue;
    BrfMesh &m(brfdata.mesh[i]);
    if (isOri)
    m.Apply(
      BrfSkeleton::adjustCoordSystHalf(
        reference.skeleton[p.first].GetBoneMatrices()[p.second].transpose()
      ).transpose()
    );
    m.SetUniformRig(p.second);
    k++;
  }
  if (k>0)
  setModified(true);
  //selector->setCurrentIndex(2);

  statusBar()->showMessage(tr("Mounted %1 mesh%2 on bone %3").arg(k).arg((k>1)?"es":"s").arg(p.second), 8000);

}


void MainWindow::addToRefMesh(int k){
  int i=selector->firstSelected();
  assert (selector->currentTabName()==MESH);
  assert(i<(int)brfdata.mesh.size());
  BrfMesh m = brfdata.mesh[i];
  if (!m.isRigged) {
    m.KeepOnlyFrame(guiPanel->getCurrentSubpieceIndex(MESH));
    bool isAtOrigin;
    Pair p = askRefBoneInt(true,isAtOrigin);

    if (p.first==-1) {
      statusBar()->showMessage(tr("Canceled."), 2000);
      return;
    }

    if (isAtOrigin)
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
  statusBar()->showMessage(tr("Added mesh %1 to set %2.").arg(m.name).arg(ch), 5000);
}

template <class T> const char* add(vector<T>& v, const vector<T>& t, int i){
  v.push_back(t[i]);
  return t[i].name;
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
      //QString path = this->modName; //settings->value("LastModulePath").toString();
      //if (path.isEmpty()) path = settings->value("LastOpenPath").toString();
      QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Select an \"actions.txt\" file (hint: it's in the module dir)") ,
        tr("%1\\actions.txt").arg(modPath()),
        tr("Txt file(*.txt)")
      );
      if (fileName.isEmpty()) {
        statusBar()->showMessage(tr("Split canceled."), 2000);
        return;
      }
      //settings->setValue("LastModulePath",QFileInfo(fileName).absolutePath());
      char newTxt[2048];
      int res = ani.Break(brfdata.animation, fileName.toAscii().data(),newTxt );

      if (res==0) statusBar()->showMessage(tr("Nothing to split (or could not split)."));
      else {
        selector->updateData(brfdata);
        setModified(true);
        //selector->setCurrentIndex(2);

        statusBar()->showMessage(tr("Animation %2 split in %1 chunks -- new animation.txt file save in \"%3\"!").arg(res).arg(ani.name).arg(newTxt), 8000);
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


int MainWindow::afterMeshImport() const{
  if (optionAfterMeshLoadRecompute->isChecked()) return 2;
  if (optionAfterMeshLoadMerge->isChecked()) return 1;
  return 0;
}

int MainWindow::assembleAniMode() const{
  if (optionAssembleAniMatchVert->isChecked()) return 0;
  if (optionAssembleAniQuiverMode->isChecked()) return 2;
  return 1;
}

void MainWindow::optionAutoFixTextureUpdated(){
  if (glWidget->fixTexturesOnSight  = optionAutoFixTextureOn->isChecked())
    glWidget->update();
}

void MainWindow::saveOptions() const {

  settings->setValue("afterMeshImport",afterMeshImport());
  settings->setValue("assembleAniMode",assembleAniMode());
  settings->setValue("lastSearchString",lastSearchString);
  //settings->setValue("autoFixDXT1",(int)(glWidget->fixTexturesOnSight));
  settings->setValue("autoZoom",(int)(glWidget->commonBBox));
  settings->setValue("groupMode",(int)(glWidget->getViewmodeMult() ));
}

QString MainWindow::modPath() const{
  return mabPath+"/Modules/"+modName;
}

void MainWindow::loadOptions(){
  {
  int k=1;
  QVariant s =settings->value("afterMeshImport");
  if (s.isValid()) k = s.toInt();
  optionAfterMeshLoadRecompute->setChecked(k==2);
  optionAfterMeshLoadMerge->setChecked(k==1);
  optionAfterMeshLoadNothing->setChecked(k==0);
  }

  {
  int k=0;
  QVariant s =settings->value("groupMode");
  if (s.isValid()) k = s.toInt();
  glWidget->setViewmodeMult(k);
  comboViewmodeBG->button(0)->setChecked(k==0);
  comboViewmodeBG->button(1)->setChecked(k==1);
  }

  {
  int k=0;
  QVariant s =settings->value("assembleAniMode");
  if (s.isValid()) k = s.toInt();
  optionAssembleAniMatchVert->setChecked(k==0);
  optionAssembleAniMatchTc->setChecked(k==1);
  optionAssembleAniQuiverMode->setChecked(k==2);
  }

  {
  int k=0;
  /*QVariant s =settings->value("autoFixDXT1");
  if (s.isValid()) k = s.toInt();
  optionAutoFixTextureOff->setChecked(k==0);
  optionAutoFixTextureOn->setChecked(k==1);*/
  glWidget->fixTexturesOnSight = k;
  }

  {
  int k=0;
  QVariant s =settings->value("autoZoom");
  if (s.isValid()) k = s.toInt();
  optionAutoZoomUseSelected->setChecked(k==0);
  optionAutoZoomUseGlobal->setChecked(k==1);
  glWidget->commonBBox = k;
  }

  modName = settings->value("modName").toString();
  if (modName.isEmpty()) modName = "native";
  mabPath = settings->value("mabPath").toString();
  inidata.setPath(mabPath,mabPath+"/Modules/"+modName);

  lastSearchString = settings->value("lastSearchString").toString();
}

void MainWindow::updatePaths(){
  modStatus->setText(QString("module:[<b>%1</b>]").arg(modName) );

  glWidget->texturePath[0]=mabPath+"/Textures";
  glWidget->texturePath[1]=modPath()+"/Textures";

  settings->setValue("modName",modName);
  settings->setValue("mabPath",mabPath);
}




bool MainWindow::saveReference(){
  guiPanel->setReference(&reference);
  if ((int)reference.animation.size()>=glWidget->selRefAnimation) glWidget->selRefAnimation=-1;
  if (reference.GetFirstUnusedLetter()>=glWidget->selRefSkin) glWidget->selRefSkin=-1;
  if (!reference.Save(
       QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("reference.brf").toAscii().data()
     ))
  {
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
    // stop editing refernce file
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
    curFile = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("reference.brf").toAscii().data();
    brfdata = reference;
    setEditingRef(true);
    selector->setup(brfdata);
    statusBar()->showMessage(tr("Editing reference file..."), 2000);
    updateTitle();
    return true;
  }
}

bool MainWindow::loadFile(const QString &_fileName)
{
  QString fileName = _fileName;
  fileName.replace("\\","/");
  if (!maybeSave()) return false;
  setEditingRef(false);
  setCurrentFile(fileName);
  if (!brfdata.Load(fileName.toAscii().data())) {
     QMessageBox::information(this, tr("Open BRF"),
                              tr("Cannot load %1.").arg(fileName));

     return false;
  } else  {
    //tryLoadMaterials();
    selector->setup(brfdata);
    //glWidget->selectNone();
    //selector->setCurrentIndex(100); // for some reason, if I set the 0 message is not sent
    int first = brfdata.FirstToken();
    if (first>=0) selectOne(first, 0);


    //scanBrfDataForMaterials(brfdata);

    //statusBar()->showMessage(tr("File loaded!"), 2000);
    setModified(false);
    return true;
  }
}

bool MainWindow::saveFile(const QString &fileName)
{
  if (curFileIndex!=-1) {
    inidata.file[curFileIndex] = brfdata;
    inidata.updateLists();
  }
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
  QString f0="M&B Resource (*.brf)",f1 = "WarBand Resource v.1 (*.brf)";
  QString selectedf = (brfdata.version==1)?f1:f0;
  QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save File") ,
    (curFile.isEmpty())?
      settings->value("LastOpenPath").toString():
      curFile,
    f0+";;"+f1,
     &selectedf
  );


  if (fileName.isEmpty()) return false;

  setEditingRef(false);
  setCurrentFile(fileName);
  brfdata.version=(selectedf==f1)?1:0;
  saveFile(fileName);
  setModified(false);
  return true;
}

void MainWindow::updateTitle(){
  QString maybestar = (isModified)?QString("(*)"):QString("");
  QString notInIni = (curFileIndex==-1)?QString(" [not in module.ini]"):QString("");
  QString tit("OpenBrf");
  if (!editingRef) {
    if (curFile.isEmpty())
      setWindowTitle(tr("%1%2").arg(tit).arg(maybestar));
    else
      setWindowTitle(tr("%1 - %2%3%4").arg(tit).arg(curFile).arg(maybestar).arg(notInIni));
  } else
    setWindowTitle(tr("%1 - editing internal reference data%2").arg(tit).arg(maybestar));
}

void MainWindow::setModified(bool mod){
  isModified=mod;
  updateTitle();
}

void MainWindow::guessPaths(QString fn){
  fn = QFileInfo(fn).canonicalPath();


  QString _modPath;

  QDir b(fn);
  if  (QString::compare(b.dirName(),"Resource",Qt::CaseInsensitive)==0)
  if (b.cdUp())  // go out of "resource"

  glWidget->texturePath[2]=b.absolutePath()+"/Textures";

  if (b.exists("module.ini"))
  if (b.cdUp()) // go out of <modName>
  if (QString::compare(b.dirName(),"modules",Qt::CaseInsensitive )==0) {
    QDir a(fn);
    a.cdUp();
    _modPath = a.absolutePath();
    modName = a.dirName();
    a.cdUp();
    a.cdUp(); // out of "modules"
    mabPath = a.absolutePath();
  }

  {
    QDir a(fn);
    a.cdUp();
    if (a.cd("ModuleSystem")) {
      if (a.exists("forOpenBRF.txt"))
        menuBar()->addAction(tldMenuAction);
    }
  }



  QDir a(fn);
  if  (QString::compare(a.dirName(),"commonres",Qt::CaseInsensitive)==0)
  if (a.cdUp()) {
    mabPath = a.absolutePath();
    if (a.cd("modules") && (a.cd(modName)) ) {
      _modPath = a.absolutePath();
    } else {
      modName = "native";
      _modPath = modPath();
    }
  }

  updatePaths();
  inidata.setPath(mabPath,_modPath);

  loadIni();
}

bool MainWindow::loadIni(){
  QTime qtime;
  qtime.start();

  bool res = (!inidata.load(true));

  cancelNavStack();

  guiPanel->setIniData(inidata);
  statusBar()->showMessage( tr("%5 %1 brf files from module.ini of \"%3\"-- %2 msec total [%4 text/mat/shad]").
      arg(inidata.file.size()).arg(qtime.elapsed()).arg(modName).arg(inidata.nRefObjects())
      .arg((res)?"scanned":"ERRORS found while scanning"),6000);
  return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
  curFile = fileName;
  QStringList files = settings->value("recentFileList").toStringList();
  files.removeAll(fileName);
  files.prepend(fileName);
  while (files.size() > MaxRecentFiles)
      files.removeLast();
  settings->setValue("recentFileList", files);
  settings->setValue("lastOpenPath",QFileInfo(fileName).absolutePath());

  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
    if (mainWin) mainWin->updateRecentFileActions();
  }

  // try to guess mab path and module...
  guessPaths(fileName);

  curFileIndex = inidata.findFile(fileName);

  updateTitle();

}

MainWindow::~MainWindow()
{
  saveOptions();
    //delete ui;
}

bool MainWindow::navigateLeft(){

  if (navigationStackPos<=0) return false;
  navigationStackPos--;
  QPair<Pair,QString> p = navigationStack[navigationStackPos];
  //qDebug()<<"p,first: "<<p.first<<" (on"<<inidata.filename.size() << ")\n";
  if (p.second.isEmpty()) return false;
  //if (p.first>=(int)inidata.filename.size()) return false;

  if (p.first.first!=curFileIndex || p.first.first==-1)
    if (!loadFile( //inidata.filename[p.first])
        p.second )
        ) return false;

  curFileIndex = p.first.first;

  int obj=MESH;
  if (navigationStackPos==1) obj=MATERIAL;
  selectOne(obj,p.first.second);
  //selector->currentWidget()->setFocus();
  guiPanel->setNavigationStackDepth( navigationStackPos );
  return true;
}

void MainWindow::cancelNavStack(){
  navigationStackPos = 0;
  guiPanel->setNavigationStackDepth( navigationStackPos );
  for (int i=0; i<3; i++) navigationStack[i]=
       QPair<Pair, QString>(Pair(-1,-1),"");
}

bool MainWindow::navigateRight(){
  //int s = selector->onlySelected(MESH);
  //BrfMesh *m = selectedMesh();

  QPair<Pair,QString> old(
      Pair(curFileIndex,selector->firstSelected()),
      curFile);

  if (selector->currentTabName()==MESH) {
    if (!guiPanel->ui->boxMaterial->hasFrame()) return false;

    QString st = guiPanel->ui->boxMaterial->text();
    Pair p = inidata.indexOf(st,MATERIAL);
    if (p.first==-1) {
      statusBar()->showMessage( tr("Navigate: cannot find material \"%1\" in current module").arg(st),6000);
      return false;
    }
    if (p.first!=curFileIndex) {
      if (!loadFile( inidata.filename[p.first])) return false;
    }
    navigationStackPos = 0;

    navigationStack[navigationStackPos]=old;
    //qDebug()<<"WRITING pos: "<<old.first<<"\n";
    curFileIndex = p.first;
    if (navigationStackPos<10) navigationStackPos ++;
    guiPanel->setNavigationStackDepth( navigationStackPos );


    // switch to file containing the material
    selectOne(MATERIAL,p.second);
    selector->currentWidget()->setFocus();


    return true;
  }
  if (selector->currentTabName()==MATERIAL) {
    QLineEdit *le = guiPanel->materialLeFocus();

    if (!le) return false;

    if (!le->hasFrame()) return false;

    QString st = le->text();
    if (st==QString("none")) return false;


    int ne;
    if (guiPanel->curMaterialFocus==GuiPanel::SHADERNAME){
      ne = SHADER;
    } else {
      ne = TEXTURE;
      //st = st+".dds";
    }
    Pair p = inidata.indexOf(st, ne);

    if (p.first==-1) {
      statusBar()->showMessage( tr("Navigate: cannot find %2 \"%1\" in current module")
                                .arg(st).arg(tokenFullName[ne]),6000);
      return false;
    }
    if (p.first!=curFileIndex) {
      if (!loadFile( inidata.filename[p.first])) return false;
    }
    navigationStackPos = 1;
    //navigationStackObj[navigationStackPos]=MATERIAL;
    navigationStack[navigationStackPos]=old;
    curFileIndex = p.first;
    if (navigationStackPos<10) navigationStackPos ++;
    guiPanel->setNavigationStackDepth( navigationStackPos );

    // switch to file containing the material
    selectOne(ne,p.second);
    selector->currentWidget()->setFocus();
    return true;
  }
  return false;
}
void  MainWindow::selectOne(int kind, int i){
  selector->selectOne(kind,i);
}
bool MainWindow::navigateUp(){
  return false;
}
bool MainWindow::navigateDown(){
  return false;
}
bool MainWindow::searchBrf(){
  return false;
}
bool MainWindow::refreshIni(){  
  inidata.updated=false;
  brfdata.ForgetTextureLocations();
  glWidget->forgetChachedTextures();
  glWidget->update();
  return loadIni();
}

bool MainWindow::checkIni(){

  if (!maybeSave()) return false;

  AskModErrorDialog *d=new AskModErrorDialog(this, inidata, false, "");
  d->setup();
  d->exec();
  if (d->i>=0) {
    loadFile(inidata.filename[d->i]);
    selectOne(d->kind, d->j);
  }
  delete d;
  return true;
}

bool MainWindow::searchIni(){

  if (!maybeSave()) return false;

  static bool optA = true;
  static int optB = -1;
  static QString optC;
  AskModErrorDialog *d=new AskModErrorDialog(this, inidata, true, lastSearchString );
  d->setOptions(optA,optB,optC);
  d->setup();
  d->exec();
  if (d->i>=0) {
    loadFile(inidata.filename[d->i]);
    selectOne(d->kind, d->j);
  }
  d->getOptions(&optA,&optB,&optC);
  delete d;
  return true;
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
    loadIni();
    brfdata.Clear();
    curFile.clear();
    selector->setup( brfdata );
    setEditingRef(false);
    updateTitle();
  }
}


bool readFile(QIODevice &device, QSettings::SettingsMap &map){
  return true;
}
bool writeFile(QIODevice &device, const QSettings::SettingsMap &map){
  return true;
}

void MainWindow::registerExtension(){
  //QSettings settings(QSettings::NativeFormat, QSettings::SystemScope,"HKEY_CLASSES_ROOT");
  {
  //QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
  //QSettings settings("HKEY_LOCAL_MACHINE", QSettings::NativeFormat);
  //settings.beginGroup("SOFTWARE");
  //settings.beginGroup("Classes");

  //settings.beginGroup(".brf");
  QSettings settings(QSettings::NativeFormat,QSettings::SystemScope, "Classes", ".brf");
    settings.setValue("","brf.resource");
  //settings.endGroup();
  }
  //QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
  QSettings settings(QSettings::NativeFormat,QSettings::SystemScope,"Classes", "brf.resource");

  //settings.beginGroup("brf.resource");
    settings.setValue("","Mount and Blade resource file");
    settings.beginGroup("DafualtIcon");
    settings.setValue("",QString("%1%2").arg(QCoreApplication::applicationFilePath()).arg(",0") );
    settings.endGroup();

    settings.beginGroup("shell");
      settings.setValue("","");
      settings.beginGroup("open");
        settings.setValue("","");
        settings.beginGroup("command");
          settings.setValue("",
             QString("\"%1\" \"%2\"").
             arg(QCoreApplication::applicationFilePath()).arg("%1") );
        settings.endGroup();
      settings.endGroup();
    settings.endGroup();
  //settings.endGroup();
  //QSettings::Format brfFormat = QSettings::registerFormat("brf",readFile,writeFile,Qt::CaseInsensitive);

  //QSettings fsettings(brfFormat, QSettings::UserSettings, "mtarini", "openBrf");

  //fsettings.setValue
  //int f = QWindowsMime::registerMimeType("brf");
  //if (!f) statusBar()->showMessage("Failed");
  //statusBar()->showMessage(tr("Registered %1?").arg(f));
  //QSetting sett(brfFormat,
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

#include "askFlagsDialog.h"

char* FlagNameArray[32] = {
  "No fog",
  "No Lighting",
  "",
  "No Z-write",
  "No depth Test", //
  "Specular enable",
  "Alpha test",
  "Uniform lighting",


  "Blend",
  "Blend add",
  "Blend multiply *",
  "Blend factor **",
  "Alpha test 1",
  "Alpha test 128",
  "Alpha test 256 *",
  "",

  "Render 1st",
  "Origin at camera",
  "LoD",
  "",
  "",
  "",
  "",
  "",

  "R",
  "R",
  "R",
  "R",
  "Invert bumpmap",
  "",
  "",
  "",

};

void MainWindow::setFlagsMaterial(){
  unsigned int curfOR=0, curfAND = 0xFFFFFFFF;

  QModelIndexList list=selector->selectedList();

  for (int i=0; i<(int)list.size(); i++) {
    int sel = list[i].row();
    if (sel<0 || sel>=(int)brfdata.material.size()) continue;

    BrfMaterial &m(brfdata.material[sel]);

    curfOR |= m.flags;
    curfAND &= m.flags;

  }

  //setFlags(curf,ui->leMatFlags->text());
  QStringList l ;
  for (int i=0; i<32; i++) l.append(FlagNameArray[i]);

  AskFlagsDialog *d = new AskFlagsDialog(this,curfOR,curfAND,l);
  d->setWindowTitle("Material flags");
  if (d->exec()==QDialog::Accepted) {
    //ui->leMatFlags->setText(StringH( d->getRes() ));
    //emit(dataMaterialChanged());

    for (int i=0; i<(int)list.size(); i++) {
      int sel = list[i].row();
      if (sel<0 || sel>=(int)brfdata.material.size()) continue;

      BrfMaterial &m(brfdata.material[sel]);

      m.flags |= d->toOne();
      m.flags &= d->toZero();

    }
    guiPanel->setSelection(list,MATERIAL);
    setModified(true);
  }

}
