#include "guipanel.h"
#include "ui_guipanel.h"

#include "brfData.h"

GuiPanel::GuiPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuiPanel)
{
  ui->setupUi(this);
}

//static QString String(char* c){
//  return QString(c);
//}
static QString String(int i){
  return QString("%1").arg(i);
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
  ui->meshData->setVisible(k == MESH);
  ui->meshView->setVisible(k == MESH);

  BrfMesh *m = NULL;


  switch (TokenEnum(k)){
    case ANIMATION:
    break;
    case MESH:
      if (sel>=0 && nsel==1) m=&(data->mesh[sel]);
      ui->boxFlags    ->setText( (flags>=0)?String( flags ):"" );
      ui->boxFlags    ->setReadOnly( flags<0 || !nsel);
      //ui->boxName     ->setText( (m)?String(m->name     ):"" );
      ui->boxMaterial ->setText( materialSt );
      ui->boxTexture  ->setText( (manyMaterials)?"<various>":" [todo] " );
      ui->boxMaterial ->setReadOnly( manyMaterials || !nsel);
      ui->boxNVerts   ->display( nv );
      ui->boxNFaces   ->display( nf );
      ui->boxNFrames  ->display( nfr);
      ui->boxNPos     ->display( np );
      ui->meshDataAni->setVisible( vertexani ) ;
      ui->meshViewAni->setVisible( vertexani || rigged ) ;
      ui->rbRiggingcolor->setEnabled( rigged );
      ui->rbVertexcolor->setEnabled(vertexcolor);
      break;
    default:

      break;
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



