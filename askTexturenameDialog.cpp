#include "askTexturenameDialog.h"
#include "ui_askTexturenameDialog.h"
#include <QtGui>

bool AskTexturenameDialog::lastAlsoAdd=false;

AskTexturenameDialog::AskTexturenameDialog(QWidget *parent, bool letAlsoAdd) :
    QDialog(parent),
    m_ui(new Ui::askTexturenameDialog)
{
    m_ui->setupUi(this);
    m_ui->buttonBox->setVisible(false);
    m_ui->checkBox->setChecked(lastAlsoAdd);
    m_ui->checkBox->setVisible(letAlsoAdd);
}

bool AskTexturenameDialog::alsoAdd(){
  return lastAlsoAdd=m_ui->checkBox->isChecked();
}

QString AskTexturenameDialog::getRes() const{
  return m_ui->lineEdit->text();
}

AskTexturenameDialog::~AskTexturenameDialog()
{
    delete m_ui;
}

void AskTexturenameDialog::setDef(QString s){
  m_ui->lineEdit->setText(s);
}
void AskTexturenameDialog::setLabel(QString s){
  m_ui->label->setText(s);
}

void AskTexturenameDialog::setBrowsable(QString s){
  path = s;
  m_ui->buttonBox->setVisible(true);
}
void AskTexturenameDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AskTexturenameDialog::on_pushButton_clicked()
{
  QString fileName =QFileDialog::getOpenFileName(
    this,
    tr("Select a texture file") ,
    path,
    QString("Direct Draw Texture (*.dds)")
  );
  if (!fileName.isEmpty())
    m_ui->lineEdit->setText( QFileInfo(fileName).baseName() );
}
