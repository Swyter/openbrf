#include "guipanel.h"
#include "ui_guipanel.h"

#include "brfData.h"

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

GuiPanel::GuiPanel(QWidget *parent, MapSS *mm) :
    QWidget(parent),
    ui(new Ui::GuiPanel)
{
  reference = NULL;
  mapMT = mm;
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

  QString flagMask("\\0\\x>Hhhhhhhh");
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
  connect(ui->cbRefani, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateRefAnimation()));

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
  return QString("0x%1").arg(i,0,16);
}

void GuiPanel::setAnimation(const BrfAnimation* a){
  if (!reference) return;
  if (!ui) return;
  if (!ui->cbRefSkel) return;
  ui->cbRefSkel->clear();
  for (unsigned int i=0; i<reference->skeleton.size(); i++)
    if (a->nbones==(int)reference->skeleton[i].bone.size())
      ui->cbRefSkel->addItem(reference->skeleton[i].name);
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

static int _frameTime[1000]; // how elegant is that? ;)

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

void GuiPanel::setSelection(const QModelIndexList &newsel, int k){
  int sel=-1;
  int nsel = (int)newsel.size();

  bool vertexani=false;
  bool rigged=false;
  bool vertexcolor=false;
  bool manyMaterials=false;
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
  _selectedIndex = sel;
  if (k==-1) k=NONE;

  BrfMesh *m = NULL;
  BrfTexture *tex = NULL;
  BrfAnimation *ani = NULL;

  // set data
  if (sel>=0)
  switch (TokenEnum(k)){
    case MESH:
      if (sel>=0 && nsel==1) m=&(data->mesh[sel]);
      ui->boxFlags    ->setText( (flags>=0)?StringH( flags ):"" );
      ui->boxFlags    ->setReadOnly( flags<0 || !nsel);
      //ui->boxName     ->setText( (m)?String(m->name     ):"" );
      ui->boxMaterial ->setText( materialSt );
      ui->boxMaterial ->setEnabled( !manyMaterials && nsel!=0);

      updateMaterial(materialSt);

      ui->boxNVerts   ->display( nv );
      ui->boxNFaces   ->display( nf );
      ui->boxNFrames  ->display( nfr);
      ui->boxNPos     ->display( np );
      ui->meshDataAni->setVisible( vertexani);
      ui->viewRefAni->setVisible(rigged);
      ui->timeOfFrame->setEnabled( !differentAni );
      if (nfr>0)
      ui->frameNumber->setMaximum(nfr -1 );
      ui->frameNumber->setMinimum( 0 );
      ui->frameNumber->setWrapping(true);
      ui->rbRiggingcolor->setEnabled( rigged );
      ui->rbVertexcolor->setEnabled(vertexcolor);
      break;
    case TEXTURE:
      if (sel>=0 && nsel==1 && sel<(int)data->texture.size()) tex=&(data->texture[sel]);
      ui->boxTextureFlags ->setText( (tex)?StringH(tex->flags):"" );
      break;
    case SKELETON:
      if (sel>=0 && nsel==1 && sel<(int)data->skeleton.size()) {
        BrfSkeleton &s(data->skeleton[sel]);
        ui->boxSkelNBones->display( (int)s.bone.size() );
      }
      break;
    case ANIMATION:
      if (sel>=0 && nsel==1 && sel<(int)data->animation.size()) ani=&(data->animation[sel]);
      ui->boxAniNBones->display( (ani)?ani->nbones:0 );
      ui->boxAniNFrames->display( (ani)?(int)ani->frame.size():0 );
      ui->boxAniMinFrame->display( (ani)?ani->FirstIndex():0 );
      ui->boxAniMaxFrame->display( (ani)?ani->LastIndex():0 );

      ui->rbRiggingcolor->setEnabled( true ); // quick: just let user edit them
      ui->rbVertexcolor->setEnabled( true );
      ui->viewRefSkel->setVisible( true );
      if (ani) setAnimation(ani);
      break;
    case MATERIAL:
      {
      BrfMaterial &m(data->material[sel]);
      ui->leMatFlags->setText( StringH(m.flags) );
      ui->leMatBump->setText( m.bump );
      ui->leMatDifA->setText( m.diffuseA );
      ui->leMatDifB->setText( m.diffuseB );
      ui->leMatEnv->setText(m.enviro);
      ui->leMatR->setText( StringF( m.r ));
      ui->leMatG->setText( StringF( m.g ));
      ui->leMatB->setText( StringF( m.b ));
      ui->leMatCoeff->setText( StringF(m.specular) );
      ui->leMatShader->setText( m.shader );
      ui->leMatSpec->setText( m.spec );
      }
      break;
    case SHADER:
      {
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
  ui->generalView->setVisible( k!=SHADER && k!=SKELETON && k!=NONE );

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
  if (ui->boxMaterial->isEnabled()) {
    std::string s  = (*mapMT)[ std::string(a.toAscii().data() ) ];
    if (!s.length()) s="<not found>";
    ui->boxTexture  ->setText( s.c_str() );
  } else
    ui->boxTexture  ->setText( "<various>" );
}

void GuiPanel::updateFrameNumber(int newFr){
  if (newFr<0) return;
  if (!ui->meshDataAni->isVisible()) return;
  if (ui->timeOfFrame->isEnabled()) {
    int oldFr =ui->frameNumber->value();
    if (oldFr!=newFr) ui->frameNumber->setValue(newFr);
    ui->timeOfFrame->setText( QString("%1").arg( _frameTime[newFr] ) );
  }
  else
    ui->timeOfFrame->setText( QString("---") );

}

void GuiPanel::updateVisibility(){

  int k=displaying;


  // set visibility
  if (k==MESH) {
    ui->viewFloor->setVisible(true);
    ui->viewRefSkin->setVisible(false);

    ui->viewMeshRendering->setVisible(true);
    ui->viewAni->setVisible( (ui->viewRefAni->isVisible() && ui->cbRefani->currentIndex())
                             || ui->meshDataAni->isVisible() );
    ui->viewRefSkel->setVisible( ui->viewRefAni->isVisible() && ui->cbRefani->currentIndex() );
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
  } else {
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
