#include "askBoneDialog.h"
#include "ui_askBoneDialog.h"

#include "brfSkeleton.h"

AskBoneDialog::AskBoneDialog(QWidget *parent,const std::vector<BrfSkeleton> &s) :
    QDialog(parent),
    //sv(s),
    ui(new Ui::AskBoneDialog)

{
  sv=s;
  ui->setupUi(this);

  for (int i=0; i<(int)sv.size(); i++)
    ui->cbSlel->addItem( sv[i].name );
  selectSkel(0);
  connect(ui->cbSlel, SIGNAL(currentIndexChanged(int)), this, SLOT(selectSkel(int)) );
}

void AskBoneDialog::sayNotRigged(bool say){
  ui->label_3->setVisible(say);
}

void AskBoneDialog::selectSkel(int i){
  ui->cbBone->clear();
  for (unsigned int j=0; j<sv[i].bone.size(); j++)
    ui->cbBone->addItem( sv[i].bone[j].name );
}

int AskBoneDialog::getSkel() const {
  return ui->cbSlel->currentIndex();
}

int AskBoneDialog::getBone() const {
  return ui->cbBone->currentIndex();
}

AskBoneDialog::~AskBoneDialog()
{
    delete ui;
}

void AskBoneDialog::changeEvent(QEvent *e)
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
