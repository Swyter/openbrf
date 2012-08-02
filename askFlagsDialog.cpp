#include "askFlagsDialog.h"
#include "ui_askFlagsDialog.h"

#include<qtgui>

AskFlagsDialog::AskFlagsDialog(QWidget *parent, unsigned int ones, unsigned int zeros, QStringList l) :
    QDialog(parent),
    m_ui(new Ui::AskFlagsDialog)
{
  assert (l.size()==32);
  m_ui->setupUi(this);
  m_ui->widget->setLayout(new QVBoxLayout(m_ui->widget));
  m_ui->widget_2->setLayout(new QVBoxLayout(m_ui->widget_2));
  m_ui->widget_3->setLayout(new QVBoxLayout(m_ui->widget_3));
  m_ui->widget_4->setLayout(new QVBoxLayout(m_ui->widget_4));
  for (unsigned int i=0; i<32; i++) {
    cb[i]=new QCheckBox(l[i], this);
    QWidget *w;
    if (i<8) w=m_ui->widget;
    else if (i<16) w=m_ui->widget_2;
    else if (i<24) w=m_ui->widget_3;
    else w=m_ui->widget_4;

    w->layout()->addWidget(cb[i]);
    unsigned int one = 1;
    bool isOne = ones & (one<<i);
    bool isZero = zeros & (one<<i);
    if (isOne==isZero) {
      cb[i]->setTristate(false);
      cb[i]->setChecked(isOne);
    }
    else {
      cb[i]->setTristate(true);
      cb[i]->setCheckState(Qt::PartiallyChecked);
    }

    if (l[i].isEmpty()) {
      cb[i]->setText("unused?");
      QFont f = cb[i]->font();
      f.setItalic(true);
      cb[i]->setFont(f);

      if (!isOne && !isZero) cb[i]->setVisible(false);
    }
    if (l[i]==QString("R")) {
      cb[i]->setText("reserved");
      cb[i]->setEnabled(false);
      QFont f = cb[i]->font();
      f.setItalic(true);
      cb[i]->setFont(f);
      cb[i]->setVisible(false);
    }
  }
  m_ui->widget  ->layout()->addItem(new QSpacerItem(0,0, QSizePolicy::Minimum,QSizePolicy::Expanding));
  m_ui->widget_2->layout()->addItem(new QSpacerItem(0,0, QSizePolicy::Minimum,QSizePolicy::Expanding));
  m_ui->widget_3->layout()->addItem(new QSpacerItem(0,0, QSizePolicy::Minimum,QSizePolicy::Expanding));
  m_ui->widget_4->layout()->addItem(new QSpacerItem(0,0, QSizePolicy::Minimum,QSizePolicy::Expanding));
  //m_ui->widget_2->adjustSize();
  //m_ui->widget_3->adjustSize();
  //m_ui->widget_4->adjustSize();

}
unsigned int AskFlagsDialog::toOne() const{
  unsigned int one = 1, res=0;
  for (int i=0; i<32; i++)
    if (cb[i]->checkState()==Qt::Checked) res|=(one<<i);
  return res;
}
unsigned int AskFlagsDialog::toZero() const{
  unsigned int one = 1, res=0xFFFFFFFF;
  for (int i=0; i<32; i++)
    if (cb[i]->checkState()==Qt::Unchecked) res&=~(one<<i);
  return res;
}


AskFlagsDialog::~AskFlagsDialog()
{
  for (int i=0; i<32; i++) if (cb[i]) delete cb[i];
  delete m_ui;
}

void AskFlagsDialog::changeEvent(QEvent *e)
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

void AskFlagsDialog::on_pushButton_clicked()
{
    for (int i=0; i<32; i++) cb[i]->setVisible(true);
}
