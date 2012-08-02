/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include "askTransformDialog.h"
#include "ui_askTransformDialog.h"

#include <math.h>
#include <vcg/space/point3.h>
#include <vcg/math/matrix44.h>

AskTransformDialog::AskTransformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskTransformDialog)
{
    ui->setupUi(this);
    connect(ui->rotx,SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(ui->roty,SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(ui->rotz,SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(ui->trax,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->tray,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->traz,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->scx,SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(ui->scy,SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(ui->scz,SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(update()));

}

void AskTransformDialog::reset(){
  ui->scx->setValue(100);
  ui->scy->setValue(100);
  ui->scz->setValue(100);
  ui->trax->setValue(0);
  ui->tray->setValue(0);
  ui->traz->setValue(0);
  ui->rotx->setValue(0);
  ui->roty->setValue(0);
  ui->rotz->setValue(0);
}

static float toRad(float t) {return t*M_PI/180;}

void AskTransformDialog::update(){
  if (ui->checkBox->isChecked()) {
    ui->scy->blockSignals(true);
    ui->scz->blockSignals(true);
    ui->scy->setValue( ui->scx->value());
    ui->scz->setValue( ui->scx->value());
    ui->scy->blockSignals(false);
    ui->scz->blockSignals(false);
  }
  ui->scy->setEnabled( !ui->checkBox->isChecked() );
  ui->scz->setEnabled( !ui->checkBox->isChecked() );

  vcg::Matrix44f res;
  vcg::Matrix44f t;
  vcg::Matrix44f sc;
  res.FromEulerAngles(toRad( ui->rotx->value() ),toRad( ui->roty->value() ),toRad( ui->rotz->value() ));
  t.SetTranslate(ui->trax->value(),ui->tray->value(),ui->traz->value());
  sc.SetScale(vcg::Point3f(ui->scx->value(),ui->scy->value(),ui->scz->value())/100.0f);
  res = t*res*sc;


  for (int i=0,x=0; x<4; x++)
    for (int y=0; y<4; y++,i++)
      matrix[i]=res[y][x];

  emit changed();
}

AskTransformDialog::~AskTransformDialog()
{
  delete ui;
}

void AskTransformDialog::changeEvent(QEvent *e)
{

    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
