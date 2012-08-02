/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include <QtGui>
#include <QDebug>
#include <algorithm>

#include "brfData.h"
#include "glwidgets.h"
#include "selector.h"
#include "mainwindow.h"
#include "tmp/ui_mainwindow.h"
#include "tmp/ui_guipanel.h"
#include "askBoneDialog.h"
#include "askSkelDialog.h"
#include "askTexturenameDialog.h"
#include "askModErrorDialog.h"
#include "askTransformDialog.h"
#include "askCreaseDialog.h"
#include "askNewUiPictureDialog.h"
#include "askUnrefTextureDialog.h"
#include "askSelectBrfDialog.h"
#include "askIntervalDialog.h"
#include "askHueSatBriDialog.h"
#include "askLodOptionsDialog.h"

typedef QPair<int, int> Pair;

int MainWindow::loadModAndDump(QString modpath, QString file){
  modpath = modpath.replace('/','\\');
  if (modpath.endsWith('\\')) modpath.chop(1);
  if (!guessPaths(modpath+"\\Resource")) return -1;
  if (!inidata.loadAll(3)) return -2;
  inidata.updateAllLists();
  if (!inidata.saveLists(file)) return -3;
  return 1;
}


void MainWindow::onActionTriggered(QAction *q){
  if (setNextActionAsRepeatable) {
    setNextActionAsRepeatable = false;
    if (q==repeatLastCommandAct) return;

    qDebug("Triggred action: %s\n",q->text().toAscii().data());
    qDebug("Command to be repeatable: \"%s\"\n",q->text().toAscii().data());
    repeatableAction = q;
    tokenOfRepeatableAction = selector->currentTabName();
    repeatLastCommandAct->setText(QString("&Repeat %1").arg(q->text()));
    repeatLastCommandAct->setEnabled(true);

  }

}


void MainWindow::repeatLastCommand(){
  if ((repeatableAction) && (tokenOfRepeatableAction == selector->currentTabName()))
    repeatableAction->trigger();
}


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
                        "Save changes?").arg((editingRef)?tr("Internal reference objects have"):tr("The dataset has")),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      if (ret == QMessageBox::Save) {
        if (editingRef) {
          reference = brfdata;
          return saveReference();
        } else
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
/*
static void setInt(int &f, QSpinBox *q){
  if (!q->hasFrame() && q->text().isEmpty()) return;
  q->setFrame(true);
  f= q->value();
}*/
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
	sprintf(t[i+1].name,"copy_%s",t[i].name);
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
	vector<bool> killme(t.size(),false);

	for (int k=0; k<l.size(); k++) {
    int i=l[k].row();
    if (i<0 || i>=(int)t.size()) continue;
		killme[i] = true;
  }
	uint j=0;
	for (uint i=0; i<t.size(); i++) {
		if (i!=j) t[j]=t[i];
		if (!killme[i]) j++;
	}
	t.resize(j);

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
    updateGl();
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
  updateGl();
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
    //int ro;
    //setInt(ro, u->leMatRendOrd);
    //m.SetRenderOrder(ro);

    //mapMT[m.name] = m.diffuseA;
  }
  updateGl();
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
      "OpenBRF",tr("CAnnot find skel_human, skel_dwarf and skel_orc in reference data.\n")
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
      "OpenBRF",tr("CAnnot find skel_human, skel_dwarf and skel_orc in reference data.\n")
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

void MainWindow::tldGrassAlphaPaint(){
  if (selector->currentTabName()!=MESH) return;
  bool done = false;
  for (int ii=0; ii<selector->selectedList().size(); ii++) {
      int i = selector->selectedList()[ii].row();
      if (i<0) continue;
      if (i>(int)brfdata.mesh.size()) continue;
      brfdata.mesh[i].paintAlphaAsZ(0,0.5);
      done = true;
  }
  if (done) {
    setModified(true);
    updateGl();
    updateGui();
  }
}

void MainWindow::tldShrinkAroundBones(){
  BrfSkeleton *skel  = currentDisplaySkeleton();
  if (!skel) {
    statusBar()->showMessage("No current skeleton found!");
    return;
  }

  int done =0;
  for (int ii=0; ii<selector->selectedList().size(); ii++) {
    int i = selector->selectedList()[ii].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;
    BrfMesh &m(brfdata.mesh[i]);
    int frame = guiPanel->getCurrentSubpieceIndex(MESH);
    if (frame<0 || frame>=(int)m.frame.size()) {
      statusBar()->showMessage(tr("Invalid frame %1").arg(frame));
      continue;
    }

    m.ShrinkAroundBones(*skel,frame);
    setModified(true);
    updateGl();
    done++;
  }
  statusBar()->showMessage(tr("%1 meshes shrunk around bones").arg(done));

}

void MainWindow::mab2tldArmor(){
  int k=0;
  //int shi=reference.Find("skel_orc_tall",SKELETON);
  int shi=reference.Find("skel_human",SKELETON);
  int sdi=reference.Find("skel_dwarf",SKELETON);
  int soi=reference.Find("skel_orc",SKELETON);
  if (shi==-1 || sdi==-1  || soi==-1) {
    QMessageBox::information(this,
      "OpenBRF",tr("Cannot find skel_human, skel_dwarf and skel_orc in reference data.\n")
    );
    return;
  }
  BrfSkeleton &sh (reference.skeleton[shi]);
  BrfSkeleton &sd (reference.skeleton[sdi]);
  BrfSkeleton &so (reference.skeleton[soi]);

  for (int ii=0; ii<selector->selectedList().size(); ii++) {
    int i = selector->selectedList()[ii].row()+k;
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
    updateGl();
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
    default: return; //assert(0);
  }
  statusBar()->showMessage( tr("Set flag(s) to \"%1\"").arg(st) );
  setModified(true);
}

int MainWindow::getLanguageOption(){
  QSettings *settings;
  settings = new QSettings("mtarini", "OpenBRF");

  QVariant s =settings->value("curLanguage");
  if (s.isValid()) return  s.toInt(); return  0;
}

void MainWindow::openModuleIniFile(){
  QDesktopServices::openUrl(QUrl::fromLocalFile(modPath()+"/module.ini"));
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),inidata(brfdata)
{
  usingWarband = true; // until proven otherwise
  useAlphaCommands = false;

  setWindowIcon(QIcon(":/openBrf.ico"));
  QFile f(":/femininizer.morpher");

  QByteArray r;
  bool ok = false;

  if (f.open(QIODevice::ReadOnly)){
   r = f.readAll();
   if (femininizer.Load(r.data())) ok = true;
  }

  if (!ok){
    QMessageBox::warning(this, tr("OpenBrf"),
                         QString("Internal error on loading femininizer"));
  }

  settings = new QSettings("mtarini", "OpenBRF");

  repeatableAction = 0;
  setNextActionAsRepeatable = false;

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

  reference.Load(QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("reference.brf").toStdWString().c_str());
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
  setLocale(QLocale::system());

  setLanguage( curLanguage );

  glWidget->setDefaultBgColor(background,true);

  connect(this->menuBar(), SIGNAL(triggered(QAction*)),this, SLOT(onActionTriggered(QAction *)));


}




bool MainWindow::setEditingRef(bool mode){
  editingRef = mode;
  if (editingRef) {
    editRefAct->setText(tr("Stop editing reference data"));
    editRefAct->setStatusTip(tr("Stop editing \"reference\" skeletons, animations & meshes, that OpenBrf uses to display data."));
  } else {
    editRefAct->setText(tr("Edit reference data"));
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

void MainWindow::insertOrReplace(const BrfMesh &o){ insertOrReplace( brfdata.mesh, o); }
void MainWindow::insertOrReplace(const BrfSkeleton &o){ insertOrReplace( brfdata.skeleton, o); }
void MainWindow::insertOrReplace(const BrfAnimation &o){ insertOrReplace( brfdata.animation, o); }
void MainWindow::insertOrReplace(const BrfTexture &o){   insertOrReplace( brfdata.texture, o); }
void MainWindow::insertOrReplace(const BrfMaterial &o){  insertOrReplace( brfdata.material, o); }
void MainWindow::insertOrReplace(const BrfShader &o){   insertOrReplace( brfdata.shader, o); }
void MainWindow::insertOrReplace(const BrfBody &o){   insertOrReplace( brfdata.body, o); }

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
  inidataChanged();
  updateSel();
  selectOne(BrfType::tokenIndex(), newpos);
}

template <class T> int _findByName( const vector<T> &v, const QString &s){
  for (unsigned int i=0; i<v.size(); i++)
    if (QString::compare(v[i].name,s,Qt::CaseInsensitive)==0) return i;
  return -1;
}

template<class BrfType> void MainWindow::insertOrReplace( vector<BrfType> &v, const BrfType &o){
  QString st(o.name);
  int i = _findByName(v,st);
  if (i>=0) {
    v[i]=o;
    inidataChanged();
    updateSel();
    selectOne(BrfType::tokenIndex(), i);
  }
  else insert(v,o);
}

void _setName(char* st, QString s){
  s.truncate(254);
  sprintf(st, "%s", s.toAscii().data());
}

bool MainWindow::addNewUiPicture(){
  AskNewUiPictureDialog d(this);
  d.setBrowsable(mabPath+"/Modules/"+modName+"/Textures");
  int ok=d.exec();
  if (ok){
    if (AskNewUiPictureDialog::name[0]==0) return false;
    BrfMaterial mat;
    BrfTexture tex;
    BrfMesh mes;

    mat.SetDefault();
    tex.SetDefault();
    mes.SetDefault();

    mes.MakeSingleQuad(
        AskNewUiPictureDialog::px*0.01,
        AskNewUiPictureDialog::py*0.01*0.75,
        AskNewUiPictureDialog::sx*0.01,
        AskNewUiPictureDialog::sy*0.01*0.75);

    switch (AskNewUiPictureDialog::overlayMode){
    case 0: mat.flags = 0x301; break; // darken
    case 1: mat.flags = 0x101; break;
    case 2: mat.flags = 0x0; break; // solild: only no fog
      // add 0x10 for no depth
    }

    sprintf(mat.name,"%s",AskNewUiPictureDialog::name);
    sprintf(tex.name,"%s.%s",
            AskNewUiPictureDialog::name,
            d.ext.toAscii().data());
    sprintf(mes.name,"%s",AskNewUiPictureDialog::name);
    sprintf(mat.diffuseA,"%s",AskNewUiPictureDialog::name);
    sprintf(mes.material,"%s",AskNewUiPictureDialog::name);
    if (AskNewUiPictureDialog::replace) {
      insertOrReplace(mat);
      insertOrReplace(tex);
      insertOrReplace(mes);
    } else {
      insert(mat);
      insert(tex);
      insert(mes);
    }
    setModified(true);
    return true;
  }
  return false;
}



template<class TT>
bool MainWindow::addNewGeneral(QStringList defaultst ){


  int tok = TT::tokenIndex();

  QString alsoAdd;
  if (tok==MATERIAL) alsoAdd = QString(tokenBrfName[TEXTURE ]).toLower();
  if (tok==TEXTURE ) alsoAdd = QString(tokenBrfName[MATERIAL]).toLower();

  AskTexturenameDialog d(this, alsoAdd);
  if (tok==MATERIAL || tok==TEXTURE)
    d.setBrowsable(mabPath+"/Modules/"+modName+"/Textures");
  //d.setLabel( tr("Name:") );
  if (defaultst.empty())
    d.setDef(tr("new_%1").arg(QString(tokenBrfName[tok]).toLower()) );
  else
    d.setRes(defaultst);

  d.setWindowTitle( tr("New %1").arg( IniData::tokenFullName(tok)  ) );


  int ok=d.exec();
  if (ok) {
    QStringList newName=d.getRes();

    for (int i=0; i<newName.size(); i++) {
      TT m;
      _setName(m.name,newName[i]);
      m.SetDefault();
      insert(m);
      if (d.alsoAdd()) {
        if (tok==MATERIAL) {
          BrfTexture t;
          _setName(t.name,newName[i]);
          t.SetDefault();
          insert(t);
        }
        if (tok==TEXTURE) {
          BrfMaterial t;
          _setName(t.name,newName[i]);
          t.SetDefault();
          insert(t);
        }
      }
      //selector->updateData(brfdata);
    setModified(true);
    }
    return true;

  }

  return false;
}
bool MainWindow::addNewMaterial(){

  if (addNewGeneral<BrfMaterial>( QStringList() )){
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
  return addNewGeneral<BrfTexture>( QStringList() );
}
bool MainWindow::addNewShader(){
  return addNewGeneral<BrfShader>( QStringList() );
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
        "OpenBRF",
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
BrfSkeleton* MainWindow::currentDisplaySkeleton(){
  int i = glWidget->getRefSkeleton();
  if (i<0) return NULL;
  if (i>=(int)reference.skeleton.size()) return NULL;
  return &(reference.skeleton[i]);
}
BrfAnimation* MainWindow::currentDisplayAnimation(){
  int i = glWidget->getRefSkelAni();
  if (i<0) return NULL;
  if (i>=(int)reference.animation.size()) return NULL;
  return &(reference.animation[i]);
  return NULL;
}
int MainWindow::currentDisplayFrame(){
  if (selector->currentTabName()==MESH)
  return guiPanel->ui->frameNumber->value(); //glWidget->getFrameNumber();
  if (selector->currentTabName()==ANIMATION)
  return guiPanel->ui->frameNumberAni->value()-1;
  assert(0);
  return 0;
}

int MainWindow::currentDisplaySkelAniFrame(){
  assert ((selector->currentTabName()==MESH));
  return glWidget->lastSkelAniFrameUsed;
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

void MainWindow::meshSubdivideIntoComponents(){
	for (int k=0; k<selector->selectedList().size(); k++) {
		int i= selector->selectedList()[k].row();
		if (i<0) continue;
		if (i>(int)brfdata.mesh.size()) continue;

		BrfMesh &m (brfdata.mesh[i]);
		//m.FixRigidObjectsInRigging();

		std::vector<BrfMesh> res;
		m.SubdivideIntoConnectedComponents(res);
		if (res.size()>10) {
			if (QMessageBox::warning(
				 this, tr("OpenBrf"),
				 tr("Mesh %1 will be \nsplit in %2 sub-meshes!.\n\nProceed?").arg(m.name).arg(res.size()),
				 QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No )
			break;
		}

		for (uint i=0; i<res.size(); i++) insert(res[i]);

		if (!res.size())
			statusBar()->showMessage(tr("Only one component found"), 2000);
		else
			statusBar()->showMessage(tr("Mesh separated into %1 pieces.").arg(res.size()), 2000);

		updateGui();
		updateGl();

		setModified(true);

		break;

	}


}

void MainWindow::meshFixRiggingRigidParts(){

	for (int k=0; k<selector->selectedList().size(); k++) {
		int i= selector->selectedList()[k].row();
		if (i<0) continue;
		if (i>(int)brfdata.mesh.size()) continue;

		BrfMesh &m (brfdata.mesh[i]);
		m.FixRigidObjectsInRigging();
	}
	updateGui();
	updateGl();


	setModified(true);
	statusBar()->showMessage(tr("Autofixed rigid parts."), 2000);
}


template<class BrfType>
void MainWindow::objectMergeSelected(vector<BrfType> &v){
  BrfType res;
  bool first=true;
  QString commonPrefix;
  for (int i=0; i<selector->selectedList().size(); i++)
  {
    int j=selector->selectedList()[i].row();
    if (j<0 || j>=(int)v.size()) continue;
    if (first) {
      res = v[j];
      commonPrefix = QString(v[j].name);
    } else {
      if (!res.Merge( v[j] )) {
        QMessageBox::information(this,
          "OpenBrf",
          tr("Cannot merge these meshes\n (different number of frames,\n or rigged VS not rigged).\n")
        );
        return;
      }
      _findCommonPrefix( commonPrefix, v[j].name );
    }
    first=false;
  }
	if (commonPrefix.endsWith(".")) commonPrefix.chop(1);
	else commonPrefix+="_combined";
  sprintf(res.name,"%s",commonPrefix.toAscii().data());
  insert(res);
  setModified(true);
}

void MainWindow::meshComputeAo(){
  bool inAlpha = optionAoInAlpha->isChecked() && useAlphaCommands;
  glWidget->renderAoOnMeshes(
    0.33f * currAoBrightnessLevel()/4.0f,
    0.4+0.6*(1-currAoFromAboveLevel()),
    currAoPerFace(),
    inAlpha
  );
  setModified(true);
  statusBar()->showMessage(tr("Computed AO%1").arg(inAlpha?tr("(in alpha channel)"):""), 2000);

  updateGui();
  updateGl();
	//guiPanel->ui->rbVertexcolor->setEnabled(true);
	guiPanel->ui->rbVertexcolor->click();

}

void MainWindow::learnFemininzation(){
  if (selector->currentTabName()!=MESH) return;
  int ndone = 0;

  for (int k=0; k<selector->selectedList().size(); k++) {
    int i= selector->selectedList()[k].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;
    BrfMesh& m (brfdata.mesh[i]);
    if (m.frame.size()!=3) continue;
    if (!ndone) femininizer.ResetLearning();
    femininizer.LearnFrom(m,1,2);
    ndone++;
  }
  if (ndone) {
    femininizer.FinishLearning();
    femininizer.Emphatize(0.2);
  }
  femininizer.Save("femininizer.morpher");
  if (ndone) QMessageBox::information(this,tr("OpenBRF"),tr("Learnt how to femininize an armour\nfrom %1 examples!").arg(ndone));
  else QMessageBox::warning(this,tr("OpenBRF"),tr("No mesh found to learn how to femininize an armour!"));
}


void MainWindow::meshFemininize(){
  if (selector->currentTabName()!=MESH) return;
  int ndone = 0;

  bool yesToAll = false;
  for (int k=0; k<selector->selectedList().size(); k++) {
    int i= selector->selectedList()[k].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) continue;
    BrfMesh& m (brfdata.mesh[i]);


    if (!yesToAll) {
      if (m.frame.size()>1) {
        int res = QMessageBox::warning(this,"OpenBRF",tr("Warning: mesh %1 has already a feminine frame.\n\nOverwrite it?").arg(m.name),
          QMessageBox::Yes|QMessageBox::YesToAll|QMessageBox::No
        );
        if (res==QMessageBox::No) continue;
        if (res==QMessageBox::YesToAll) yesToAll = true;
      }
    }
    while (m.frame.size()<3) m.frame.push_back(m.frame[0]);
    int feminineFrame = (usingWarband)?2:1;
    int masculineFrame = (usingWarband)?1:2;
    m.MorphFrame(masculineFrame,feminineFrame,femininizer);
    m.AddALittleOfBreast(feminineFrame);
    m.ComputeNormals(feminineFrame);
    m.frame[0].time = 0;
    m.frame[1].time = (usingWarband)?0:10;
    m.frame[2].time = (usingWarband)?10:20;
    ndone++;
  }
  if (ndone){
    setModified(true);
    updateGui();
    updateGl();
  }

}



void MainWindow::exportNames(){
  // TODO!!!
}

void MainWindow::meshMerge(){
  objectMergeSelected(brfdata.mesh);
}

void MainWindow::bodyMerge(){
  objectMergeSelected(brfdata.body);
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

// these should go as private members but I'm lazy right now
static double _crease = 0.5;
static bool _keepSeams = true;

void MainWindow::meshRecomputeNormalsAndUnify_onSlider(int i){
  _crease = 1-i/100.0f*2;
  meshRecomputeNormalsAndUnifyDoIt();
}

void MainWindow::meshRecomputeNormalsAndUnify_onCheckbox(bool i){
  _keepSeams = i;
  meshRecomputeNormalsAndUnifyDoIt();
}

void MainWindow::meshRecomputeNormalsAndUnifyDoIt(){

  //int i = selector->firstSelected();

  for (int k=0; k<selector->selectedList().size(); k++) {
    int i= selector->selectedList()[k].row();
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) return;

    BrfMesh &m (brfdata.mesh[i]);


    m.UnifyPos();

    m.DivideVert();
    m.ComputeNormals();
    m.UnifyVert(true,_crease);

    m.ComputeNormals();
    if (!_keepSeams)
    m.RemoveSeamsFromNormals(_crease);
  }
  updateGui();
  updateGl();
  statusBar()->showMessage(tr("Normals recomputed with %1% hard edges.").arg(int(100-_crease*50)), 2000);

}

void MainWindow::meshRecomputeNormalsAndUnify(){
  int i = selector->firstSelected();
  if (i<0) return;
  if (i>(int)brfdata.mesh.size()) return;

  AskCreaseDialog d(this);

  d.slider()->setValue(100-int(_crease*100));
  d.checkbox()->setChecked(_keepSeams);

  connect(d.slider(),SIGNAL(valueChanged(int)),
          this,SLOT(meshRecomputeNormalsAndUnify_onSlider(int)));
  connect(d.checkbox(),SIGNAL(clicked(bool)),
          this,SLOT(meshRecomputeNormalsAndUnify_onCheckbox(bool)));

  //d.exec();
  meshRecomputeNormalsAndUnifyDoIt();

  d.exec();
  //d.setModal();
  disconnect(d.checkbox(),SIGNAL(clicked(bool)),
             this,SLOT(meshRecomputeNormalsAndUnify_onCheckbox(bool)));
  disconnect(d.slider(),SIGNAL(valueChanged(int)),
             this,SLOT(meshRecomputeNormalsAndUnify_onSlider(int)));


  setModified(true);
}


void MainWindow::flip(){
  QModelIndexList list= selector->selectedList();
  switch (selector->currentTabName()) {
  case MESH:
    for (int j=0; j<list.size(); j++){
       brfdata.mesh[list[j].row()].Flip();
    }
    break;
  case BODY:
    for (int j=0; j<list.size(); j++){
      brfdata.body[list[j].row()].Flip();
    }
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
  updateGl();
}


char* cutAtDot(const char* c){
  static QString s;
  s.clear(); s+=c;
  int k = s.lastIndexOf('.');
  if (k>0) s.truncate(k);
  return s.toAscii().data();
}

void MainWindow::meshToBody(){
  QModelIndexList list= selector->selectedList();
  if (selector->currentTabName()==MESH && list.size()>0) {
    BrfBody b;
    BrfBodyPart bp;
    for (int j=0; j<list.size(); j++){
      BrfMesh &m (brfdata.mesh[list[j].row()]);
      m.AddToBody(bp);
      if (j==0) sprintf(b.name,"bo_%s",cutAtDot(m.name));
    }
    b.part.push_back(bp);
    b.MakeQuadDominant();
    b.UpdateBBox();

    insert(b);
    setModified(true);
  }

}

void MainWindow::bodyMakeQuadDominant(){
  QModelIndexList list= selector->selectedList();
  if (selector->currentTabName()==BODY && list.size()>0) {
    for (int j=0; j<list.size(); j++){
      brfdata.body[list[j].row()].MakeQuadDominant();
    }
    setModified(true);
  }
  updateGui();
  updateGl();
}

void MainWindow::shiftAni(){

  if (selector->currentTabName()!=ANIMATION) return;
  QModelIndexList list= selector->selectedList();
  for (int i=0; i<list.size(); i++){
    int j = list[i].row();
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

void MainWindow::aniMirror(){
	if (selector->currentTabName()!=ANIMATION) return;
	QModelIndexList list= selector->selectedList();
	for (int i=0; i<list.size(); i++){
		int j = list[i].row();

		BrfAnimation a = brfdata.animation[j];
		int si = glWidget->getRefSkeleton();
		if (si<0) continue;
		BrfSkeleton s =reference.skeleton[si];

		if (brfdata.animation[j].Mirror(a,s))	setModified(true);
	}
	updateGl();
}

void MainWindow::aniExtractInterval(){
  if (selector->currentTabName()!=ANIMATION) return;
  int j = selector->firstSelected();
  BrfAnimation newani;

  BrfAnimation &ani(brfdata.animation[j]);
  AskIntervalDialog d(this, tr("Extract Interval"), ani.FirstIndex(), ani.LastIndex());
  if (d.exec()!=QDialog::Accepted) return;
  int res = ani.ExtractIndexInterval(newani, d.getA(),d.getB());
  if (res>0){
    insert(newani);
  }
}


void MainWindow::aniRemoveInterval(){
  if (selector->currentTabName()!=ANIMATION) return;
  int j = selector->firstSelected();
  BrfAnimation &ani(brfdata.animation[j]);
  AskIntervalDialog d(this, tr("Remove Interval"), ani.FirstIndex(), ani.LastIndex());
  if (d.exec()!=QDialog::Accepted) return;
  int res = ani.RemoveIndexInterval(d.getA(),d.getB());
  if (res>0) {
    setModified(true);
    updateGui();
    updateGl();
  }
}

void MainWindow::aniMerge(){
  if (selector->currentTabName()!=ANIMATION) return;
  BrfAnimation res;
  QModelIndexList list= selector->selectedList();
  for (int i=0; i<list.size(); i++){
    int j = list[i].row();
    if (i==0) res = brfdata.animation[j];
    else {
      BrfAnimation tmp = res;
      if (!res.Merge(tmp,brfdata.animation[j])) {
         QMessageBox::information(this,"OpenBrf",
          tr("Cannot merge these animations\n (different number of bones).\n")
         );
         return;
      }
    }
  }
  insert(res);
}


void MainWindow::meshFreezeFrame(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    //m.Unskeletonize(reference.skeleton[9]);//gimmeASkeleton(20)]);
    if (m.isRigged) {

      BrfSkeleton *s = currentDisplaySkeleton();
      BrfAnimation *a = currentDisplayAnimation();
      int f = currentDisplaySkelAniFrame();
      if (s && a && (f>=0))
        m.FreezeFrame(*s,*a,f);
      else assert(0);
      m.DiscardRigging();
      setModified(true);
    }

  }
  updateGui();
  updateGl();

}

void MainWindow::meshDiscardRig(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    //m.Unskeletonize(reference.skeleton[9]);//gimmeASkeleton(20)]);

    m.DiscardRigging();
    setModified(true);
  }
  updateGui();
  updateGl();
}

void MainWindow::meshRecomputeTangents(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    m.ComputeTangents();
    setModified(true);
  }
  updateGui();
  updateGl();
}

void MainWindow::meshRecolor(){
  QColor color = QColorDialog::getColor(Qt::white, this);
  if (!color.isValid()) return;
  uint r = color.red(); //uint(color.redF()*255.0);
  uint g = color.green(); //uint(color.greenF()*255.0);
  uint b = color.blue(); //uint(color.blueF()*255.0);
  uint a = 0xFF;

  uint col = b | (g<<8) | (r<<16) | (a<<24);
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    m.ColorAll(col);
    setModified(true);
  }
  guiPanel->ui->rbVertexcolor->click();
  updateGui();
  updateGl();
}

unsigned int tuneColor(unsigned int col, int contr, int dh, int ds, int db){
  QColor c(col&0xFF,(col>>8)&0xFF,(col>>16)&0xFF,(col>>24)&0xFF);
  c.convertTo(QColor::Hsv);
  qreal h,s,b,a;
  c.getHsvF(&h,&s,&b,&a);
  h = c.hueF();
  if (h<0) h=0;
    h+=dh/200.0;

  h = h+100 - floor(h+100);
  //h = dh;
  b+= 0.5 * contr/50.0 * (0.5*(sin(3.1415*(b-0.5))+1.0)-b) + db/100.0;
  s+=ds/100.0;
  if (h<0) h=0;
  if (s<0) s=0;
  if (b<0) b=0;
  if (h>1) h=1;
  if (s>1) s=1;
  if (b>1) b=1;
  c.setHsvF(h,s,b,a);

  c.convertTo(QColor::Rgb);
  unsigned int alpha = c.alpha();
  return (c.red()&0xff) | ((c.green()&0xff)<<8) | ((c.blue()&0xff)<<16) | (alpha<<24);
}


void MainWindow::meshTuneColorUndo(bool storeUndo){
  static std::vector<unsigned int> stored;
  if (storeUndo) stored.clear();
  QModelIndexList list= selector->selectedList();
  for (int j=0,h=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    for (uint i=0; i<m.vert.size(); i++,h++) {
      if (storeUndo) stored.push_back(m.vert[i].col);
      else m.vert[i].col = stored[h];
    }
  }

}

void MainWindow::meshTuneColorDo(int c,int h,int s,int b){
  meshTuneColorUndo(false);
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    m.TuneColors(c,h,s,b);
  }
  updateGl();
}

void MainWindow::meshTuneColor(){
  meshTuneColorUndo(true);
  AskHueSatBriDialog *d = new AskHueSatBriDialog(this);
  connect(d, SIGNAL(anySliderMoved(int,int,int,int)), this, SLOT(meshTuneColorDo(int,int,int,int)));
  int res = d->exec();
  if (res!=QDialog::Accepted) meshTuneColorUndo(false); else {
    setModified(true);
    guiPanel->ui->rbVertexcolor->click();
  }
  updateGui();
  updateGl();
  delete d;
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
void MainWindow::meshDiscardTan(){
  QModelIndexList list= selector->selectedList();
  for (int j=0; j<list.size(); j++){
    BrfMesh &m(brfdata.mesh[list[j].row()]);
    m.DiscardTangentField();
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
  updateGl();
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
      "OpenBRF",
      tr("Same skeleton:\nreskeletonization canceled.\n")
    );
    return;
  }
  if (reference.skeleton[a].bone.size()!=reference.skeleton[b].bone.size()) {
    QMessageBox::information(this,
      "OpenBRF",
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
  inidataChanged();
	setModified(true,false);
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
    inidataChanged();
		setModified(true,false);
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
        tr("Renaming %1...\nnew name:").arg(IniData::tokenFullName(t)),
        QLineEdit::Normal,
        QString(commonPrefix), &ok
      );
      if ((t==TEXTURE) && (!newPrefix.contains('.'))) newPrefix+=".dds";
    }
    else {
      int ps = commonPrefix.size();
      newPrefix = QInputDialog::getText(
        this,
        tr("OpenBrf"),
        tr("%3 common prefix for %1 %2...\nnew prefix:").arg(n).arg(IniData::tokenPlurName(t)).arg((ps)?tr("Changing the"):tr("Adding a")),
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
      inidataChanged();
      updateSel();

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
  inidataChanged();
  updateSel();
	setModified(true,false);
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

  saveSystemClipboard();
}

void MainWindow::onClipboardChange(){
  const QMimeData *mime = QApplication::clipboard()->mimeData();
  if (!mime) return;
  bool isMyData = mime->hasFormat("application/openBrf");

  //QMessageBox::information(this,"Clipboard stuff",(isMyData)?"with my data!":"not my data");

  if (isMyData) {
    loadSystemClipboard();
  } else {
    clipboard.Clear();
  }
  statusBar()->showMessage((isMyData)?
    QString("%1 new items found in clipboard...").arg(clipboard.totSize())
    :"Unusable data in clipboard");

  editPasteAct->setEnabled( clipboard.totSize() );

  if (clipboard.mesh.size()!=0){
    // maybe it was just rigged meshes?

    bool allRigged=true;
    for (unsigned int i=0; i<clipboard.mesh.size(); i++)
      if (!clipboard.mesh[i].isRigged) allRigged = false;
    editPasteRiggingAct->setEnabled(allRigged && clipboard.mesh.size()>0);


    // maybe it was a single frame mesh?
    editPasteFrameAct->setEnabled((clipboard.mesh.size()==1) && (clipboard.mesh[0].frame.size()==1));

		editPasteTextcoordsAct->setEnabled(clipboard.mesh.size()==1);
    editPasteModificationAct->setEnabled(true);

  } else {
    editPasteRiggingAct->setEnabled(false);
    editPasteFrameAct->setEnabled(false);
    editPasteModificationAct->setEnabled(false);
		editPasteFrameAct->setEnabled(false);
		editPasteTextcoordsAct->setEnabled(false);
  }

	editPasteAniLowerPartsAct->setEnabled((clipboard.animation.size()==1));

  editPasteTimingsAct->setEnabled(
      (
          ((clipboard.mesh.size()==1)&&(clipboard.mesh[0].frame.size()>0))
          ||(clipboard.animation.size()==1)
      )
  );

}

void MainWindow::saveSystemClipboard(){

  QMimeData *mime = new QMimeData();

  // save string as object name
  const char* text = clipboard.GetFirstObjectName();
  if (text) mime->setText( text );

  // save data
  QTemporaryFile file;
  file.open();
  FILE* pFile = fdopen(file.handle(), "wb");
  clipboard.Save(pFile);
  fflush(pFile);

  QFile refile(file.fileName());

  refile.open(QIODevice::ReadOnly);

  /*
  //refile.reset();
  if (!refile.isOpen()) {
    QMessageBox::information(this,"Cannot open file",QString("%1").arg(refile.fileName()));
  } else {
    QMessageBox::information(this,"...",
      QString("file size: %1 (%2)").arg(refile.size()).arg(refile.readAll().size())
    );
    refile.reset();
  }*/

  mime->setData("application/openBrf",refile.readAll());

  refile.close();
  file.close();


	QApplication::clipboard()->clear();
  QApplication::clipboard()->setMimeData(mime);


}

void MainWindow::loadSystemClipboard(){
  clipboard.Clear();
  const QMimeData *mime = QApplication::clipboard()->mimeData();
  const QByteArray &ba = mime->data("application/openBrf");
  QTemporaryFile f;
  wchar_t fn[1000];
  f.open();
  f.fileName().toWCharArray(fn);
  fn[f.fileName().size()]=0;
  f.write(ba);
  f.flush();
  /*
  if (!f.isOpen()) {
    QMessageBox::information(this,"Cannot open file",QString("%1").arg(f.fileName()));
  } else {
    QMessageBox::information(this,"load",
      QString("filename: %3\n%4,file size: %1 (%2)").
      arg(f.size()).arg(ba.size()).arg(f.fileName()).arg(QString("pippo"))
    );
  }*/

  //FILE* pFile = fdopen(f.handle(), "rb");
  clipboard.Load(fn);
  f.close();
  //
}

void MainWindow::editPasteAniLowerParts(){
	QModelIndexList list= selector->selectedList();

	if (clipboard.animation.size()!=1) return;

	for (int j=0; j<list.size(); j++){
		brfdata.animation[list[j].row()].CopyLowerParts(clipboard.animation[0]);
	}

	setModified(true);
	updateGui();
	updateGl();


}

void MainWindow::editPasteTextcoords(){

	QModelIndexList list= selector->selectedList();
	for (int j=0; j<list.size(); j++){
		brfdata.mesh[list[j].row()].CopyTextcoords(clipboard.mesh[0]);
	}


	updateSel();
	setModified(true);
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
  clipboard.Clear();
  addSelectedToClipBoard();
  deleteSel();
  saveSystemClipboard();
}

void MainWindow::editCopy(){
  clipboard.Clear();
  addSelectedToClipBoard();
  saveSystemClipboard();
}

void MainWindow::editAddToCopy(){
  addSelectedToClipBoard();
  saveSystemClipboard();
}

void MainWindow::editCopyComplete(){
  clipboard.Clear();
  addSelectedToClipBoard();
  completeClipboard(false);
  saveSystemClipboard();
}

void MainWindow::editCutComplete(){
  clipboard.Clear();
  addSelectedToClipBoard();
  completeClipboard(true);
  deleteSel();
  saveSystemClipboard();
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
  inidataChanged();
  updateSel();
  updateGui();
}



void MainWindow::completeClipboard(bool andDelete){
  std::vector<bool> takeM(brfdata.material.size(),false);
  std::vector<bool> takeT(brfdata.texture.size(),false);
  std::vector<bool> takeS(brfdata.shader.size(),false);

  for(uint i=0; i<clipboard.mesh.size(); i++){
    int k = brfdata.Find(clipboard.mesh[i].material,MATERIAL);
    if (k>=0) takeM[k]=true;
  }

  for (int i=takeM.size()-1; i>=0; i--) if (takeM[i]) {
    qDebug("i = %d",i);
    clipboard.material.push_back( brfdata.material[i] );
    if (andDelete) brfdata.material.erase(brfdata.material.begin()+i);
  }

  for(uint i=0; i<clipboard.material.size(); i++){
    int k;
    k = brfdata.FindTextureWithExt(clipboard.material[i].diffuseA);
    if (k>-1) takeT[k] = true;
    k = brfdata.FindTextureWithExt(clipboard.material[i].diffuseB);
    if (k>-1) takeT[k] = true;
    k = brfdata.FindTextureWithExt(clipboard.material[i].bump);
    if (k>-1) takeT[k] = true;
    k = brfdata.FindTextureWithExt(clipboard.material[i].enviro);
    if (k>-1) takeT[k] = true;
    k = brfdata.FindTextureWithExt(clipboard.material[i].spec);
    if (k>-1) takeT[k] = true;
    k = brfdata.Find(clipboard.material[i].shader,SHADER);
    if (k>-1) takeS[k] = true;
  }

  for (int i=takeT.size()-1; i>=0; i--) if (takeT[i]) {
    clipboard.texture.push_back( brfdata.texture[i] );
    if (andDelete) brfdata.texture.erase(brfdata.texture.begin()+i);
  }

  for (int i=takeS.size()-1; i>=0; i--) if (takeS[i]) {
    clipboard.shader.push_back( brfdata.shader[i] );
    if (andDelete) brfdata.shader.erase(brfdata.shader.begin()+i);
  }

}

void MainWindow::addSelectedToClipBoard(){
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
}

void MainWindow::editPasteTimings(){
  std::vector<int> timings;
  if (clipboard.mesh.size()==1) {
    clipboard.mesh[0].GetTimings(timings);
  } else if (clipboard.animation.size()==1) {
    clipboard.animation[0].GetTimings(timings);
  } else {
    statusBar()->showMessage(tr("Cannot paste timings! Select *one* animated mesh or skel animation"),8000);
    return;
  }

  int max = selector->selectedList().size();
  TokenEnum t=(TokenEnum)selector->currentTabName();


  if (t==MESH){
    for (int j=0; j<max; j++) {
      int i = selector->selectedList()[j].row();
      brfdata.mesh[i].SetTimings(timings);
    }
    statusBar()->showMessage(tr("Pasted timings over %1 (animated) mesh").arg(max),8000);
  } else if (t==ANIMATION){
    for (int j=0; j<max; j++) {
      int i = selector->selectedList()[j].row();
      brfdata.animation[i].SetTimings(timings);
    }
    statusBar()->showMessage(tr("Pasted timings over %1 skeletal animations").arg(max),8000);
  } else {
    max =0;
    statusBar()->showMessage(tr("Cannot paste times over that").arg(max),8000);
  }

  if (max>0) {  updateGui();  setModified(true);  }
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
  for (int i=0; i<(int)clipboard.texture.size(); i++) insert(clipboard.texture[i]);
  for (int i=0; i<(int)clipboard.shader.size(); i++) insert(clipboard.shader[i]);
  for (int i=0; i<(int)clipboard.material.size(); i++) insert(clipboard.material[i]);
  for (int i=0; i<(int)clipboard.mesh.size(); i++) insert(clipboard.mesh[i]);
  for (int i=0; i<(int)clipboard.skeleton.size(); i++) insert(clipboard.skeleton[i]);
  for (int i=0; i<(int)clipboard.animation.size(); i++) insert(clipboard.animation[i]);
  //brfdata.Merge(clipboard);
  //selector->updateData(brfdata);
	setModified(true,false);
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

  inidataChanged();
  updateSel();

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
    updateGl();
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
    updateGl();
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
    //m.ResizeTextCoords(Point2f(0.75,0.75),Point2f(1,1)); return;
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
        updateSel();
        inidataChanged();

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
      wchar_t newTxt[2048];
			vector<BrfAnimation> resVec;
			int res = ani.Break(resVec, fileName.toStdWString().c_str(),newTxt );

      if (res==0) statusBar()->showMessage(tr("Nothing to split (or could not split)."));
      else {
				for (uint i=0; i<resVec.size(); i++) insert(resVec[i]);
        updateSel();
        inidataChanged();

        setModified(true);
        //selector->setCurrentIndex(2);

        statusBar()->showMessage(
            tr("Animation %2 split in %1 chunks -- new animation.txt file save in \"%3\"!")
            .arg(res).arg(ani.name).arg(QString::fromStdWString(std::wstring(newTxt))), 8000);
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

  if (event->mimeData()->hasUrls())
  {
    urlList = event->mimeData()->urls(); // returns list of QUrls
    // if just text was dropped, urlList is empty (size == 0)


    if ( urlList.size() > 0) // if at least one QUrl is present in list
    {
      QStringList list;
      bool areTextures = true;
      for (int i=0; i<urlList.size(); i++) {
        QFileInfo info( urlList[i].toLocalFile() );
        if (info.suffix().compare("dds",Qt::CaseInsensitive)!=0) areTextures = false;

        list.push_back( info.completeBaseName() );
      }

      if (areTextures) {
        addNewGeneral<BrfTexture>( list );
      } else {
        QFileInfo info;
        QString fn = urlList[0].toLocalFile();
        info.setFile( fn ); // information about file
        if ( info.isFile() ) loadFile( fn ); // if is file, setText
      }
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

int MainWindow::currAoBrightnessLevel() const{
  for (int i=0; i<5; i++) if (optionAoBrightness[i]->isChecked()) return i;
  return 2; // default value?
}

int MainWindow::currAoFromAboveLevel() const{
  for (int i=0; i<2; i++) if (optionAoFromAbove[i]->isChecked()) return i;
  return 1; // default value
}

bool MainWindow::currAoPerFace() const{
  for (int i=0; i<2; i++) if (optionAoPerFace[i]->isChecked()) return i;
  return 0; // default value
}

void MainWindow::optionAutoFixTextureUpdated(){
  if ( (glWidget->fixTexturesOnSight  = optionAutoFixTextureOn->isChecked()) )
    updateGl();
}

void MainWindow::optionLanguageSet0(){setLanguage(0);}
void MainWindow::optionLanguageSet1(){setLanguage(1);}
void MainWindow::optionLanguageSet2(){setLanguage(2);}
void MainWindow::optionLanguageSet3(){setLanguage(3);}
void MainWindow::optionLanguageSet4(){setLanguage(4);}

void MainWindow::optionLanguageSetCustom(){
  if (maybeSave()) {
    nextTranlationFilename = askImportFilename("QLinguist translation file (*.qm)");
    if (!nextTranlationFilename.isEmpty()) {
      glWidget->forgetChachedTextures();
      qApp->exit(101);
    }
  }
}

void MainWindow::setLanguage(int k){


  if (k!=curLanguage) {
    if (!maybeSave()) return;

    curLanguage = k;

    glWidget->forgetChachedTextures();

    // quit and restart
    qApp->exit(101);

    //QMessageBox::information(this,"OpenBrf",tr("Language changed:\nRerun OpenBrf for changes to take place"));
  }
  for (int i=0; i<4; i++) optionLanguage[i]->setChecked(i==k);
  curLanguage = k;

}

void MainWindow::saveOptions() const {

  settings->setValue("afterMeshImport",afterMeshImport());
  settings->setValue("assembleAniMode",assembleAniMode());
  settings->setValue("lastSearchString",lastSearchString);
  //settings->setValue("autoFixDXT1",(int)(glWidget->fixTexturesOnSight));
  settings->setValue("autoZoom",(int)(glWidget->commonBBox));
  settings->setValue("inferMaterial",(int)(glWidget->inferMaterial));
  settings->setValue("groupMode",(int)(glWidget->getViewmodeMult() ));
  settings->setValue("curLanguage",(int)curLanguage);
  settings->setValue("aoBrightness",(int)currAoBrightnessLevel());
  settings->setValue("aoAboveLevel",(int)currAoFromAboveLevel());
  settings->setValue("aoPerFace",(int)currAoPerFace());

  settings->setValue("lod1",lodBuild[0]);
  settings->setValue("lod2",lodBuild[1]);
  settings->setValue("lod3",lodBuild[2]);
  settings->setValue("lod4",lodBuild[3]);

  settings->setValue("lod1Perc",lodPercent[0]);
  settings->setValue("lod2Perc",lodPercent[1]);
  settings->setValue("lod3Perc",lodPercent[2]);
  settings->setValue("lod4Perc",lodPercent[3]);

  settings->setValue("lodReplace",(lodReplace)?1:0);

  settings->setValue("background",background);

  settings->setValue("useOpenGl2",(int)optionUseOpenGL2->isChecked());


}

QString MainWindow::modPath() const{
  return mabPath+"/Modules/"+modName;
}

void MainWindow::setUseOpenGL2(bool b){
  if (b==glWidget->useOpenGL2) return;
  if (b && askIfUseOpenGL2(false)) {
    glWidget->setUseOpenGL2(true);
    optionUseOpenGL2->setChecked(true);
    statusBar()->showMessage("OpenGL2.0 activated",8000);
  } else {
    glWidget->setUseOpenGL2(false);
    optionUseOpenGL2->setChecked(false);
    guiPanel->ui->cbNormalmap->setChecked(false);
    guiPanel->ui->cbSpecularmap->setChecked(false);
  }
}


void MainWindow::setNormalmap(int k){
  if (!glWidget) return;
  if (k) {
    // setting normalmaps... need to enable opengl2.0
    if (glWidget->useOpenGL2 || askIfUseOpenGL2(true)) {
      glWidget->setUseOpenGL2(true);
      optionUseOpenGL2->setChecked(true);
      glWidget->setNormalmap(true);
    } else {
      guiPanel->ui->cbNormalmap->setChecked(false);
    }
  } else {
    glWidget->setNormalmap(false);
  }
}

void MainWindow::setSpecularmap(int k){
  if (!glWidget) return;
  if (k) {
    // setting normalmaps... need to enable opengl2.0
    if (glWidget->useOpenGL2 || askIfUseOpenGL2(true)) {
      glWidget->setUseOpenGL2(true);
      optionUseOpenGL2->setChecked(true);
      glWidget->setSpecularmap(true);
    } else {
      guiPanel->ui->cbSpecularmap->setChecked(false);
    }
  } else {
    glWidget->setSpecularmap(false);
  }
}

void MainWindow::setUseAlphaCommands(bool mode){
  useAlphaCommands = mode;
  optionAoInAlpha->setVisible(mode);
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
  QVariant s =settings->value("groupMode");
  if (s.isValid()) k = s.toInt();
  glWidget->setViewmodeMult(k);
  comboViewmodeBG->button(0)->setChecked(k==0);
  comboViewmodeBG->button(1)->setChecked(k==1);
  }

  {
  int k=1;
  QVariant s =settings->value("assembleAniMode");
  if (s.isValid()) k = s.toInt();
  optionAssembleAniMatchVert->setChecked(k==0);
  optionAssembleAniMatchTc->setChecked(k==1);
  optionAssembleAniQuiverMode->setChecked(k==2);
  }

  {
  QVariant s =settings->value("curLanguage");
  if (s.isValid()) curLanguage = s.toInt(); else curLanguage = 0;
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

  /* {
  int k=1;
  QVariant s =settings->value("inferMaterial");
  if (s.isValid()) k = s.toInt();
  optionInferMaterialOff->setChecked(k==0);
  optionInferMaterialOn->setChecked(k==1);
  glWidget->inferMaterial = k;
  }*/

  {
  int k=2;
  QVariant s =settings->value("aoBrightness");
  if (s.isValid()) k = s.toInt();
  for (int h=0; h<5; h++)
  optionAoBrightness[h]->setChecked(h==k);
  }

  {
  int k=0;
  QVariant s =settings->value("useOpenGl2");
  if (s.isValid()) k = s.toInt();
  glWidget->setUseOpenGL2(k);
  //optionUseOpenGL2->blockSignals(true);
  optionUseOpenGL2->setChecked(k);
  //optionUseOpenGL2->blockSignals(false);
  this->guiPanel->ui->cbNormalmap->setChecked(k);
  this->guiPanel->ui->cbSpecularmap->setChecked(k);
  glWidget->setSpecularmap(k);
  glWidget->setNormalmap(k);

  }

  {
  int k=1;
  QVariant s =settings->value("aoAboveLevel");
  if (s.isValid()) k = s.toInt();
  for (int h=0; h<2; h++)
  optionAoFromAbove[h]->setChecked(h==k);
  }

  {
  int k=1;
  QVariant s =settings->value("aoPerFace");
  if (s.isValid()) k = s.toInt();
  for (int h=0; h<2; h++)
  optionAoPerFace[h]->setChecked(h==k);
  }

  {
  int k=0;
  QVariant s =settings->value("aoInAlpha");
  if (s.isValid()) k = s.toInt();
  optionAoInAlpha->setChecked(k);
  }

  for (int i=0,defaultVal=5000; i<4; i++,defaultVal/=2){
    bool k=true;
    QVariant s =settings->value(QString("lod%1").arg(i+1));
    if (s.isValid()) k = s.toBool();
    lodBuild[i]=k;
    float f=defaultVal/100.0;
    QVariant s2 =settings->value(QString("lod%1Perc").arg(i+1));
    if (s2.isValid()) f = s2.toDouble();
    lodPercent[i]=f;
  }
  {
  int k=1;
  QVariant s =settings->value("lodReplace");
  if (s.isValid()) k = s.toInt();
  lodReplace = k;
  }

  {
    QColor k(128,128,128);
    QVariant s =settings->value("background");
    if (s.isValid()) k = s.value<QColor>();
    background = k;
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
  QString fn = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("reference.brf");
  //QMessageBox::information(this, "OpenBRF",QString("Saving ref: %1").arg(fn));
  if (!reference.Save(fn.toStdWString().c_str()))
  {
    QMessageBox::information(this, "OpenBRF",
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
    findCurFileInIni();

    updateSel();

    return true;
  } else {
    if (!maybeSave()) return false;
    selector->setIniData(NULL,-1);
    curFileBackup = curFile;
    brfdataBackup = brfdata;
    curFile = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("reference.brf").toAscii().data();
    brfdata = reference;
    updateSel();
    setEditingRef(true);
    statusBar()->showMessage(tr("Editing reference file..."), 2000);
    updateTitle();
    return true;
  }
}


void MainWindow::inidataChanged(){
  selector->iniDataWaitsSaving = true;
  selector->setIniData(NULL,curFileIndex);
}

bool MainWindow::loadFile(const QString &_fileName)
{
  QString fileName = _fileName;
  fileName.replace("\\","/");
  //QMessageBox::information(this, "OpenBRF",tr("Loading %1.").arg(_fileName));

  if (!maybeSave()) return false;
  setEditingRef(false);

  if (!brfdata.Load(fileName.toStdWString().c_str())) {


    QMessageBox::information(this, "OpenBRF",
                              tr("Cannot load %1.").arg(fileName));

    return false;

  } else  {
    //tryLoadMaterials();
    setCurrentFile(fileName);
    updateSel();

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

  //setCurrentFile(fileName);
  if (curFileIndex>=0 && curFileIndex<(int)inidata.file.size()){
    if (inidata.origin[curFileIndex]!=IniData::MODULE_RES)
    if (QMessageBox::warning(
      this,"OpenBRF",tr("You are saving a CommonRes file!\n(i.e. not one specific of this module).\n\nAre you sure?"),
      QMessageBox::Ok, QMessageBox::No
    )== QMessageBox::No) return false;
  }
  if (brfdata.HasAnyTangentDirs() && brfdata.version==0) {
    QMessageBox::warning(
      this,"OpenBRF - Warning",tr("You are trying to save meshes with tangend directions in M&B 1.011 file format.\nUnfortunately, tangent directions can only be saved in Warband file format.\nTangent directions will not be saved..."),
      QMessageBox::Ok
    );
  }
  if (!brfdata.Save(fileName.toStdWString().c_str())) {
     QMessageBox::information(this, "OpenBRF",
                                 tr("Cannot write file %1.").arg(fileName));
     return false;
   } else {
     statusBar()->showMessage(tr("File saved!"), 2000);
     if (curFileIndex>=0 && curFileIndex<(int)inidata.file.size()){
       inidata.file[curFileIndex]=brfdata; // update ini file
       inidata.updateBeacuseBrfDataSaved();
       selector->iniDataWaitsSaving = false;
       selector->setIniData(&inidata,curFileIndex);
       updateSel();
     }
     setModified(false);
     return true;
   }
}

void MainWindow::goUsedBy(){
  int k=-1;
  QVariant prop;
  QObject *p = sender();
  if (p) {
    prop = p->property("id");
    if (prop.isValid()) k = prop.toInt();
    else statusBar()->showMessage("Debug: Prop non valid!");
  } else statusBar()->showMessage("Debug: Sender not valid!");
  if (k>=0) {
    if (curFileIndex<(int)inidata.file.size()) {
      ObjCoord c(curFileIndex,selector->firstSelected(), selector->currentTabName());
      const std::vector<ObjCoord> &v(
        inidata.usedBy(c)
        //inidata.file[c.fi].GetUsedBy(c.oi, c.t )
      );
    if (k<(int)v.size()) goTo(v[k]);
    else statusBar()->showMessage(QString("Debug: got %1<%2 (%3)!").arg(k).arg(v.size()).arg(inidata.nameShort( c )));
    }
  }

}

void MainWindow::selectBrfData(){
  if (!maybeSave()) return;
  loadIni(1);

  while (1){
    AskSelectBRFDialog d(this);
    connect(d.openModuleIniButton(),SIGNAL(clicked()),this,SLOT(openModuleIniFile()));
    for (unsigned int k = 0; k<inidata.file.size(); k++){
      int h = inidata.origin[k]==IniData::MODULE_RES?0:2;
      QString shortName = inidata.filename[k];
      shortName = QString("%1. %2").arg(inidata.iniLine[k]).arg(QFileInfo(shortName).baseName());
      if (inidata.updated>2) {
        int s = inidata.totSize(k);
        if (inidata.updated<4) {
          shortName.append(QString(" (%1)").arg(s));
        } else {
          int u = inidata.totUsed(k);
          shortName.append(QString(" (%2+%1)").arg(s-u).arg(u));
        }
      }
      d.addName(h,shortName,inidata.filename[k]);

    }
    QDir dir(inidata.modPath);
    dir.cd("Resource");

    QStringList filters;
    filters << "*.brf" ;
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    QStringList list =  dir.entryList();
    for (int i=0; i<list.size(); i++){
      QString name = QFileInfo(list[i]).baseName();
      QString fullname = dir.absoluteFilePath(list[i]);
      if (inidata.findFile(fullname)==-1)
      d.addName(1, name ,fullname);
    }

    //for (int dir.count()
    d.doExec();
    if (d.loadMe=="???1") { loadIni(4); continue; }
    if (d.loadMe=="???2") { refreshIni(); continue; }

    if (!d.loadMe.isEmpty()){
      loadFile(d.loadMe);
    }
    break;
  }
}

void MainWindow::showUnrefTextures(){
  loadIni(4);
  while(1){
  AskUnrefTextureDialog d(this);

  QDir dir(inidata.modPath);
  dir.cd("Textures");

  QStringList filters;
  filters << "*.dds" << "*.TIF" ;
  dir.setNameFilters(filters);
  dir.setFilter(QDir::Files);
  QStringList list =  dir.entryList();
  for (int i=0; i<list.size(); i++){
    QString name = QFileInfo(list[i]).fileName();
    if (!(inidata.findTexture(name)))
    d.addFile(name);
  }

  if (d.exec()!=QDialog::Accepted) break;
  }

}


void MainWindow::showModuleStats(){
  loadIni(4);
  QMessageBox::information(this,"OpenBRF",inidata.stats());
}

void MainWindow::computeUsedBy(){
  loadIni(4);
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
  QString f0=tr("M&B Resource (*.brf)"),f1 = tr("WarBand Resource v.1 (*.brf)");
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
  QString notInIni = (curFileIndex==-1)?tr(" [not in module.ini]"):tr("");
  QString tit("OpenBrf");
  if (!editingRef) {
    if (curFile.isEmpty())
      setWindowTitle(tr("%1%2").arg(tit).arg(maybestar));
    else
      setWindowTitle(tr("%1 - %2%3%4").arg(tit).arg(curFile).arg(maybestar).arg(notInIni));
  } else
    setWindowTitle(tr("%1 - editing internal reference data%2").arg(tit).arg(maybestar));
}

void MainWindow::setModified(bool mod, bool repeatable){
  isModified=mod;
	if (mod && repeatable) setNextActionAsRepeatable = true; // action becomes repetable
  updateTitle();
}

bool MainWindow::guessPaths(QString fn){
  bool res=false;
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
    usingWarband = (a.exists("mb_warband.exe")||(!a.exists("mount&blade.exe"))) ;
    res=true;
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
    res=true;
  }

  updatePaths();
  inidata.setPath(mabPath,_modPath);

  loadIni(2);
  return res;
}

bool MainWindow::loadIni(int lvl){
  
  QTime qtime;
  qtime.start();

  bool res = inidata.loadAll(lvl); // if lvl == 2 only tex mat etc

  //cancelNavStack();

  statusBar()->showMessage( tr("%5 %1 brf files from module.ini of \"%3\"-- %2 msec total [%4 text/mat/shad]").
      arg(inidata.file.size()).arg(qtime.elapsed()).arg(modName).arg(inidata.nRefObjects())
      .arg((res)?tr("scanned"):tr("ERRORS found while scanning")),6000);

  guiPanel->setIniData(inidata);
  findCurFileInIni();

  if (lvl==4) {
    if (!res) QMessageBox::warning(this,"OpenBRF",inidata.errorStringOnScan);
    updateSel();
  }


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
  guessPaths(QFileInfo(fileName).canonicalPath() );

  findCurFileInIni();
}

void MainWindow::findCurFileInIni()
{
  curFileIndex = inidata.findFile(curFile);
  if (curFileIndex>=0) {
    selector->setIniData(&inidata,curFileIndex);
  } else {
    selector->setIniData(NULL,-1);
  }

  if (curFileIndex == -1) selector->setIniData(NULL,-1);
  else selector->setIniData(&inidata,curFileIndex);

  updateTitle();

}
MainWindow::~MainWindow()
{
  saveOptions();
    //delete ui;
}

bool MainWindow::goTo(ObjCoord o){
  if (o.fi!=curFileIndex )
  if (!loadFile( inidata.filename[o.fi]) ) return false;

  curFileIndex = o.fi;

  selectOne(o.t,o.oi);
  return true;
}

bool MainWindow::navigateLeft(){

  int stackPos=0;
  int currTab = selector->currentTabName();
  if (currTab==MATERIAL) stackPos = 1;
  if (currTab==SHADER) stackPos = 2;
  if (currTab==TEXTURE) stackPos = 2;
  if (!stackPos) return false;

  QPair<ObjCoord,QString> p = navigationStack[stackPos-1];

  if (p.second.isEmpty()) {
    return false;
  }
  qDebug("-1");

  if (p.first.fi==-1) {
    if (!loadFile( p.second )) return false;
    curFileIndex = p.first.fi;
    selectOne(p.first.t,p.first.oi);
  } else {
    if (!goTo(p.first)) return false;
  }


  guiPanel->setNavigationStackDepth( stackPos-1 );

  return true;
}



bool MainWindow::navigateRight(){

  int currTab = selector->currentTabName();

  if ((currTab!=MESH) && (currTab!=MATERIAL) && (currTab!=SHADER)) {
    return false;
  }

  inidata.loadAll(2); // ini must be loaded, at least mat and textures

  QPair<ObjCoord,QString> old(
      ObjCoord(curFileIndex,selector->firstSelected(), TokenEnum(selector->currentTabName())),
      curFile);

  int nextTab = MATERIAL;
  QString nextName;
  int stackPos = -1;

  if (currTab==MESH) {

    if (!guiPanel->ui->boxMaterial->hasFrame()) return false;
    stackPos = 0;
    nextTab = MATERIAL;
    nextName = guiPanel->ui->boxMaterial->text();
  }
  if (currTab==MATERIAL) {
    QLineEdit *le = guiPanel->materialLeFocus();
    if (!le) return false;
    if (!le->hasFrame()) return false;
    nextName = le->text();
    if (nextName==QString("none")) return false;


    if (guiPanel->curMaterialFocus==GuiPanel::SHADERNAME){
      nextTab = SHADER;
    } else {
      nextTab = TEXTURE;
    }
    stackPos = 1;

  }

  if (currTab==SHADER) {
    nextName = guiPanel->ui->leShaderFallback->text();
    ObjCoord p = inidata.indexOf(nextName,SHADER);
    if (!p.isValid()) return false;
    if (!goTo(p)) return false;
    selector->currentWidget()->setFocus();
    return true;
  }

  ObjCoord p = inidata.indexOf(nextName,nextTab);
  if (p.fi==-1) {
    statusBar()->showMessage( tr("Navigate: cannot find \"%1\" in current module").arg(nextName),6000);
    return false;
  }

  if (!goTo(p)) return false;
  navigationStack[stackPos]=old;
  guiPanel->setNavigationStackDepth( stackPos+1 );

  selector->currentWidget()->setFocus();

  return true;

}

void MainWindow::cancelNavStack(){
  //navigationStackPos = 0;
  guiPanel->setNavigationStackDepth( 0 );
  for (int i=0; i<2; i++) navigationStack[i]=
      QPair<ObjCoord, QString>(ObjCoord::Invalid(),"");
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
  int tmp = inidata.updated;
  inidata.updated=0; // force reload all
  brfdata.ForgetTextureLocations();
  glWidget->forgetChachedTextures();

  bool res = loadIni(tmp);
  updateGl();
  updateGui();
  return res;

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

  int oldIni = inidata.updated;
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
  } else {
    if (inidata.updated!=oldIni) updateSel(); // refresh colors
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
    loadIni(2);
    brfdata.Clear();
    curFile.clear();
    curFileIndex = -1;
    updateGui();
    updateGl();
    updateSel();
    updateTitle();
    inidataChanged();
    setEditingRef(false);
  }
}


bool readFile(QIODevice &device, QSettings::SettingsMap &map){
  return true;
}
bool writeFile(QIODevice &device, const QSettings::SettingsMap &map){
  return true;
}

void MainWindow::registerExtension(){
  QString exeFile = QCoreApplication::applicationFilePath();
  exeFile.replace('/',QString("\\\\"));

  //QSettings settings(QSettings::NativeFormat, QSettings::SystemScope,"HKEY_CLASSES_ROOT");
  {
  //QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
  //QSettings settings("HKEY_LOCAL_MACHINE", QSettings::NativeFormat);
  //settings.beginGroup("SOFTWARE");
  //settings.beginGroup("Classes");


  //settings.beginGroup(".brf");
  QSettings settings(QSettings::NativeFormat,QSettings::SystemScope, "classes", ".brf");
    settings.setValue("","brf.resourceT");
  //settings.endGroup();
  }
  //QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
  QSettings settings(QSettings::NativeFormat,QSettings::SystemScope,"classes", "brf.resource");

  //settings.beginGroup("brf.resource");
    //settings.setValue("","Mount&Blade Binary Resource File");
    settings.setValue("","");
    settings.setValue("FriendlyTypeName","Mount&Blade Binary Resource File");
    settings.setValue("PerceivedType","Application");

    settings.beginGroup("DafualtIcon");
    settings.setValue("",QString("%1%2 test").arg(exeFile).arg(",0") );
    settings.endGroup();

    settings.beginGroup("shell");
      //settings.setValue("","");
      settings.beginGroup("open");
        //settings.setValue("","");
        settings.beginGroup("command");
          settings.setValue("",
             QString("\"%1\" \"%2\"").
             arg(exeFile).arg("%1") );
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

void MainWindow::optionLodSettings(){
  AskLodOptionsDialog *d = new AskLodOptionsDialog(this);
  d->setData(lodPercent, lodBuild, lodReplace);
  if (d->exec()==QDialog::Accepted){
    d->getData(lodPercent, lodBuild, &lodReplace);
    statusBar()->showMessage(tr("New Lod parameters set"));
  }
  statusBar()->showMessage(tr("Cancelled"));
}

void MainWindow::optionSetBgColor(){
  QColor color = QColorDialog::getColor(QColor(128,128,128,255), this);
  if (!color.isValid()) return;
  else {
    background = color;
    glWidget->setDefaultBgColor(background,!editingRef);
  }

}

    
#include "askFlagsDialog.h"

#define TR AskFlagsDialog::tr
void MainWindow::setFlagsBody(){
  unsigned int curfOR=0, curfAND = 0xFFFFFFFF;


  QString FlagNameArray[64] = {
     TR("Two-sided"),"",
     TR("No Collision"),"",
     TR("No Shadow"),TR("Game won't use this collision object"),
     "","",
     "","",
     "","",
     "","",
     "","",

     "","",
     "","",
     "","",
     "","",
     "","",
     "","",
     "","",
     "","",

     TR("Difficult"),"",
     TR("Unwalkable"),"",
    "","",
    "","",
    "","",
    "","",
    "","",
    "","",

    "","",
    "","",
    "","",
    "","",
    "","",
    "","",
    "","",
    "","",

  };

  QModelIndexList list=selector->selectedList();

  int sel = selector->firstSelected();
  if (sel<0 || sel>=(int)brfdata.body.size()) return;

  int selp = guiPanel->getCurrentSubpieceIndex(BODY);
  if (selp<0 || selp>(int)brfdata.body[sel].part.size()) return;
  BrfBodyPart &p(brfdata.body[sel].part[selp]);

  if (p.type==BrfBodyPart::MANIFOLD) return;
  curfOR = curfAND = p.flags;

  //setFlags(curf,ui->leMatFlags->text());
  QStringList l, tips ;
  for (int i=0; i<32; i++) {
    l.append(FlagNameArray[i*2]);
    tips.append(FlagNameArray[i*2+1]);
  }

  AskFlagsDialog *d = new AskFlagsDialog(this,curfOR,curfAND, l,tips);
  d->setWindowTitle(tr("Collision objects flags"));
  if (d->exec()==QDialog::Accepted) {

    uint flags = d->toOne();

    if (p.flags != flags) {

      p.flags = flags;
      setModified(true);
      guiPanel->updateBodyPartData();
    }
    //guiPanel->update(); //setSelection(list,MATERIAL);
  }

}
void MainWindow::setFlagsMesh(){
  unsigned int curfOR=0, curfAND = 0xFFFFFFFF;


  QString FlagNameArray[64] = {
    TR("Unknown (for props?)"),TR("Exact meaning of this flag is unknown."),
    TR("Unknown (for particles?)"),TR("Exact meaning of this flag is unknown."),
    TR("Unknown (plants?)"),TR("Exact meaning of this flag is unknown."),
    TR("Unknown (for particles?)"),TR("Exact meaning of this flag is unknown."),
    "","",
    TR("Unknown (hairs and body parts?)"),TR("Exact meaning of this flag is unknown."),
    "","",
    "","",

    TR("Unknown (for particles?)"),TR("Exact meaning of this flag is unknown."),
    TR("Unknown (screen space?)"),TR("Exact meaning of this flag is unknown."),
    "","",
    "","",
    "","",
    "","",
    "","",
    "","",

    TR("R: (tangent space)"),TR("This flag is automatically set if mesh has tangent directions defined."),
    TR("R: (Warband format)"),TR("This flag is automatically set for meshes in WB formats"),
    "","",
    "","",
    "","",
    "","",
    "","",
    "","",

    TR("Pre-exponentiate colors"),TR("Vertex colors will be pre-exponentiated (for gamma corrections) if this flag is set."),
    "","",
    TR("Unknown (for particles?)"),TR("Exact meaning of this flag is unknown."),
    "","",
    "","",
    "","",
    "","",
    "","",

  };

  QModelIndexList list=selector->selectedList();

  for (int i=0; i<(int)list.size(); i++) {
    int sel = list[i].row();
    if (sel<0 || sel>=(int)brfdata.mesh.size()) continue;

    BrfMesh &m(brfdata.mesh[sel]);

    curfOR |= m.flags;
    curfAND &= m.flags;

  }

  //setFlags(curf,ui->leMatFlags->text());
  QStringList l, tips ;
  for (int i=0; i<32; i++) {
    l.append(FlagNameArray[i*2]);
    tips.append(FlagNameArray[i*2+1]);
  }

  AskFlagsDialog *d = new AskFlagsDialog(this,curfOR,curfAND, l,tips);
  d->setWindowTitle(tr("Mesh flags"));
  if (d->exec()==QDialog::Accepted) {
    //ui->leMatFlags->setText(StringH( d->getRes() ));
    //emit(dataMaterialChanged());

    for (int i=0; i<(int)list.size(); i++) {
      int sel = list[i].row();
      if (sel<0 || sel>=(int)brfdata.mesh.size()) continue;

      BrfMesh &m(brfdata.mesh[sel]);

      m.flags |= d->toOne();
      m.flags &= d->toZero();

    }
    guiPanel->setSelection(list,MESH);
    setModified(true);
  }

}

void MainWindow::setFlagsTexture(){
  unsigned int curfOR=0, curfAND = 0xFFFFFFFF;


  QString FlagNameArray[64] = {
    TR("Unknown"),"",
    TR("Force hi-res"),TR("By default, depending on the game settings, higher-res mipmap levels might by not loaded"),
    TR("Unknown"),"",
    TR("Languange dependent"),TR("If set, depending on the game language settings, this texture is substituted by the one found in the language folder (WB only)"),
    TR("HDR only"),TR("If High-Dynamic-Ramge is off, this texture won't be loaded"),
    TR("No HDR"),TR("If High-Dynamic-Ramge is on, this texture won't be loaded"),
    "","",
    TR("Unknown"),"",

    "","",
    "","",
    "","",
    "","",
    "B:","", // maybe mipmaps X
    "B:","",
    "B:","",
    "B:","",

    "B:","", // maybe mipmaps Y
    "B:","",
    "B:","",
    "B:","",
    TR("Clamp U"),TR("By default, texture U is set to wrap (horizontally tiled texture)"),
    TR("Clamp V"),TR("By default, texture V is set to wrap (vertically tiled texture)"),
    "","",
    "","",

    "B:","", // animation frames
    "B:","",
    "B:","",
    "B:","",
    "","",
    "","",
    "","",
    "","",

  };

  QModelIndexList list=selector->selectedList();

  for (int i=0; i<(int)list.size(); i++) {
    int sel = list[i].row();
    if (sel<0 || sel>=(int)brfdata.texture.size()) continue;

    BrfTexture &t(brfdata.texture[sel]);
    curfOR |= t.flags;
    curfAND &= t.flags;
  }

  //setFlags(curf,ui->leMatFlags->text());
  QStringList l, tips ;
  for (int i=0; i<32; i++) {
    l.append(FlagNameArray[i*2]);
    tips.append(FlagNameArray[i*2+1]);
  }

  AskFlagsDialog *d = new AskFlagsDialog(this,curfOR,curfAND, l,tips);
  int aniVals[16] = {0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60};
  d->setBitCombo(TR("Animation frames"),TR("N. of frames of texture anim (append \"_0\", \"_1\" ... to dds file names)."),24,28,aniVals);
  const char* aniPows[16] = {"default","2","4","8","16","32","64","128","256","512","1024","2K", "4K","8K","16K","32K",};
  d->setBitCombo(TR("Size U (?)"),TR("Unclear meaining, usually only set for face textures"),12,16,aniPows);
  d->setBitCombo(TR("Size V (?)"),TR("Unclear meaining, usually only set for face textures"),16,20,aniPows);

  d->setWindowTitle(tr("Texture flags"));
  if (d->exec()==QDialog::Accepted) {
    //ui->leMatFlags->setText(StringH( d->getRes() ));
    //emit(dataMaterialChanged());

    for (int i=0; i<(int)list.size(); i++) {
      int sel = list[i].row();
      if (sel<0 || sel>=(int)brfdata.texture.size()) continue;

      BrfTexture &t(brfdata.texture[sel]);

      t.flags |= d->toOne();
      t.flags &= d->toZero();

    }
    guiPanel->setSelection(list,TEXTURE);
    setModified(true);
  }

}


void MainWindow::setFlagsMaterial(){
  unsigned int curfOR=0, curfAND = 0xFFFFFFFF;


  QString FlagNameArray[64] = {
     TR("No fog"),TR("This object must not be affected by fog"),
     TR("No Lighting"),TR("This object won't be dynamically relit"),
    "","",
     TR("No Z-write"),TR("Rendering object leaves the depth buffer unaffected"),
     TR("No depth Test"),TR("Object ignores the depth test: i.e. it will be always drawn over others."), //
     TR("Specular enable"),TR("Specular reflections are enabled."),
     TR("Unknown (for alpha test?)"),TR("Exact meaning of this flag is unknown."),
     TR("Uniform lighting"),"",


     TR("Blend"),TR("Enable alpha-blending (for semi-transparencty)"),
     TR("Blend add"),TR("Alpha-blend function: add"),
     TR("Blend multiply"),TR("Alpha-blend function: mulitply"),
     TR("Blend factor"),TR("Alpha-blend function: factor"),
     "B:","", // alpha test value
     "B:","", // alpha test value
     TR("Unknown (for alpha test?)"),TR("Exact meaning of this flag is unknown."),
    "","",

     TR("Render 1st"),"",
     TR("Origin at camera"),"",
     TR("LoD"),TR("If set, this material is optimized for LODs>1"),
    "","",
    "","",
    "","",
    "","",
    "","",

    "B:","", // render order
    "B:","",
    "B:","",
    "B:","",

    TR("Invert bumpmap"),TR("If set, bumpmap should be considered inverted on Y axis"),
    "","",
    "","",
    "","",

  };


  QModelIndexList list=selector->selectedList();

  for (int i=0; i<(int)list.size(); i++) {
    int sel = list[i].row();
    if (sel<0 || sel>=(int)brfdata.material.size()) continue;

    BrfMaterial &m(brfdata.material[sel]);

    curfOR |= m.flags;
    curfAND &= m.flags;

  }

  //setFlags(curf,ui->leMatFlags->text());
  QStringList l, tips ;
  for (int i=0; i<32; i++) {
    l.append(FlagNameArray[i*2]);
    tips.append(FlagNameArray[i*2+1]);
  }

  AskFlagsDialog *d = new AskFlagsDialog(this,curfOR,curfAND, l,tips);
  int vals[16] = {-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7};
  d->setBitCombo(TR("Render order"),TR("Determines what is rendered first (neg number), or later (pos numbers)"),24,28, vals,8);

  const char* valsAlpha[4] = {"No","< 8/256","< 136/256","< 251/256"};
  d->setBitCombo(TR("Alpha test:"),TR("Alpha testing (for cutouts). Pixels more transparent than a given number will be not drawn."),12,14, valsAlpha);

  d->setWindowTitle(tr("Material flags"));
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
