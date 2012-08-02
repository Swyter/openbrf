/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include "askUnrefTextureDialog.h"
#include "ui_askUnrefTextureDialog.h"

AskUnrefTextureDialog::AskUnrefTextureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskUnrefTextureDialog)
{
    ui->setupUi(this);
    //ui->listWidget->setEnabled(false);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(refresh()));
}

void AskUnrefTextureDialog::refresh(){
  accept();
}

AskUnrefTextureDialog::~AskUnrefTextureDialog()
{
    delete ui;
}

void AskUnrefTextureDialog::addFile(QString f){
  ui->listWidget->addItem(f);
}

void AskUnrefTextureDialog::changeEvent(QEvent *e)
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
