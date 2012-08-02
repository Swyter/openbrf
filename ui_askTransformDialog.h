/********************************************************************************
** Form generated from reading ui file 'askTransformDialog.ui'
**
** Created: Thu Oct 22 07:36:57 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKTRANSFORMDIALOG_H
#define UI_ASKTRANSFORMDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_AskTransformDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QSpinBox *rotx;
    QSpinBox *roty;
    QSpinBox *rotz;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *groupBox_2;
    QDoubleSpinBox *trax;
    QDoubleSpinBox *tray;
    QDoubleSpinBox *traz;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QGroupBox *groupBox_3;
    QSpinBox *scx;
    QSpinBox *scy;
    QSpinBox *scz;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QCheckBox *checkBox;

    void setupUi(QDialog *AskTransformDialog)
    {
        if (AskTransformDialog->objectName().isEmpty())
            AskTransformDialog->setObjectName(QString::fromUtf8("AskTransformDialog"));
        AskTransformDialog->resize(121, 392);
        buttonBox = new QDialogButtonBox(AskTransformDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 330, 101, 61));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(AskTransformDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 110, 101, 91));
        rotx = new QSpinBox(groupBox);
        rotx->setObjectName(QString::fromUtf8("rotx"));
        rotx->setGeometry(QRect(22, 20, 70, 22));
        rotx->setWrapping(true);
        rotx->setMinimum(-180);
        rotx->setMaximum(180);
        rotx->setSingleStep(15);
        roty = new QSpinBox(groupBox);
        roty->setObjectName(QString::fromUtf8("roty"));
        roty->setGeometry(QRect(22, 40, 70, 22));
        roty->setWrapping(true);
        roty->setMinimum(-180);
        roty->setMaximum(180);
        roty->setSingleStep(15);
        rotz = new QSpinBox(groupBox);
        rotz->setObjectName(QString::fromUtf8("rotz"));
        rotz->setGeometry(QRect(22, 60, 70, 22));
        rotz->setWrapping(true);
        rotz->setMinimum(-180);
        rotz->setMaximum(180);
        rotz->setSingleStep(15);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(-2, 21, 21, 16));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(-2, 41, 21, 16));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(-2, 61, 21, 16));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_2 = new QGroupBox(AskTransformDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 101, 91));
        trax = new QDoubleSpinBox(groupBox_2);
        trax->setObjectName(QString::fromUtf8("trax"));
        trax->setGeometry(QRect(22, 20, 70, 22));
        trax->setWrapping(true);
        trax->setDecimals(4);
        trax->setMinimum(-1000);
        trax->setMaximum(1000);
        trax->setSingleStep(0.25);
        tray = new QDoubleSpinBox(groupBox_2);
        tray->setObjectName(QString::fromUtf8("tray"));
        tray->setGeometry(QRect(22, 40, 70, 22));
        tray->setWrapping(true);
        tray->setDecimals(4);
        tray->setMinimum(-1000);
        tray->setMaximum(1000);
        tray->setSingleStep(0.25);
        traz = new QDoubleSpinBox(groupBox_2);
        traz->setObjectName(QString::fromUtf8("traz"));
        traz->setGeometry(QRect(22, 60, 70, 22));
        traz->setWrapping(true);
        traz->setDecimals(4);
        traz->setMinimum(-1000);
        traz->setMaximum(1000);
        traz->setSingleStep(0.25);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(-2, 21, 21, 16));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(-2, 41, 21, 16));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(-2, 61, 21, 16));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(AskTransformDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 210, 101, 111));
        scx = new QSpinBox(groupBox_3);
        scx->setObjectName(QString::fromUtf8("scx"));
        scx->setGeometry(QRect(22, 20, 70, 22));
        scx->setWrapping(true);
        scx->setMinimum(5);
        scx->setMaximum(1000);
        scx->setSingleStep(5);
        scx->setValue(100);
        scy = new QSpinBox(groupBox_3);
        scy->setObjectName(QString::fromUtf8("scy"));
        scy->setEnabled(false);
        scy->setGeometry(QRect(22, 40, 70, 22));
        scy->setWrapping(true);
        scy->setMinimum(5);
        scy->setMaximum(1000);
        scy->setSingleStep(5);
        scy->setValue(100);
        scz = new QSpinBox(groupBox_3);
        scz->setObjectName(QString::fromUtf8("scz"));
        scz->setEnabled(false);
        scz->setGeometry(QRect(22, 60, 70, 22));
        scz->setWrapping(true);
        scz->setMinimum(5);
        scz->setMaximum(1000);
        scz->setSingleStep(5);
        scz->setValue(100);
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(-2, 21, 21, 16));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(-2, 41, 21, 16));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(-2, 61, 21, 16));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(24, 86, 72, 18));
        checkBox->setChecked(true);

        retranslateUi(AskTransformDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskTransformDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskTransformDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskTransformDialog);
    } // setupUi

    void retranslateUi(QDialog *AskTransformDialog)
    {
        AskTransformDialog->setWindowTitle(QApplication::translate("AskTransformDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AskTransformDialog", "Rotation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AskTransformDialog", "X:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AskTransformDialog", "Y:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AskTransformDialog", "Z:", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AskTransformDialog", "Translation", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AskTransformDialog", "X:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AskTransformDialog", "Y:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("AskTransformDialog", "Z:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("AskTransformDialog", "Scale %", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("AskTransformDialog", "X:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("AskTransformDialog", "Y:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("AskTransformDialog", "Z:", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("AskTransformDialog", "Uniform", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskTransformDialog);
    } // retranslateUi

};

namespace Ui {
    class AskTransformDialog: public Ui_AskTransformDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKTRANSFORMDIALOG_H
