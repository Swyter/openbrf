/********************************************************************************
** Form generated from reading ui file 'askModErrorDialog.ui'
**
** Created: Thu Aug 5 18:22:49 2010
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKMODERRORDIALOG_H
#define UI_ASKMODERRORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_askModErrorDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QLineEdit *lineEdit;
    QLabel *label;
    QWidget *searchOpt;
    QComboBox *comboBox;
    QCheckBox *checkBox;
    QTextBrowser *textBrowser;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *askModErrorDialog)
    {
    if (askModErrorDialog->objectName().isEmpty())
        askModErrorDialog->setObjectName(QString::fromUtf8("askModErrorDialog"));
    askModErrorDialog->resize(710, 250);
    verticalLayout = new QVBoxLayout(askModErrorDialog);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    widget = new QWidget(askModErrorDialog);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setMinimumSize(QSize(0, 20));
    lineEdit = new QLineEdit(widget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setGeometry(QRect(141, 0, 301, 20));
    label = new QLabel(widget);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(4, 2, 131, 16));
    label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(widget);

    searchOpt = new QWidget(askModErrorDialog);
    searchOpt->setObjectName(QString::fromUtf8("searchOpt"));
    searchOpt->setMinimumSize(QSize(0, 20));
    comboBox = new QComboBox(searchOpt);
    comboBox->setObjectName(QString::fromUtf8("comboBox"));
    comboBox->setGeometry(QRect(142, 0, 121, 20));
    checkBox = new QCheckBox(searchOpt);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));
    checkBox->setGeometry(QRect(310, 0, 131, 20));

    verticalLayout->addWidget(searchOpt);

    textBrowser = new QTextBrowser(askModErrorDialog);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

    verticalLayout->addWidget(textBrowser);

    buttonBox = new QDialogButtonBox(askModErrorDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);

    verticalLayout->addWidget(buttonBox);


    retranslateUi(askModErrorDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), askModErrorDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), askModErrorDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(askModErrorDialog);
    } // setupUi

    void retranslateUi(QDialog *askModErrorDialog)
    {
    askModErrorDialog->setWindowTitle(QApplication::translate("askModErrorDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("askModErrorDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("askModErrorDialog", "look in commonres too", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(askModErrorDialog);
    } // retranslateUi

};

namespace Ui {
    class askModErrorDialog: public Ui_askModErrorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKMODERRORDIALOG_H
