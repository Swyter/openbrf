#include "guipanel.h"
#include "ui_guipanel.h"

#include "brfData.h"

static void alignY(QWidget *a, const QWidget *b){
  QRect ar = a->geometry();
  int h = ar.height();
  ar.setTop(b->geometry().top());
  ar.setHeight(h);
  a->setGeometry(ar);
}

GuiPanel::GuiPanel(QWidget *parent, MapSS *mm) :
    QWidget(parent),
    ui(new Ui::GuiPanel)
{
  mapMT = mm;
  ui->setupUi(this);

  ui->meshData->setVisible(false);
  ui->textureData->setVisible(false);
  ui->animationData->setVisible(false);
  ui->materialData->setVisible(false);
  ui->generalView->setVisible(false);

  alignY(ui->textureData  ,ui->meshData);
  alignY(ui->materialData  ,ui->meshData);
  alignY(ui->animationData,ui->meshData);

  ui->boxFlags->setInputMask("00000000");
  ui->boxTextureFlags->setInputMask("00000000");
  connect(ui->cbSkin, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateVisibility()));
  connect(ui->cbRefani, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateVisibility()));

}

//static QString String(char* c){
//  return QString(c);
//}
static QString String(int i){
  return QString("%1").arg(i);
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

void GuiPanel::setSelection(const QModelIndexList &newsel, int k){
  int sel=-1;
  int nsel = (int)newsel.size();

  bool vertexani=false;
  bool rigged=false;
  bool vertexcolor=false;
  bool manyMaterials=false;
  int flags=-1;
  char materialSt[255]="";

  int nv=0, nf=0, nfr=0, np=0;
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
      if (nfr<k) k=nfr;
      nf += m->face.size();
      if (!materialSt[0]) sprintf(materialSt,"%s",m->material);
      else if (strcmp(materialSt,m->material)) {
        sprintf(materialSt,"%s","<various>");
        manyMaterials=true;
      }
      if (flags==-1) flags=m->flags; else {
        if (flags!=m->flags) flags=-2;
      }
    }


  }


  if (k==-1) k=NONE;






  BrfMesh *m = NULL;
  BrfTexture *tex = NULL;
  BrfAnimation *ani = NULL;



  // set data
  switch (TokenEnum(k)){
    case MESH:
      if (sel>=0 && nsel==1) m=&(data->mesh[sel]);
      ui->boxFlags    ->setText( (flags>=0)?String( flags ):"" );
      ui->boxFlags    ->setReadOnly( flags<0 || !nsel);
      //ui->boxName     ->setText( (m)?String(m->name     ):"" );
      ui->boxMaterial ->setText( materialSt );
      if (!manyMaterials) {
        std::string a( materialSt );
        std::string s  = (*mapMT)[ a ];
        if (!s.length()) s="<not found>";

        ui->boxTexture  ->setText( s.c_str() );
      } else
        ui->boxTexture  ->setText( materialSt );
      ui->boxMaterial ->setReadOnly( manyMaterials || !nsel);
      ui->boxNVerts   ->display( nv );
      ui->boxNFaces   ->display( nf );
      ui->boxNFrames  ->display( nfr);
      ui->boxNPos     ->display( np );
      ui->meshDataAni->setVisible( vertexani);
      ui->viewRefAni->setVisible(rigged);

      ui->rbRiggingcolor->setEnabled( rigged );
      ui->rbVertexcolor->setEnabled(vertexcolor);

      break;
    case TEXTURE:
      if (sel>=0 && nsel==1 && sel<(int)data->texture.size()) tex=&(data->texture[sel]);
      ui->boxTextureFlags ->setText( (tex)?String(tex->flags):"" );
      break;
    case ANIMATION:
      if (sel>=0 && nsel==1 && sel<(int)data->animation.size()) ani=&(data->animation[sel]);
      ui->boxAniNBones->display( (ani)?ani->nbones:0 );
      ui->boxAniNFrames->display( (ani)?(int)ani->frame.size():0 );
      ui->boxAniMinFrame->display( (ani)?ani->FirstIndex():0 );
      ui->boxAniMaxFrame->display( (ani)?ani->LastIndex():0 );

      ui->rbRiggingcolor->setEnabled( true ); // quick: just let user edit them
      ui->rbVertexcolor->setEnabled( true );
    default:

      break;
  }

  ui->animationData->setVisible(k == ANIMATION);
  ui->textureData->setVisible(k == TEXTURE);
  ui->meshData->setVisible(k == MESH);
  ui->generalView->setVisible( k==MESH || k==ANIMATION || k==BODY );

  displaying=k;
  updateVisibility();

}

void GuiPanel::updateVisibility(){

  int k=displaying;


  // set visibility
  if (k==MESH) {
    ui->viewFloor->setVisible(true);

    ui->viewMeshRendering->setVisible(true);
    ui->viewAni->setVisible( (ui->viewRefAni->isVisible() && ui->cbRefani->currentIndex())
                             || ui->meshDataAni->isVisible() );
  } else if (k==ANIMATION) {
    ui->viewFloor->setVisible(false);
    ui->viewRefSkin->setVisible(true);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible( ui->cbSkin->currentIndex() );
    ui->viewAni->setVisible(true);
  } else if (k==BODY) {
    ui->viewFloor->setVisible(true);
    ui->viewRefSkin->setVisible(false);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible(false);
    ui->viewAni->setVisible(false);
  } else {
    ui->viewFloor->setVisible(false);
    ui->viewRefSkin->setVisible(false);
    ui->viewRefAni->setVisible(false);
    ui->viewMeshRendering->setVisible(false);
    ui->viewAni->setVisible(false);
  }



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



