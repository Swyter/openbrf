#include <qfiledialog.h>
#include <qmessagebox.h>
#include <algorithm>

#include "brfData.h"
#include "selector.h"
#include "mainwindow.h"
#include "vcgmesh.h"

#include "ioSMD.h"
#include "ioMD3.h"
#include "ioMB.h"
#include "ioOBJ.h"

void MainWindow::moduleSelect(){

  QString oldDir = modPath();
  QString dir = QFileDialog::getExistingDirectory(
    this,tr("Select Module folder"),oldDir,
    QFileDialog::ShowDirsOnly //| QFileDialog::DontResolveSymlink
  );
  if (!dir.isEmpty())
  if (!guessPaths(dir+"/Resource")) {
    QMessageBox::warning(this,"OpenBRF",tr("Not a recognized module folder"));
  }
}



void MainWindow::meshComputeLod(){
  if (selector->currentTabName()!=MESH) return;

  std::vector<int> sel;
  for (int k=0; k<selector->selectedList().size(); k++) {
    sel.push_back(selector->selectedList()[k].row());
  }
  std::sort(sel.begin(), sel.end());


  for (int k=selector->selectedList().size()-1; k>=0; k--) {
    std::vector<BrfMesh> resvec;
    int i=sel[k];
    if (i<0) continue;
    if (i>(int)brfdata.mesh.size()) return;

    BrfMesh &m(brfdata.mesh[i]);
    //m.UnifyPos();
    //m.UnifyVert(false,0);

    std::vector<BrfMesh> mvec; mvec.push_back(m);
    //float  amount = 1;

    // find correct name
    QString st(m.name);
    QString partname = st;
    QString partnumber = st;
    int k = st.lastIndexOf('.');
    if (k!=-1) {
      partname.truncate(k);
      partnumber = st.right(st.length()-k-1);
      bool ok;
      partnumber.toInt(&ok);
      if (!ok) k=-1;
      partnumber = QString(".")+partnumber;
    }
    if (k==-1) {
      partname = st;
      partnumber.clear();
    }



    for (int lod=1; lod<=N_LODS; lod++) if (lodBuild[lod-1]){
      VcgMesh::add( m , 0 );
      float amount =lodPercent[lod-1];
      VcgMesh::simplify((int)amount);

      BrfMesh res = VcgMesh::toBrfMesh();
      if (m.isRigged) res.TransferRigging(mvec,0,0); else res.rigging.clear();
      res.isRigged = m.isRigged;
      res.flags = m.flags;
      sprintf(res.material,"%s", m.material);
      res.hasVertexColor = m.hasVertexColor;
      sprintf(res.name,"%s.lod%d%s",partname.toAscii().data(),lod,partnumber.toAscii().data());
      res.UnifyPos();
      res.UnifyVert(false,0);
      res.ComputeNormals();
      res.ComputeTangents();
      resvec.push_back(res);
    }
    for (uint ii=0; ii<resvec.size(); ii++){
      brfdata.mesh.insert(brfdata.mesh.begin()+i+ii+1,resvec[ii]);
    }
    inidataChanged();
    setModified(true);
    updateSel();
    selector->selectOne(MESH, i);
  }

  //return true;
}



bool MainWindow::exportMeshGroup(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.mesh.size()) return false;
  QString fn = askExportFilename(brfdata.mesh[ i ].name,"Wavefront Object Files (*.obj)");
  if (fn.isEmpty()) return false;
  QFile f;
  IoOBJ::reset();
  if (!IoOBJ::open(f,fn)) {

    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot open file for writing;")
    );
    return false;
  }
  IoOBJ::reset();
  for (int k=0; k<selector->selectedList().size(); k++){
    const BrfMesh &m(brfdata.mesh[ selector->selectedList()[k].row() ]);
    IoOBJ::writeMesh(f,m,0);
  }
  f.close();
  return true;
}

bool MainWindow::exportMeshGroupManyFiles(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.mesh.size()) return false;

  QString path = settings->value("LastExpImpPath").toString();
  if (path.isEmpty()) path = QDir::currentPath();

  QString dir = QFileDialog::getExistingDirectory ( this, tr("Select a folder to export all meshes"),path,QFileDialog::ShowDirsOnly);

  if (dir.isEmpty()) return false;

  for (int k=0; k<selector->selectedList().size(); k++){
    QFile f;
    const BrfMesh &m(brfdata.mesh[ selector->selectedList()[k].row() ]);
    QString fn = QString("%1/%2.obj").arg(dir,m.name);
    if (!IoOBJ::open(f,fn)) {
      QMessageBox::information(this,
        tr("Open Brf"),
        tr("Cannot open file %1 for writing;").arg(fn)
      );
      return false;
    }
    IoOBJ::reset();
    IoOBJ::writeMesh(f,m,0);
    f.close();

  }
  return true;
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

  QString fn = askExportFilename(brfdata.mesh[ i ].name,
    "Studiomdl Data rigged mesh (*.SMD);;"
    "Maya Ascii File [experimental] (*.ma)");
  if (fn.isEmpty()) return false;

  int res;
  char *errorSt;
  if (fn.endsWith(".ma",Qt::CaseInsensitive)) {
    res = IoMB::Export(fn.toStdWString().c_str(), m, s, currentDisplayFrame() );
    errorSt = IoMB::LastErrorString();
  } else {
    res = ioSMD::Export(fn.toStdWString().c_str(), m, s, currentDisplayFrame() );
    errorSt = ioSMD::LastErrorString();

  }

  if (res) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot export rigged mesh:\n %1\n").arg( errorSt  )
    );
    return false;
  }
  return true;
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
    "Studiomdl Data skinned rest-pose (*.SMD);;"
    "Maya skinned rest-pose (*.MA)"
  );
  if (fn.isEmpty()) return false;

  int res;

  if (fn.endsWith(".ma",Qt::CaseInsensitive)) {
    res = IoMB::Export(fn.toStdWString().c_str(),m,s,0);
  } else {
    res = ioSMD::Export(fn.toStdWString().c_str(), m, s, 0);
  }
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

  int res = ioSMD::Export(fn.toStdWString().c_str(), a, s);
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

  QString fn = askExportFilename(
    brfdata.skeleton[ i ].name,
    "Studiomdl Data Skeleton (*.SMD);;"
    "Maya ascii file (*.MA)"
  );
  if (fn.isEmpty()) return false;

  //BrfData tmp; tmp.mesh.push_back(m); tmp.Save((fn+"_tmp.brf").toAscii().data());

  int res;
  if (fn.endsWith(".smd",Qt::CaseInsensitive)) {
    res = ioSMD::Export(fn.toStdWString().c_str(), m, s, 0);
  } else {
    res = IoMB::Export(fn.toStdWString().c_str(), m, s, 0);
  }
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
      "*.obj "
      "*.ply "
      "*.off "
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

  QString name=QFileInfo(fn).completeBaseName();
  name.truncate(254);
  sprintf( s.name, "%s", name.toAscii().data());
  insert(s);
  setModified(true);
  return true;
}



bool MainWindow::exportCollisionBody(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.body.size()) return false;
  if (selector->currentTabName()!=BODY) return false;
  QString fn = askExportFilename(brfdata.body[ i ].name,"Wavefront Object Files (*.obj)");
  if (fn.isEmpty()) return false;
  if (!brfdata.body[i].ExportOBJ(fn.toStdWString().c_str())){
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot write file?")
    );
    return false;
  }
  return true;
}


bool MainWindow::exportMovingMesh(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.mesh.size()) return false;
  if (selector->currentTabName()!=MESH) return false;
  QString fn = askExportFilename(brfdata.mesh[ i ].name,tr("Quake 3 vertex animation (*.MD3)"));
  if (fn.isEmpty()) return false;
  if (!IoMD3::Export(fn.toStdWString().c_str(),brfdata.mesh[i])){
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Error exporting MD3 file\n: %1").arg(QString::fromStdWString(IoMD3::LastErrorString()))
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
      if (QFileInfo(fn).suffix().toLower()== "obj") {
        return brfdata.mesh[i].SaveOBJ(
          fn.toAscii().data(),this->currentDisplayFrame()
        );
      } else {
        VcgMesh::add(brfdata.mesh[ i ], this->currentDisplayFrame() );
        return VcgMesh::save(fn.toAscii().data());
      }
    break;
    default: assert(0); // how was this signal sent?!
  }
  return false;
}

/*
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
*/

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

QStringList MainWindow::askImportFilenames(QString ext){
  QString path = settings->value("LastExpImpPath").toString();
  if (path.isEmpty()) path = QDir::currentPath();

  QStringList fileName = QFileDialog::getOpenFileNames(
    this,
    tr("Import files") ,
    path,
    ext,
    &lastImpExpFormat
   );
   if (fileName.isEmpty()) {
     statusBar()->showMessage(tr("Import canceled."), 2000);
   } else
   settings->setValue("LastExpImpPath",QFileInfo(fileName[0]).absolutePath());
   return fileName;
}


QString MainWindow::askExportFilename(QString filename){
  return askExportFilename(filename, QString(
  "Wavefront Object Files (*.obj);;" // e:C n:N
  "Polygon File Format (*.ply);;" // ok
  "Collada (*.dae);;"
  "AutoCad (*.dxf);;"  // n:C n:N
  "Object File Format (*.off);;" // e:N e:C
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




bool MainWindow::importMovingMesh(){
  QString fn = askImportFilename(QString("Quake 3 vertex animation (*.MD3)"));
  if (fn.isEmpty()) return false;

  std::vector<BrfMesh> tmp;

  bool ok=false;
  ok = IoMD3::Import(fn.toStdWString().c_str(),tmp);
  if (!ok) {
    QMessageBox::information(this, tr("Open Brf"),
     tr("Cannot import file %1:\n%3\n").arg(fn)
     .arg(QString::fromStdWString(std::wstring(IoMD3::LastErrorString())))
    );
    return false;
  }
  for (unsigned int i=0; i<tmp.size(); i++) {
    applyAfterMeshImport(tmp[i]);
    insert(tmp[i]);
  }

  selector->updateData(brfdata);
  setModified(true);
  return true;
}




bool MainWindow::importBrf(){

  //QString lastBrfFormat(tr("strange, latest, rare Warband format (*.brf)"));

  QString fn = askImportFilename(tr("Warband or M&B resource (*.brf)"));//+lastBrfFormat);
  if (fn.isEmpty()) return false;
  BrfData tmp;
  //bool useNext=(lastImpExpFormat==lastBrfFormat);

  if (!tmp.Load(fn.toStdWString().c_str(),0)) {
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

bool MainWindow::importCollisionBody(){
  QStringList fn = askImportFilenames(
    tr("mesh file (*.obj)")
  );
  if (fn.isEmpty()) return false;

  for (int i=0; i<fn.size(); i++) {
    BrfBody b;

    if (!b.ImportOBJ(fn[i].toStdWString().c_str())) {
      QMessageBox::information(this, tr("Open Brf"),
        tr("Cannot import file %1\n").arg(fn[i])
      );
      continue;
    }

    QString name=QFileInfo(fn[i]).completeBaseName();
    name.truncate(254);

    sprintf( b.name, "%s", name.toAscii().data());

    insert( b );
    setModified(true);
  }

  return true;
}

bool MainWindow::_importStaticMesh(QString s, std::vector<BrfMesh> &mV, std::vector<bool> &wasMultipleMatV){
  QStringList fnList = askImportFilenames(
    tr("mesh file ("
      "*.obj "
      "*.ply "
      "*.off "
      "*.stl "
      "*.dae)"
    )
  );
  if (fnList.isEmpty()) return false;
  mV.resize(fnList.size());
  wasMultipleMatV.resize(fnList.size());
  bool res=false;
  for (int j=0; j<fnList.size(); j++) {
    BrfMesh& m(mV[j]);
    //bool & wasMultipleMat(wasMultipleMatV[j]);
    QString& fn(fnList[j]);

    m.name[0]=0;

    if (QFileInfo(fn).suffix().toLower()== "obj") {
      if (!m.LoadOBJ( fn.toAscii().data() )) {
        QMessageBox::information(this,
          tr("Open Brf"),
          tr("Cannot import file %1\n")
        );
        continue;
      }
      wasMultipleMatV[j] = IoOBJ::wasMultpileMat();
    } else {
      wasMultipleMatV[j] = false;
      if (!VcgMesh::load(fn.toAscii().data())) {
        QMessageBox::information(this,
          tr("Open Brf"),
          tr("Cannot import file %1\n\n"
             "(error: %2)").arg(fn).arg(VcgMesh::lastErrString()
          )
        );
        continue;
      }
      m = VcgMesh::toBrfMesh();
    }
    if (m.name[0]==0) {
      // assign name of the mesh
      QString meshname=QFileInfo(fn).completeBaseName();
      meshname.truncate(254);
      sprintf( m.name, "%s", meshname.toAscii().data());
    }
    applyAfterMeshImport(m);
    res=true;
  }
  return res;
}



bool MainWindow::importStaticMesh(){
  vector<BrfMesh> m;
  vector<bool> mult;
  if (!_importStaticMesh("Import static mesh", m, mult)) return false;

  for (int j=0; j<(int)m.size(); j++) {

    if (mult[j]) {
       int reply = QMessageBox::question(this, tr("OpenBRF"),
         tr("Mesh \"%1\" has multiple materials\\objects.\nImport a separate mesh per material\\object?").arg(m[j].name),
         QMessageBox::Yes | QMessageBox::No
       );
       mult[j] = reply == QMessageBox::Yes;
    }
    if (!mult[j]) insert( m[j] );
    else  {
      std::vector<BrfMesh> v;
      IoOBJ::subdivideLast(m[j],v);
      for (unsigned int i=0; i<v.size(); i++) {
        insert(v[i]);
      }
    }
    setModified(true);

    statusBar()->showMessage(tr("Imported mesh \"%1\"--- normals:%2 colors:%3 texture_coord:%4")
      .arg( m[j].name )
      .arg((VcgMesh::gotNormals()?"[ok]":"[recomputed]"))
      .arg((VcgMesh::gotColor()?"[ok]":"[NO]"))
      .arg((VcgMesh::gotTexture()?"[ok]":"[NO]")),5000
    );
  }
  return true;
}

bool MainWindow::importMovingMeshFrame(){
  int i = selector->firstSelected();
  int j = guiPanel->getCurrentSubpieceIndex(MESH);
  if ((selector->currentTabName()!=MESH) ||
      (i<0) ||
      (i>=(int)brfdata.mesh.size())
  )
  { QMessageBox::information(this,
      tr("Import vertex animation frame"),
      tr("Frist select a mesh\nto add a frame to.")
    );
    return false;
  }

  vector<BrfMesh> m;
  vector<bool> tmp;
  if (!_importStaticMesh("Import one or more meshes and add them as a frame", m, tmp))
  {
    statusBar()->showMessage(tr("Import failed"),5000);
    return false;
  }
  bool allOk = true;
  int N = m.size();
  for (int h=0; h<N; h++) {
    bool res=false;
    if (assembleAniMode()==0) {
      res = brfdata.mesh[i].AddFrameMatchVert(m[h],j);
      if (!res) statusBar()->showMessage(
          tr("Vertex number mismatch... using texture-coord matching instead of vertex-ordering")
          .arg(m[h].name)
          ,7000
      );
      allOk = false;
    }
    if (assembleAniMode()==2) {
      res = brfdata.mesh[i].AddFrameMatchPosOrDie(m[h],j);
    }
    if (!res) brfdata.mesh[i].AddFrameMatchTc(m[h],j);
    setModified(true);
    selector->selectOne(MESH,i);

  }
  if (N==1)
    statusBar()->showMessage(tr("Added frame %1").arg(j+1),2000);
  else
    statusBar()->showMessage(tr("Added frames %1..%2").arg(j+1).arg(j+N),2000);
  return true;
}


bool MainWindow::importAnimation(){
  QString brfFormat("Old BrfEdit style SMD (*.SMD)");
  QString fn = askImportFilename(QString("Studiomdl Data (*.SMD);;")+brfFormat);
  if (fn.isEmpty()) return false;
  BrfSkeleton s;
  BrfAnimation a;

  bool backComp=(lastImpExpFormat==brfFormat);
  int res = ioSMD::Import(fn.toStdWString().c_str(), a, s);
  if (res!=0) {
     QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import animation:\n %1\n").arg( ioSMD::LastErrorString() )
    );
    return false;
  }


  QString name=QFileInfo(fn).completeBaseName();
  name.truncate(254);
  sprintf( a.name, "%s", name.toAscii().data());


  if (backComp) {
    //int j = gimmeASkeleton( s.bone.size() );
    //if (j>=0)
    //  a.Reskeletonize( s, reference.skeleton[j] );
    if (a.frame.size()) a.frame.pop_back(); // remove last frame
  }
  if (a.AutoAssingTimesIfZero()){
    statusBar()->showMessage(tr("Found no time value in SMD file, so I added them."),5000);
  }
  insert(a);

  setModified(true);

  return true;

}



bool MainWindow::importSkeleton(){

  QString fn = askImportFilename(
      "all known formats  (*.SMD; *.MA);;"
      "Studiomdl Data  (*.SMD);;"
      "Maya ascii file (*.MA)"
  );
  if (fn.isEmpty()) return false;
  BrfSkeleton s;

  bool ok=false;
  QString resst = "Unknown extension";

  std::vector<BrfMesh> m;

  if (fn.endsWith(".smd",Qt::CaseInsensitive)) {
    m.resize(1);
    ok = ioSMD::Import(fn.toStdWString().c_str(), m[0], s)==0;
    resst = ioSMD::LastErrorString();
  }

  if (fn.endsWith(".ma",Qt::CaseInsensitive)) {
    ok = IoMB::Import(fn.toStdWString().c_str(), m, s, 1);
    resst = IoMB::LastErrorString();
  }

  if (!ok) {
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Cannot import skeleton:\n%1\n").arg( resst )
    );
    return false;
  }

  QString name=QFileInfo(fn).completeBaseName();
  name.truncate(254);
  sprintf( s.name, "%s", name.toAscii().data());

  insert(s);
  setModified(true);

  statusBar()->showMessage(tr("Imported skeleton \"%1\"--- nbones:%2")
    .arg( s.name )
    .arg( s.bone.size() ),6000
  );
  return true;
}


bool MainWindow::importRiggedMesh(){
  QStringList fnList = askImportFilenames(
      "all known formats  (*.SMD; *.MA);;"
      "Studiomdl Data  (*.SMD);;"
      "Maya ascii file (*.MA)"
  );
  if (fnList.isEmpty()) return false;

  int total = 0;

  for (int j=0; j<fnList.size(); j++) {
    BrfSkeleton s;
    std::vector<BrfMesh> m;
    bool ok=false;
    QString resst = "Unknown extension";

    bool warning = false;


    if (fnList[j].endsWith(".smd",Qt::CaseInsensitive)) {
      m.resize(1);
      ok = ioSMD::Import(fnList[j].toStdWString().c_str(), m[0], s)==0;
      resst = ioSMD::LastErrorString();
      warning = ioSMD::Warning();
    }

    if (fnList[j].endsWith(".ma",Qt::CaseInsensitive)) {
      warning = false;
      ok = IoMB::Import(fnList[j].toStdWString().c_str(), m, s,0);
      resst = IoMB::LastErrorString();
    }

    if (!ok) {
      QMessageBox::information(this,
        tr("Open Brf"),
        tr("Cannot import mesh %2:\n%1\n").arg( resst ).arg(fnList[j])
      );
      continue;
    }

    QString name=QFileInfo(fnList[j]).completeBaseName();
    name.truncate(254);
    for (unsigned int i=0; i<m.size(); i++) {
      QString name2 ;
      if (i>0) name2=QString("%1.%2").arg(name).arg(i); else name2=name;
      sprintf( m[i].name, "%s", name2.toAscii().data());
      applyAfterMeshImport(m[i]);
      insert(m[i]);
    }
    total+=m.size();
    if (warning) {
      QMessageBox::information(this,
        tr("Open Brf"),
        tr("%1\n").arg( ioSMD::LastWarningString() )
      );
    }

  }

  statusBar()->showMessage(
    tr("Imported %1 rigged mesh%2")
    .arg( total ).arg((total==1)?"":"es"),6000
  );

  if (total>0) setModified(true);
  return true;
}

