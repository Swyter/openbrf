#include "brfdata.h"
#include "glwidgets.h"
#include "selector.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_guipanel.h"
#include "vcgmesh.h"

#include <QtGui>

bool MainWindow::maybeSave()
{
    if (isModified) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("BrfEdit"),
                     tr("This dataset has been modified.\n"
                        "Save changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
          return saveAs();
        else if (ret == QMessageBox::Cancel)
          return false;
    }
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
  setModified(true);
}

void MainWindow::onChangeMeshFlags(QString st){
  if (!glWidget) return;
  int n=0;
  unsigned int fl  =  st.toUInt();
  st.truncate(254);

  for (unsigned int i=0; i<GLWidget::MAXSEL; i++) {
    if (i>=brfdata.mesh.size()) break;
    if (glWidget->selGroup[i]) {
      brfdata.mesh[i].flags = fl;
      n++;
    }
  }
  statusBar()->showMessage( tr("Set %1 mesh flags to \"%2\"").arg(n).arg(fl) );
  setModified(true);
}

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
   // : QWidget(parent)//, ui(new Ui::MainWindow)
{

  settings = new QSettings("mtarini", "OpenBRF");

  glWidget = new GLWidget;
  selector = new Selector(this);

  isModified=false;

  createActions();
  createMenus();

  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           glWidget,  SLOT(setSelection(QModelIndexList,int)) );

  connect( selector, SIGNAL(breakAni(int,bool)),
           this, SLOT(breakAni(int,bool)));

  QSplitter* main = new QSplitter();
  guiPanel = new GuiPanel();

  main->addWidget(selector);
  main->addWidget(guiPanel);
  main->addWidget(glWidget);
  connect(guiPanel->ui->cbLighting,     SIGNAL(stateChanged(int)),glWidget,SLOT(setLighting(int)));
  connect(guiPanel->ui->cbTexture,      SIGNAL(stateChanged(int)),glWidget,SLOT(setTexture(int)));
  connect(guiPanel->ui->cbWireframe    ,SIGNAL(stateChanged(int)),glWidget,SLOT(setWireframe(int)));
  connect(guiPanel->ui->rbNocolor      ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerWhite()));
  connect(guiPanel->ui->rbRiggingcolor ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerRig()));
  connect(guiPanel->ui->rbVertexcolor  ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerVert()));
  connect(guiPanel->ui->buPlay         ,SIGNAL(clicked()),glWidget,SLOT(setPlay()));
  connect(guiPanel->ui->buPause        ,SIGNAL(clicked()),glWidget,SLOT(setPause()));
  connect(guiPanel->ui->buStop         ,SIGNAL(clicked()),glWidget,SLOT(setStop()));
  connect(guiPanel->ui->boxMaterial    ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeMeshMaterial(QString)));
  connect(guiPanel->ui->boxFlags       ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeMeshFlags(QString)));
  //connect(guiPanel->ui->boxMaterial    ,SIGNAL(),this,SLOT(onChangeMeshMaterial));


  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           guiPanel,  SLOT(setSelection(QModelIndexList,int)) );

  //main->setLayout(mainLayout);

  reference.Load("reference.brf");
  setCentralWidget(main);

  updateTitle();

  glWidget->selected=1;
  glWidget->data = guiPanel->data = &brfdata;
  glWidget->reference = &reference;

  this->setAcceptDrops(true);

}

template <class T> const char* add(vector<T>& v, const vector<T>& t, int i){
  v.push_back(t[i]);
  return t[i].name;
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

bool MainWindow::importMeshPly(){
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
  else brfdata.mesh.push_back(m);

  selector->updateData(brfdata);
  setModified(true);

  statusBar()->showMessage(tr("Imported mesh \"%1\"--- normals:%2 colors:%3 texture_coord:%4")
    .arg( m.name )
    .arg((VcgMesh::gotNormals()?"[ok]":"[recomputed]"))
    .arg((VcgMesh::gotColor()?"[ok]":"[NO]"))
    .arg((VcgMesh::gotTexture()?"[ok]":"[NO]"))
  );
  return true;
}


bool MainWindow::exportSkelMod(){
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

bool MainWindow::importSkelMod(){
    QMessageBox::information(this,
      tr("Open Brf"),
      tr("Import skeleton:: to do")
    );  return false;
}

bool MainWindow::exportPly(){
  int i = selector->firstSelected();
  if (i<0) return false;
  if (i>(int)brfdata.mesh.size()) return false;
  
  QString fn = askExportFilename(brfdata.mesh[ i ].name);
  switch (selector->currentTabName()) {
    case MESH:
      // save mesh as Ply
      if (fn.isEmpty()) return false;
      VcgMesh::add(brfdata.mesh[ i ]);
      return VcgMesh::save(fn.toAscii().data());
    break;
    default: assert(0); // how was this signal sent?!
  }
  return false;
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
static bool _del(vector<T> &t, int i){
  if (i<0 || i>=(int)t.size()) return false;
  t.erase(t.begin()+i,t.begin()+i+1); return true;
}
template< class T >
static char* _name(T &t, int i){
  if (i<0 || i>=(int)t.size()) return NULL;
  return t[i].name;
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
    newName.truncate(254);
    if (ok) {
      sprintf(name,newName.toAscii().data());
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
    ext
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
  "3D studio Max (*.3ds);;"
  "Collada (*.dae);;"
  "AutoCad (*.dxf);;"  // n:C n:N
  "VRML (*.vrml);;"
  "Object File Format (*.off);;" // e:N e:C
  "Wavefront Object Files (*.obj);;" // e:C n:N
  "Intermediate Data Text File (*.idtf);;"
  "Open Inventor (*.iv);;"
  //"(*.smf);;"
  "Universal 3D (*.u3d);;"
  "Stereolithography 3D system(*.stl);;" // n:C
  "VMI (*.vmi);;"
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
      //if (!fileName.isEmpty()) loadFile(fileName);
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
//! [13] //! [14]
{

   QMessageBox::about(this, tr("Open-Brf"),
            tr("<b>ver 0.0.5 pre-alpha</b><br>"
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
    //fileMenu->addAction(newAct);
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

    helpMenu = menuBar()->addMenu(tr("&Info"));
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

bool MainWindow::loadFile(const QString &fileName)
{
  setCurrentFile(fileName);
  if (!brfdata.Load(fileName.toAscii().data())) {
     QMessageBox::information(this, tr("Open BRF"),
                              tr("Cannot load %1.").arg(fileName));

     return true;
  } else  {
    selector->setup(brfdata);
    //glWidget->selectNone();
    selector->setCurrentIndex(100); // for some reason, if I set the 0 message is not sent

    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded!"), 2000);
    setModified(false);
    return true;
  }
}

bool MainWindow::saveFile(const QString &fileName)
{
  setCurrentFile(fileName);
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
    if (curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
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

  saveFile(fileName);
  setCurrentFile(fileName);
  setModified(false);
  return true;
}

void MainWindow::updateTitle(){
  QString maybestar = (isModified)?QString("(*)"):QString("");
  if (curFile.isEmpty())
    setWindowTitle(tr("OpenBrf%1").arg(maybestar));
  else
    setWindowTitle(tr("%2 - %1%3")
       .arg(curFile).arg(tr("OpenBrf")).arg(maybestar));
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

void MainWindow::createActions()
{
    //newAct = new QAction(tr("&New"), this);
    //newAct->setShortcuts(QKeySequence::New);
    //newAct->setStatusTip(tr("Create a new file"));
    //connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

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
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    //aboutQtAct = new QAction(tr("About &Qt"), this);
    //aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    //connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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
