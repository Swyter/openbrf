#include "guipanel.h"
#include "ui_guipanel.h"

#include "brfData.h"
#include "iniData.h"

#include <QtGui>

static void alignY(QWidget *a, const QWidget *b){
  QRect ar = a->geometry();
  int h = ar.height();
  ar.setTop(b->geometry().top());
  ar.setHeight(h);
  a->setGeometry(ar);
}


class BodyPartModel : public QAbstractListModel{
public:
  BodyPartModel(QObject *parent)
    : QAbstractListModel(parent)
  {  vec.clear(); }
  QStringList vec;
  int getSize() const { return vec.size(); }
  void clear(){
    if (vec.size()>0) {
      vec.clear();
      emit(layoutChanged());
    }
  }
  void setBody(const BrfBody &b){
    int size = (int)b.part.size();
    if (size!=vec.size()) {
      emit(layoutAboutToBeChanged());
      vec.clear();
      emit(layoutChanged());
    }

    for (int i=0; i<size; i++) vec.push_back(b.part[i].name());
    emit(this->dataChanged(createIndex(0,0),createIndex(1,100)));
  }
  int rowCount(const QModelIndex &parent) const {return getSize();}
  int columnCount(const QModelIndex &parent) const {return 1;}
  QVariant data(const QModelIndex &index, int role) const
  {
    if (role==Qt::DisplayRole) {
      int i= index.row();
      if (i<vec.size() && i>=0)
      return (vec[i]);
    }
    return QVariant();
  }
  QVariant headerData(int section, Qt::Orientation orientation, int role) const
  {return QString("header");}
  QModelIndex pleaseCreateIndex(int a, int b){return createIndex(a,b);}
};

class TextureAccessModel : public QAbstractListModel{
public:
  TextureAccessModel(QObject *parent)
    : QAbstractListModel(parent)
  { size = 0; }
  int size;
  void setSize(int i){
    if (size!=i) {
      emit(layoutAboutToBeChanged());
      size=i;
      emit(layoutChanged());
        //emit(this->dataChanged(createIndex(0,0),createIndex(1,100)));
    }
  }
  int rowCount(const QModelIndex &parent) const {return size;}
  int columnCount(const QModelIndex &parent) const {return 1;}
  QVariant data(const QModelIndex &index, int role) const
  {
    if (role==Qt::DisplayRole) {
      switch (index.row()) {
       case 0: return QString("1st");
       case 1: return QString("2nd");
       case 2: return QString("3rd");
       default: return QString("%1th").arg(index.row()+1);
     }
    }
    else return QVariant();
  }
  QVariant headerData(int section, Qt::Orientation orientation, int role) const
  {return QString("header");}
  QModelIndex pleaseCreateIndex(int a, int b){return createIndex(a,b);}
};

void GuiPanel::setIniData(const IniData &inidata){  
  QCompleter *c = new QCompleter( inidata.namelist[TEXTURE] );

  ui->leMatDifA->setCompleter(c);
  ui->leMatDifB->setCompleter(c);
  ui->leMatBump->setCompleter(c);
  ui->leMatEnv->setCompleter(c);
  ui->leMatSpec->setCompleter(c);
  //ui->leMatSpec->completer()->setCompletionMode(QCompleter::InlineCompletion);

  QCompleter *d = new QCompleter( inidata.namelist[SHADER] );
  ui->leMatShader->setCompleter(d);

  QCompleter *e = new QCompleter( inidata.namelist[MATERIAL] );
  ui->boxMaterial->setCompleter(e);
}

QLineEdit* GuiPanel::materialLeFocus(){
  switch (curMaterialFocus){
  case DIFFUSEA: return ui->leMatDifA;
  case DIFFUSEB: return ui->leMatDifB;
  case SHADERNAME: return ui->leMatShader;
  case SPECULAR: return ui->leMatSpec;
  case BUMP: return ui->leMatBump;
  case ENVIRO: return ui->leMatEnv;
  }
  return NULL;
}

void GuiPanel::setTextureData(DdsData d){
  if (displaying==TEXTURE) {


    //if (_selectedIndex<0) {
    //  // multiple sel
    //} else
    {
      ui->boxTextureFileSize->display(d.filesize/1024);
      ui->boxTextureMipmap->display(d.mipmap);
      ui->boxTextureResX->display(d.sx);
      ui->boxTextureResY->display(d.sy);
      switch (d.location) {
        //UNKNOWN, NOWHERE, COMMON, MODULE, LOCAL
        default: ui->boxOrigin->setText(tr("unknown")); break;
        case 1: ui->boxOrigin->setText(tr("not-found")); break;
        case 2: ui->boxOrigin->setText(tr("common")); break;
        case 3: ui->boxOrigin->setText(tr("module")); break;
        case 4: ui->boxOrigin->setText(tr("local")); break;
      }

      switch (d.ddxversion) {
        case 1: ui->boxTextureFormat->setText(tr("DXT 1 (1bit alpha)")); break;
        case 3: ui->boxTextureFormat->setText(tr("DXT 3 (sharp alpha)")); break;
        case 5: ui->boxTextureFormat->setText(tr("DXT 5 (smooth alpha)")); break;
        default: ui->boxTextureFormat->setText(tr("unknown")); break;
      }
    }
    /*
    ui->rbAlphaColor->setEnabled(alpha);
    ui->rbAlphaTransparent->setEnabled(alpha);
    */
  }
}

void GuiPanel::updateHighlight(){
}

GuiPanel::GuiPanel(QWidget *parent, IniData &id) :
    QWidget(parent),
    inidata(id),
    ui(new Ui::GuiPanel)
{
  reference = NULL;
  curMaterialFocus = DIFFUSEA;

  //mapMT = mm;
  _selectedIndex = -1;

  ui->setupUi(this);

  ui->wiBodyAxisA->setVisible(false);
  ui->wiBodyAxisB->setVisible(false);
  ui->wiBodyRadius->setVisible(false);
  ui->wiBodyNFaces->setVisible(false);
  ui->wiBodyNVerts->setVisible(false);
  ui->wiBodySigns->setVisible(false);
  ui->wiBodyFlags->setVisible(false);

  ui->bodyData->setVisible(false);
  ui->meshData->setVisible(false);
  ui->textureData->setVisible(false);
  ui->animationData->setVisible(false);
  ui->materialData->setVisible(false);
  ui->skeletonData->setVisible(false);
  ui->shaderData->setVisible(false);

  ui->lvTextAcc->setModel( new TextureAccessModel(this) );
  ui->lvBodyPart->setModel( new BodyPartModel(this) );

  ui->generalView->setVisible(false);

  alignY(ui->textureData  ,ui->meshData);
  alignY(ui->materialData  ,ui->meshData);
  alignY(ui->animationData,ui->meshData);
  alignY(ui->skeletonData,ui->meshData);
  alignY(ui->shaderData,  ui->meshData);
  alignY(ui->bodyData,  ui->meshData);

  QString flagMask(">Hhhhhhhh");
  ui->boxFlags->setInputMask(flagMask);
  ui->boxTextureFlags->setInputMask(flagMask);
  ui->leMatFlags->setInputMask(flagMask);
  ui->leShaderFlags->setInputMask(flagMask);
  ui->leShaderTaFlags->setInputMask(flagMask);
  ui->leBodyFlags->setInputMask(flagMask);


  //ui->leMatR->setInputMask("0.0000");
  //ui->leMatG->setInputMask("0.0000");
  //ui->leMatB->setInputMask("0.0000");
  //ui->leMatCoeff->setInputMask("0000");
  //ui->timeOfFrame->setInputMask("0000");

  textureAccessDup = new QAction("Duplicate",this);
  textureAccessDel = new QAction("Remove",this);
  textureAccessAdd = new QAction("Add",this);

  bodyPartDup = new QAction("Duplicate",this);
  bodyPartDel = new QAction("Remove",this);
  bodyPartAdd = new QAction("Add",this);

  connect(ui->cbSkin, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateVisibility()));
  connect(ui->cbRefani, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateVisibility()));
  connect(ui->cbRuler, SIGNAL(stateChanged(int)), this, SLOT(updateVisibility()));
  connect(ui->cbRefani, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateRefAnimation()));

  connect(ui->rulerSlid, SIGNAL(sliderMoved (int)), this, SLOT(setRulerLenght(int)));
  connect(ui->rulerSpin, SIGNAL(valueChanged(int)), this, SLOT(setRulerLenght(int)));

  connect(ui->lvTextAcc->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(updateShaderTextaccData()));
  connect(ui->lvBodyPart->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(updateBodyPartData()));

}


static QString String(int i){
  return QString("%1").arg(i);
}
static QString StringF(float i){
  return QString("%1").arg(i);
}
static QString StringH(unsigned int i){
  return QString("%1").arg(i,0,16).toUpper();
}

void GuiPanel::setAnimation(const BrfAnimation* a){
  if (!reference) return;
  if (!ui) return;
  if (!ui->cbRefSkel) return;
  int n = ui->cbRefSkel->currentIndex();
  ui->cbRefSkel->clear();
  for (unsigned int i=0; i<reference->skeleton.size(); i++)
    if (a->nbones==(int)reference->skeleton[i].bone.size())
      ui->cbRefSkel->addItem(reference->skeleton[i].name);
  if (n<0 || n>=ui->cbRefSkel->count()) n=0;
  ui->cbRefSkel->setCurrentIndex(n);
}

void GuiPanel::updateRefAnimation(){
  if (!reference) return;
  int a=ui->cbRefani->currentIndex()-1;
  setRefAnimation(a);
}
void GuiPanel::setRefAnimation(int i){
  if (reference)
    if (i>=0 && i<(int)reference->animation.size())
      setAnimation(&(reference->animation[i]));
}



void GuiPanel::setReference(BrfData* r){
  ui->cbRefani->clear();
  ui->cbRefani->addItem("no anim");
  reference = r;
  for (unsigned int i=0; i<r->animation.size(); i++)
    ui->cbRefani->addItem(r->animation[i].name);

  int n = r->GetFirstUnusedLetter();
  ui->cbSkin->clear();
  ui->cbSkin->addItem("none");
  for (char a='A'; a<'A'+n; a++)
    ui->cbSkin->addItem(tr("Mesh-set %1").arg(a));

}



int GuiPanel::getCurrentSubpieceIndex(int type) const{

  if (type==SHADER) {
    if (!ui->lvTextAcc->selectionModel()) return -1;
    QModelIndexList il = ui->lvTextAcc->selectionModel()->selectedIndexes();
    if (il.size()>0) {
      int i = il.first().row();
      if (i<ui->lvTextAcc->model()->rowCount())
      return  i;
    }
  }

  if (type==BODY) {
    if (!ui->lvBodyPart->selectionModel()) return -1;
    QModelIndexList il = ui->lvBodyPart->selectionModel()->selectedIndexes();
    if (il.size()>0) {
      int i = il.first().row();
      if (i<ui->lvBodyPart->model()->rowCount())
      return  i;
    }
  }

  if (type==MESH) {
    if (! (ui->meshDataAni->isVisible())) return 0;
    return (ui->frameNumber->value());
  }

  return -1;
}



void GuiPanel::updateBodyPartSize(){
  updateBodyPartData();
}
void GuiPanel::updateBodyPartData(){
 int sel=_selectedIndex;

 if (sel>=(int)data->body.size()) return;
 if (sel<0) return;

 BrfBody &b(data->body[sel]);

 ((BodyPartModel*)(ui->lvBodyPart->model()))->setBody( b );

 int ta=getCurrentSubpieceIndex(BODY) ;
 if (ta>=(int)b.part.size()) return;
 if (ta<0) return;
 BrfBodyPart p(b.part[ta]);
 bool vis[7]={false,false,false,false,false, false ,false};
 ui->leBodyFlags->setText(StringH( p.flags));
 if (p.ori==-1)
   ui->leBodySign->setText("-1");
 else
   ui->leBodySign->setText("+1");
 switch (p.type)
 {
   case BrfBodyPart::SPHERE:
     ui->labelAxisRadius->setText("center:");
     ui->leBodyAX->setText( StringF( p.center.X() ));
     ui->leBodyAY->setText( StringF( p.center.Y() ));
     ui->leBodyAZ->setText( StringF( p.center.Z() ));
     ui->leBodyRad->setText( StringF( p.radius ));
     vis[0]=true; vis[2]=true; vis[5]=true;
     break;
   case BrfBodyPart::CAPSULE:
     ui->labelAxisRadius->setText("axisA:");
     ui->leBodyAX->setText( StringF( p.center.X() ));
     ui->leBodyAY->setText( StringF( p.center.Y() ));
     ui->leBodyAZ->setText( StringF( p.center.Z() ));
     ui->leBodyBX->setText( StringF( p.dir.X() ));
     ui->leBodyBY->setText( StringF( p.dir.Y() ));
     ui->leBodyBZ->setText( StringF( p.dir.Z() ));
     ui->leBodyRad->setText( StringF( p.radius ));
     vis[0]=true; vis[1]=true; vis[2]=true;vis[5]=true;
     break;
   case BrfBodyPart::FACE:
     ui->leBodyNVert->display( (int)p.pos.size() );
     vis[4]=true;vis[5]=true;
     break;
   case BrfBodyPart::MANIFOLD:
     ui->leBodyNVert->display( (int)p.pos.size() );
     ui->leBodyNFace->display( (int)p.face.size() );
     vis[3]=true; vis[4]=true;vis[6]=true;
     break;
   default: break;
 }
 ui->wiBodyAxisA->setVisible(vis[0]);
 ui->wiBodyAxisB->setVisible(vis[1]);
 ui->wiBodyRadius->setVisible(vis[2]);
 ui->wiBodyNFaces->setVisible(vis[3]);
 ui->wiBodyNVerts->setVisible(vis[4]);
 ui->wiBodyFlags->setVisible(vis[5]);
 ui->wiBodySigns->setVisible(vis[6]);
}

void GuiPanel::updateShaderTextaccSize(){
 int sel=_selectedIndex;
 if (sel>=(int)data->shader.size()) return;
 if (sel<0) return;

 BrfShader &s(data->shader[sel]);

 int ta=getCurrentSubpieceIndex(SHADER) ;

 TextureAccessModel* tam =((TextureAccessModel*)(ui->lvTextAcc->model()));
 tam->setSize( s.opt.size() );
 if (ta<0 || ta>=(int)s.opt.size()) {
   ui->lvTextAcc->selectionModel()->select(
       tam->pleaseCreateIndex(0,0),
       QItemSelectionModel::Select
   );
   ta=getCurrentSubpieceIndex(SHADER) ;
 }
 updateShaderTextaccData();
}

void GuiPanel::updateShaderTextaccData(){
 int sel=_selectedIndex;

 if (sel>=(int)data->shader.size()) return;
 if (sel<0) return;

 BrfShader &s(data->shader[sel]);

 int ta=getCurrentSubpieceIndex(SHADER) ;


 if (ta>=(int)s.opt.size()) return;
 if (ta>=0) {
   ui->leShaderTaColorOp->setText( String(s.opt[ta].colorOp) );
   ui->leShaderTaAlphaOp->setText( String(s.opt[ta].alphaOp) );
   ui->leShaderTaFlags->setText( StringH(s.opt[ta].flags) );
   ui->leShaderTaMap  ->setText( String(s.opt[ta].map) );
 } else {
   ui->leShaderTaColorOp->clear();
   ui->leShaderTaAlphaOp->clear();
   ui->leShaderTaFlags->clear();
   ui->leShaderTaMap  ->clear();
 }
}

void myClear(QLineEdit *l){
  l->blockSignals(true);
  l->setText("");
  l->setFrame(true);
  l->setForegroundRole(QPalette::Link);
  l->setBackgroundRole(QPalette::Base);
  l->blockSignals(false);
}

void myClear(QLCDNumber *qlc){
  qlc->display(0);
}

void myClear(QSpinBox *qsb){
  qsb->blockSignals(true);
  qsb->clear();//setValue(0);
  qsb->setFrame(true);
  qsb->blockSignals(false);
}
void mySetText(QLineEdit *l, QString s){
  l->blockSignals(true);
  QString old = l->text();
  if (l->hasFrame() && old.isEmpty() ) l->setText(s);
  else {
    if (s!=old) {
      l->setText("");
      //qDebug("Was '%s' is '%s'",s.toAscii().data(), old.toAscii().data() );
      l->setBackgroundRole(QPalette::Button);//QPalette::AlternateBase);
      l->setFrame(false);
    }
  }
  //QFont f = l->font();
  //f.setItalic(s=="none");
  //l->setFont(f);
  l->blockSignals(false);
}

void mySetText(QSpinBox *l, int i){
  l->blockSignals(true);
  if (l->hasFrame() && l->text().isEmpty()){
    l->setValue(i);
  } else {
    if (l->value()!=i) {
      l->clear();
      l->setBackgroundRole(QPalette::Button);//QPalette::AlternateBase);
      l->setFrame(false);
    }

  }
  l->blockSignals(false);
}

void mySetValueAdd(QLCDNumber *qlc, int n){
  qlc->display(n+qlc->value());
}

void mySetValueMax(QLCDNumber *qlc, int n){
  if (n>qlc->value())
  qlc->display(n);
}

static void mySetCompositeVal(int &a, bool b){
  if (a==-1) a=(b)?2:0;
  else if (a==2) a=(b)?2:1;
  else if (a==0) a=(b)?1:0;
}
static Qt::CheckState myCheckState(int a){
  switch (a) {
  case 1:  return Qt::PartiallyChecked;
  case 2:  return Qt::Checked;
  default: return Qt::Unchecked;
  }
}

void GuiPanel::setSelection(const QModelIndexList &newsel, int k){
  int sel=-1;
  int nsel = (int)newsel.size();
  if (newsel.size()!=0) sel = newsel[0].row();

  //bool vertexani=false;
  //bool rigged=false;
  //bool vertexcolor=false;
  /*bool manyMaterials=false;
  int flags=-1;
  char materialSt[255]="";
  bool differentAni = false;

  int nv=0, nf=0, nfr=0, np=0;
  int last = -1;

  for (QModelIndexList::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
    sel = i->row();
    if (k==MESH && sel<(int)data->mesh.size() ) {
      BrfMesh *m = &(data->mesh[sel]);
      rigged |= m->isRigged;
      vertexani |= m->frame.size()>1;
      vertexcolor |= m->hasVertexColor;
      np += m->frame[0].pos.size();
      nv += m->vert.size();
      int k = m->frame.size();
      if (nfr>k || !nfr) nfr=k;

      if (!differentAni) {

        if (last!=-1) {
          if (data->mesh[sel].frame.size()!=data->mesh[last].frame.size()) differentAni=true;
          else for (unsigned int fi=0; fi < data->mesh[sel].frame.size(); fi++)
            if (data->mesh[sel].frame[fi].time!=data->mesh[last].frame[fi].time) differentAni=true;
        } else {
          for (unsigned int fi=0; fi < data->mesh[sel].frame.size(); fi++)
            _frameTime[fi]=data->mesh[sel].frame[fi].time;
        }
        last = sel;
      }

      nf += m->face.size();
      if (!materialSt[0]) sprintf(materialSt,"%s",m->material);
      else if (strcmp(materialSt,m->material)) {
        sprintf(materialSt,"%s","<various>");
        manyMaterials=true;
      }
      if (flags==-1) flags=m->flags; else {
        if (flags!=(int)m->flags) flags=-2;
      }
    }


  }
  if (k==-1) k=NONE;

  BrfMesh *m = NULL;
  BrfTexture *tex = NULL;
  BrfAnimation *ani = NULL;
*/
_selectedIndex = sel;
//_nsel =
switch (TokenEnum(k)){
  case MATERIAL:{
    myClear(ui->leMatBump);
    myClear(ui->leMatDifA);
    myClear(ui->leMatDifB);
    myClear(ui->leMatEnv);
    myClear(ui->leMatShader);
    myClear(ui->leMatSpec);

    myClear(ui->leMatFlags);
    myClear(ui->leMatRendOrd);
    myClear(ui->leMatCoeff);
    myClear(ui->leMatR);
    myClear(ui->leMatG);
    myClear(ui->leMatB);

    for (QModelIndexList::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
      int sel = i->row();
      if (sel<0 || sel>=(int)data->material.size())  break;
      BrfMaterial &m(data->material[sel]);

      mySetText(ui->leMatBump,  m.bump );
      mySetText(ui->leMatDifA,  m.diffuseA );
      mySetText(ui->leMatDifB,  m.diffuseB );
      mySetText(ui->leMatEnv,   m.enviro);
      mySetText(ui->leMatShader,m.shader );
      mySetText(ui->leMatSpec,  m.spec );

      mySetText(ui->leMatFlags, StringH(m.flags) );
      mySetText(ui->leMatRendOrd, m.renderOrder );
      mySetText(ui->leMatCoeff, StringF(m.specular) );
      mySetText(ui->leMatR, StringF( m.r ));
      mySetText(ui->leMatG, StringF( m.g ));
      mySetText(ui->leMatB, StringF( m.b ));
    }
    break;
    }

  case MESH: {
    myClear(ui->boxFlags);
    myClear(ui->boxMaterial);
    myClear(ui->boxNVerts);
    myClear(ui->boxNFaces);
    myClear(ui->boxNPos);
    myClear(ui->boxNVerts);
    myClear(ui->boxNFrames);
    ui->meshDataAni->setVisible(false);
    ui->rbRiggingcolor->setEnabled(false);
    ui->rbVertexcolor->setEnabled(false);
    ui->viewRefAni->setVisible( false );

    int hasAni=-1,hasCol=-1,hasTan=-1,hasRig=-1;
    for (QModelIndexList::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
      int sel = i->row();
      if (sel<0 || sel>=(int)data->mesh.size())  break;
      BrfMesh *m=&(data->mesh[sel]);

      mySetText(ui->boxFlags, StringH(m->flags & ~(3<<16) ));
      mySetText( ui->boxMaterial ,  m->material );

      mySetValueAdd( ui->boxNVerts , m->vert.size());
      mySetValueAdd( ui->boxNFaces , m->face.size());
      mySetValueAdd( ui->boxNPos   , m->frame[0].pos.size());
      mySetValueMax( ui->boxNFrames, m->frame.size());

      mySetCompositeVal(hasAni, m->HasVertexAni());
      mySetCompositeVal(hasCol, m->hasVertexColor);
      mySetCompositeVal(hasTan, m->HasTangentField());
      mySetCompositeVal(hasRig, m->isRigged);

      for (unsigned int fi=0; fi < m->frame.size(); fi++)
         frameTime[fi]=m->frame[fi].time;
    }

    if (hasAni>0)  {
      ui->rbRiggingcolor->setEnabled( true );
      ui->viewRefAni->setVisible( true );
    }
    if (hasCol>0) ui->rbVertexcolor->setEnabled(true);
    if (hasAni>0) ui->meshDataAni->setVisible(true);
    ui->cbMeshHasAni->setCheckState(myCheckState(hasAni));
    ui->cbMeshHasCol->setCheckState(myCheckState(hasCol));
    ui->cbMeshHasTan->setCheckState(myCheckState(hasTan));
    ui->cbMeshHasRig->setCheckState(myCheckState(hasRig));

    updateMaterial(ui->boxMaterial->text());
    ui->timeOfFrame->setEnabled( newsel.size()==1 );

    int nfr = (int)ui->boxNFrames->value();
    if (nfr>0)
    ui->frameNumber->setMaximum(nfr -1 );
    ui->frameNumber->setMinimum( 0 );
    ui->frameNumber->setWrapping(true);

    break;
    }

  case TEXTURE:
    myClear(ui->boxTextureFlags);

    for (QModelIndexList::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
      int j=i->row();
      if (j>=0 && j<(int)data->texture.size())
        mySetText(ui->boxTextureFlags,StringH(data->texture[j].flags));
    }
    break;
  case SKELETON:
    if (sel>=0 && nsel==1 && sel<(int)data->skeleton.size()) {
      BrfSkeleton &s(data->skeleton[sel]);
      ui->boxSkelNBones->display( (int)s.bone.size() );
    }
    break;
  case ANIMATION:
    {
    BrfAnimation *ani =NULL;
    if (sel>=0 && nsel==1 && sel<(int)data->animation.size()) ani=&(data->animation[sel]);
    ui->boxAniNBones->display( (ani)?ani->nbones:0 );
    ui->boxAniNFrames->display( (ani)?(int)ani->frame.size():0 );
    ui->boxAniMinFrame->display( (ani)?ani->FirstIndex():0 );
    ui->boxAniMaxFrame->display( (ani)?ani->LastIndex():0 );

    if (ani) {
      for (unsigned int fi=0; fi < ani->frame.size(); fi++)
       frameTime[fi]=ani->frame[fi].index;
      ui->frameNumberAni->setMaximum(ani->frame.size());
      ui->frameNumberAni->setMinimum(1);
    }

    ui->rbRiggingcolor->setEnabled( true ); // quick: just let user edit them
    ui->rbVertexcolor->setEnabled( true );
    ui->viewRefSkel->setVisible( true );
    if (ani) setAnimation(ani);
    }
    break;
  case SHADER:
      {
      if (!newsel.size()) break;
      int sel = newsel[0].row();
      if (sel<0 || sel>=(int)data->shader.size())  break;
      BrfShader &s(data->shader[sel]);
      ui->leShaderTechnique->setText( s.technique );
      ui->leShaderFallback->setText( s.fallback );
      ui->leShaderFlags->setText( StringH(s.flags) );
      ui->leShaderRequires->setText( String(s.requires) );
      updateShaderTextaccSize();
      }
      break;
    case BODY:
      updateBodyPartData();
      break;
    default:

      break;
  }

  ui->animationData->setVisible(k == ANIMATION);
  ui->textureData->setVisible(k == TEXTURE);
  ui->meshData->setVisible(k == MESH);
  ui->materialData->setVisible( k==MATERIAL );
  ui->skeletonData->setVisible( k==SKELETON );
  ui->shaderData->setVisible( k==SHADER );
  ui->bodyData->setVisible( k==BODY );
  ui->generalView->setVisible( k!=SHADER && k!=NONE );

  QRect rect = ui->generalView->geometry();
  if (k==MATERIAL)
    rect.setTop( ui->materialData->geometry().bottom() + 40 );
  else
    rect.setTop( ui->meshData->geometry().bottom() + 40 );
  ui->generalView->setGeometry(rect);

  displaying=k;
  updateVisibility();
  updateFrameNumber( ui->frameNumber->value() );

}



void GuiPanel::updateMaterial(QString a){

  if (ui->boxMaterial->hasFrame()) {
    bool bump;
    bool spec;
    QString s = inidata.mat2tex(a,&bump,&spec);
    if (s.isEmpty()) s="<not found>";
    ui->boxTexture  ->setText( s );
    ui->cbNormalmap->setEnabled(bump);
    ui->cbSpecularmap->setEnabled(spec);
  } else
    ui->boxTexture  ->setText( "<various>" );
}

void GuiPanel::updateFrameNumber(int newFr){
  if (newFr<0) return;
  if (displaying==MESH) {
    if (!ui->meshDataAni->isVisible()) return;
    if (ui->timeOfFrame->isEnabled()) {

      ui->frameNumber->blockSignals(true);
      ui->frameNumber->setValue(newFr);
      ui->frameNumber->blockSignals(false);

      ui->timeOfFrame->blockSignals(true);
      ui->timeOfFrame->setText( QString("%1").arg( frameTime[newFr] ) );
      ui->timeOfFrame->blockSignals(false);
    }
    else {
      ui->timeOfFrame->blockSignals(true);
      ui->timeOfFrame->setText( QString("---") );
      ui->timeOfFrame->blockSignals(false);
    }
  }
  if (displaying==ANIMATION) {
      ui->frameNumberAni->blockSignals(true);
      ui->frameNumberAni->setValue(newFr);
      ui->frameNumberAni->blockSignals(false);

      ui->timeOfFrameAni->blockSignals(true);
      ui->timeOfFrameAni->setText( QString("%1").arg( frameTime[newFr-1] ) );
      ui->timeOfFrameAni->blockSignals(false);
  }

}

void GuiPanel::setRulerLenght(int l){
  ui->rulerSlid->blockSignals(true);
  ui->rulerSpin->blockSignals(true);

  ui->rulerSlid->setValue(l);
  ui->rulerSpin->setValue(l);

  ui->rulerSlid->blockSignals(false);
  ui->rulerSpin->blockSignals(false);
}

void GuiPanel::updateVisibility(){

  int k=displaying;

  ui->viewRuler->setVisible(k==MESH);

  // set visibility
  if (k==MESH) {
    ui->viewFloor->setVisible(true);
    ui->viewRefSkin->setVisible(false);

    ui->viewMeshRendering->setVisible(true);
    ui->viewAni->setVisible( (ui->viewRefAni->isVisible() && ui->cbRefani->currentIndex())
                             || ui->meshDataAni->isVisible() );
    ui->viewRefSkel->setVisible( ui->viewRefAni->isVisible() && ui->cbRefani->currentIndex() );
    ui->rulerSlid->setVisible(ui->cbRuler->isChecked());
    ui->rulerSpin->setVisible(ui->cbRuler->isChecked());
  } else if (k==ANIMATION) {
    ui->viewFloor->setVisible(false);
    ui->viewRefSkin->setVisible(true);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible( ui->cbSkin->currentIndex() );
    ui->viewAni->setVisible(true);
    ui->viewRefSkel->setVisible(true);
  } else if (k==BODY) {
    ui->viewFloor->setVisible(true);
    ui->viewRefSkin->setVisible(false);
    ui->viewRefSkel->setVisible(false);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible(false);
    ui->viewAni->setVisible(false);
  } else if (k==MATERIAL || k==TEXTURE) {
    ui->viewRefSkin->setVisible(false);
    ui->viewRefSkel->setVisible(false);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible(false);
    ui->viewFloor->setVisible(false);
    ui->viewAni->setVisible(false);
  } else if (k==SKELETON) {
    ui->viewFloor->setVisible(false);
    ui->viewRefSkin->setVisible(true);
    ui->viewRefSkel->setVisible(false);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible(false);
    ui->viewAni->setVisible(false);
  }
  else {
    ui->viewFloor->setVisible(false);
    ui->viewRefSkin->setVisible(false);
    ui->viewRefSkel->setVisible(false);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible(false);
    ui->viewAni->setVisible(false);
  }

  ui->viewShowAlpha->setVisible(k==MATERIAL || k==TEXTURE);


}

GuiPanel::~GuiPanel()
{
  delete ui;
}

void GuiPanel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void GuiPanel::on_lvTextAcc_customContextMenuRequested(QPoint pos)
{
  QMenu menu(this);
  int k=getCurrentSubpieceIndex(SHADER);
  if (k>=0) {
    menu.addAction(textureAccessDup);
    menu.addAction(textureAccessDel);
  }
  menu.addAction(textureAccessAdd);
  menu.exec(cursor().pos());
  //pos+ui->lvTextAcc->pos()+this->pos()+ui->shaderData->pos()+((QWidget*)parent())->pos()+
  //          ((QWidget*)(parent()->parent()))->pos());
  //event->accept();
}

void GuiPanel::showMaterialDiffuseA(){curMaterialFocus = DIFFUSEA; updateHighlight(); emit(followLink());}
void GuiPanel::showMaterialDiffuseB(){curMaterialFocus = DIFFUSEB; updateHighlight();emit(followLink());}
void GuiPanel::showMaterialBump(){curMaterialFocus = BUMP; updateHighlight();emit(followLink());}
void GuiPanel::showMaterialEnviro(){curMaterialFocus = ENVIRO; updateHighlight();emit(followLink());}
void GuiPanel::showMaterialSpecular(){curMaterialFocus = SPECULAR; updateHighlight();emit(followLink());}
void GuiPanel::showMaterialShader(){curMaterialFocus = SHADERNAME; updateHighlight();emit(followLink());}

void GuiPanel::setNavigationStackDepth(int i){
  ui->labBackM->setVisible(i>0);
  ui->labBackT->setVisible(i>1);
  ui->labBackS->setVisible(i>1);
}

void GuiPanel::on_listView_customContextMenuRequested(QPoint pos)
{
  return;
  QMenu menu(this);
  int k=getCurrentSubpieceIndex(BODY);
  if (k>=0) {
    menu.addAction(bodyPartDup);
    menu.addAction(bodyPartDel);
  }
  menu.addAction(bodyPartAdd);
  menu.exec(cursor().pos());

}
