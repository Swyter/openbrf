/********************************************************************************
** Form generated from reading ui file 'askSkelDialog.ui'
**
** Created: Fri Aug 14 21:49:36 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKSKELDIALOG_H
#define UI_ASKSKELDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AskSkelDialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *cbSkelFrom;
    QComboBox *cbSkelTo;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *checkBox;

    void setupUi(QDialog *AskSkelDialog)
    {
        if (AskSkelDialog->objectName().isEmpty())
            AskSkelDialog->setObjectName(QString::fromUtf8("AskSkelDialog"));
        AskSkelDialog->resize(320, 222);
        buttonBox = new QDialogButtonBox(AskSkelDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(90, 180, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        cbSkelFrom = new QComboBox(AskSkelDialog);
        cbSkelFrom->setObjectName(QString::fromUtf8("cbSkelFrom"));
        cbSkelFrom->setGeometry(QRect(150, 60, 151, 22));
        cbSkelTo = new QComboBox(AskSkelDialog);
        cbSkelTo->setObjectName(QString::fromUtf8("cbSkelTo"));
        cbSkelTo->setGeometry(QRect(150, 90, 151, 22));
        label = new QLabel(AskSkelDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 60, 121, 21));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(AskSkelDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 90, 131, 21));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(AskSkelDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 12, 301, 16));
        QFont font;
        font.setItalic(true);
        label_3->setFont(font);
        label_4 = new QLabel(AskSkelDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 26, 301, 16));
        label_4->setFont(font);
        checkBox = new QCheckBox(AskSkelDialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(50, 140, 221, 21));

        retranslateUi(AskSkelDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskSkelDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskSkelDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskSkelDialog);
    } // setupUi

    void retranslateUi(QDialog *AskSkelDialog)
    {
        AskSkelDialog->setWindowTitle(QApplication::translate("AskSkelDialog", "Dialog", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cbSkelFrom->setAccessibleName(QApplication::translate("AskSkelDialog", "cbSkelTo", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label->setText(QApplication::translate("AskSkelDialog", "From (current skeleton):", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AskSkelDialog", "To (destination skeleton):", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AskSkelDialog", "Change geometry of a mesh currently tigged for a skeleton", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AskSkelDialog", "to make it follow a new skeleton:", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("AskSkelDialog", "result in a new vertex-ani frame", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskSkelDialog);
    } // retranslateUi

};

namespace Ui {
    class AskSkelDialog: public Ui_AskSkelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKSKELDIALOG_H
