#include "askSkelDialog.h"
#include "ui_askSkelDialog.h"

#include "brfSkeleton.h"

AskSkelDialog::AskSkelDialog(QWidget *parent, const std::vector<BrfSkeleton> &sv) :
    QDialog(parent),
    m_ui(new Ui::AskSkelDialog)
{
  m_ui->setupUi(this);

  for (int i=0; i<(int)sv.size(); i++)
  {
    m_ui->cbSkelFrom->addItem( sv[i].name );
    m_ui->cbSkelTo->addItem( sv[i].name );
  }
  m_ui->cbSkelFrom->setCurrentIndex(0);
  m_ui->cbSkelTo->setCurrentIndex(1);

}

int AskSkelDialog::getSkelFrom() const{
  return m_ui->cbSkelFrom->currentIndex();
}
int AskSkelDialog::getSkelTo() const{
  return m_ui->cbSkelTo->currentIndex();
}
bool AskSkelDialog::asFrame() const{
  return m_ui->checkBox->isChecked();
}

AskSkelDialog::~AskSkelDialog()
{
    delete m_ui;
}

void AskSkelDialog::changeEvent(QEvent *e)
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
