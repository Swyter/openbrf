/********************************************************************************
** Form generated from reading ui file 'askErrorsDialog.ui'
**
** Created: Fri Sep 11 19:36:52 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKERRORSDIALOG_H
#define UI_ASKERRORSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_AskErrorsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *AskErrorsDialog)
    {
        if (AskErrorsDialog->objectName().isEmpty())
            AskErrorsDialog->setObjectName(QString::fromUtf8("AskErrorsDialog"));
        AskErrorsDialog->resize(393, 284);
        buttonBox = new QDialogButtonBox(AskErrorsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textBrowser = new QTextBrowser(AskErrorsDialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(20, 20, 351, 211));

        retranslateUi(AskErrorsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskErrorsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskErrorsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskErrorsDialog);
    } // setupUi

    void retranslateUi(QDialog *AskErrorsDialog)
    {
        AskErrorsDialog->setWindowTitle(QApplication::translate("AskErrorsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskErrorsDialog);
    } // retranslateUi

};

namespace Ui {
    class AskErrorsDialog: public Ui_AskErrorsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKERRORSDIALOG_H
