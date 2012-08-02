/********************************************************************************
** Form generated from reading ui file 'askTexturenameDialog.ui'
**
** Created: Mon Aug 17 18:27:30 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKTEXTURENAMEDIALOG_H
#define UI_ASKTEXTURENAMEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_askTexturenameDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *pushButton;
    QCheckBox *checkBox;

    void setupUi(QDialog *askTexturenameDialog)
    {
        if (askTexturenameDialog->objectName().isEmpty())
            askTexturenameDialog->setObjectName(QString::fromUtf8("askTexturenameDialog"));
        askTexturenameDialog->resize(273, 142);
        buttonBox = new QDialogButtonBox(askTexturenameDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(-130, 100, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(askTexturenameDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(70, 40, 121, 20));
        label = new QLabel(askTexturenameDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(16, 45, 46, 14));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton = new QPushButton(askTexturenameDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(200, 40, 51, 23));
        checkBox = new QCheckBox(askTexturenameDialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(70, 70, 181, 18));

        retranslateUi(askTexturenameDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), askTexturenameDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), askTexturenameDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(askTexturenameDialog);
    } // setupUi

    void retranslateUi(QDialog *askTexturenameDialog)
    {
        askTexturenameDialog->setWindowTitle(QApplication::translate("askTexturenameDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("askTexturenameDialog", "name:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("askTexturenameDialog", "Browse", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("askTexturenameDialog", "also add a new Texture", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(askTexturenameDialog);
    } // retranslateUi

};

namespace Ui {
    class askTexturenameDialog: public Ui_askTexturenameDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKTEXTURENAMEDIALOG_H
