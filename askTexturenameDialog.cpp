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

QStringList AskTexturenameDialog::getRes() const{
  QStringList res;
  res =  m_ui->lineEdit->text().split(",", QString::SkipEmptyParts);
  for (int i=0; i<res.size(); i++){
    res[i] = res[i].trimmed().replace("\"","");
  }
  return res;
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
  QStringList fileName =QFileDialog::getOpenFileNames(
    this,
    tr("Select a texture file") ,
    path,
    QString("Direct Draw Texture (*.dds)")
  );

  if (!fileName.isEmpty()) {
    for (int j=0; j<fileName.size(); j++) {
      fileName[j] = QString("\"%1\"").arg(QFileInfo(fileName[j]).baseName());
    }
    m_ui->lineEdit->setText(fileName.join(", ") );
  }
}
