/********************************************************************************
** Form generated from reading ui file 'askBoneDialog.ui'
**
** Created: Thu Sep 16 22:03:42 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKBONEDIALOG_H
#define UI_ASKBONEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_AskBoneDialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *cbSlel;
    QComboBox *cbBone;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLabel *label_5;

    void setupUi(QDialog *AskBoneDialog)
    {
        if (AskBoneDialog->objectName().isEmpty())
            AskBoneDialog->setObjectName(QString::fromUtf8("AskBoneDialog"));
        AskBoneDialog->resize(312, 209);
        buttonBox = new QDialogButtonBox(AskBoneDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(-100, 170, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        cbSlel = new QComboBox(AskBoneDialog);
        cbSlel->setObjectName(QString::fromUtf8("cbSlel"));
        cbSlel->setGeometry(QRect(140, 60, 161, 21));
        cbBone = new QComboBox(AskBoneDialog);
        cbBone->setObjectName(QString::fromUtf8("cbBone"));
        cbBone->setGeometry(QRect(140, 90, 161, 21));
        label = new QLabel(AskBoneDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 60, 120, 20));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(AskBoneDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 90, 120, 20));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(AskBoneDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 10, 191, 21));
        QFont font;
        font.setItalic(true);
        label_3->setFont(font);
        label_4 = new QLabel(AskBoneDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 26, 261, 21));
        label_4->setFont(font);
        radioButton = new QRadioButton(AskBoneDialog);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(140, 124, 84, 20));
        radioButton_2 = new QRadioButton(AskBoneDialog);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(140, 141, 161, 18));
        label_5 = new QLabel(AskBoneDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(-1, 124, 131, 20));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(AskBoneDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskBoneDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskBoneDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskBoneDialog);
    } // setupUi

    void retranslateUi(QDialog *AskBoneDialog)
    {
        AskBoneDialog->setWindowTitle(QApplication::translate("AskBoneDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AskBoneDialog", "Reference skeleton:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AskBoneDialog", "Bone:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AskBoneDialog", "This mesh is not rigged:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AskBoneDialog", "Select a skeleton and a bone to attach this mesh to.", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("AskBoneDialog", "at origin", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("AskBoneDialog", "in the correct final position", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AskBoneDialog", "Piece currently centered:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskBoneDialog);
    } // retranslateUi

};

namespace Ui {
    class AskBoneDialog: public Ui_AskBoneDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKBONEDIALOG_H
