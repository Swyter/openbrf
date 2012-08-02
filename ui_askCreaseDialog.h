/********************************************************************************
** Form generated from reading ui file 'askCreaseDialog.ui'
**
** Created: Thu Sep 16 22:03:42 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKCREASEDIALOG_H
#define UI_ASKCREASEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>

QT_BEGIN_NAMESPACE

class Ui_AskCreaseDialog
{
public:
    QDialogButtonBox *buttonBox;
    QSlider *slider;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QCheckBox *seamsCB;

    void setupUi(QDialog *AskCreaseDialog)
    {
        if (AskCreaseDialog->objectName().isEmpty())
            AskCreaseDialog->setObjectName(QString::fromUtf8("AskCreaseDialog"));
        AskCreaseDialog->resize(141, 319);
        buttonBox = new QDialogButtonBox(AskCreaseDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 280, 101, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        buttonBox->setCenterButtons(true);
        slider = new QSlider(AskCreaseDialog);
        slider->setObjectName(QString::fromUtf8("slider"));
        slider->setGeometry(QRect(60, 70, 21, 131));
        slider->setMaximum(100);
        slider->setValue(50);
        slider->setOrientation(Qt::Vertical);
        label = new QLabel(AskCreaseDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 7, 121, 31));
        label_2 = new QLabel(AskCreaseDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(26, 50, 91, 20));
        QFont font;
        font.setItalic(true);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(AskCreaseDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 204, 101, 21));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);
        seamsCB = new QCheckBox(AskCreaseDialog);
        seamsCB->setObjectName(QString::fromUtf8("seamsCB"));
        seamsCB->setGeometry(QRect(30, 230, 111, 41));

        retranslateUi(AskCreaseDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskCreaseDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskCreaseDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskCreaseDialog);
    } // setupUi

    void retranslateUi(QDialog *AskCreaseDialog)
    {
        AskCreaseDialog->setWindowTitle(QApplication::translate("AskCreaseDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AskCreaseDialog", "Recomputing normals:\n"
"how many hard edges?", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AskCreaseDialog", "all edges soft", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AskCreaseDialog", "all edges hard", 0, QApplication::UnicodeUTF8));
        seamsCB->setText(QApplication::translate("AskCreaseDialog", "keep texture\n"
"seams hard", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskCreaseDialog);
    } // retranslateUi

};

namespace Ui {
    class AskCreaseDialog: public Ui_AskCreaseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKCREASEDIALOG_H
