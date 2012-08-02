/********************************************************************************
** Form generated from reading ui file 'askNewUiPictureDialog.ui'
**
** Created: Mon Sep 20 01:05:49 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKNEWUIPICTUREDIALOG_H
#define UI_ASKNEWUIPICTUREDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AskNewUiPictureDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label_5;
    QGroupBox *groupBox;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *posX;
    QLineEdit *posY;
    QLineEdit *sizeX;
    QLineEdit *sizeY;
    QLabel *label_4;
    QLabel *label_3;
    QPushButton *setActualPixels;
    QPushButton *setCenter;
    QRadioButton *measurePixel;
    QRadioButton *measurePerc;
    QPushButton *setFullScreen;
    QGroupBox *groupBox_2;
    QPushButton *browse;
    QLineEdit *NameBox;
    QGroupBox *groupBox_3;
    QRadioButton *overlayDarken;
    QRadioButton *overlayNormal;
    QRadioButton *overlaySubstitute;
    QCheckBox *replaceOpt;

    void setupUi(QDialog *AskNewUiPictureDialog)
    {
        if (AskNewUiPictureDialog->objectName().isEmpty())
            AskNewUiPictureDialog->setObjectName(QString::fromUtf8("AskNewUiPictureDialog"));
        AskNewUiPictureDialog->resize(342, 418);
        buttonBox = new QDialogButtonBox(AskNewUiPictureDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(210, 350, 121, 51));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_5 = new QLabel(AskNewUiPictureDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(9, 9, 239, 39));
        groupBox = new QGroupBox(AskNewUiPictureDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 120, 321, 181));
        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 20, 191, 131));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        posX = new QLineEdit(widget);
        posX->setObjectName(QString::fromUtf8("posX"));

        gridLayout_2->addWidget(posX, 1, 1, 1, 1);

        posY = new QLineEdit(widget);
        posY->setObjectName(QString::fromUtf8("posY"));

        gridLayout_2->addWidget(posY, 2, 1, 1, 1);

        sizeX = new QLineEdit(widget);
        sizeX->setObjectName(QString::fromUtf8("sizeX"));

        gridLayout_2->addWidget(sizeX, 1, 2, 1, 1);

        sizeY = new QLineEdit(widget);
        sizeY->setObjectName(QString::fromUtf8("sizeY"));

        gridLayout_2->addWidget(sizeY, 2, 2, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout_2->addWidget(label_4, 0, 2, 1, 1);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 1, 1, 1);

        setActualPixels = new QPushButton(widget);
        setActualPixels->setObjectName(QString::fromUtf8("setActualPixels"));

        gridLayout_2->addWidget(setActualPixels, 3, 2, 1, 1);

        setCenter = new QPushButton(widget);
        setCenter->setObjectName(QString::fromUtf8("setCenter"));

        gridLayout_2->addWidget(setCenter, 3, 1, 1, 1);

        measurePixel = new QRadioButton(groupBox);
        measurePixel->setObjectName(QString::fromUtf8("measurePixel"));
        measurePixel->setGeometry(QRect(210, 85, 111, 31));
        measurePerc = new QRadioButton(groupBox);
        measurePerc->setObjectName(QString::fromUtf8("measurePerc"));
        measurePerc->setGeometry(QRect(210, 65, 111, 17));
        setFullScreen = new QPushButton(groupBox);
        setFullScreen->setObjectName(QString::fromUtf8("setFullScreen"));
        setFullScreen->setGeometry(QRect(35, 150, 158, 23));
        groupBox_2 = new QGroupBox(AskNewUiPictureDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 60, 321, 51));
        browse = new QPushButton(groupBox_2);
        browse->setObjectName(QString::fromUtf8("browse"));
        browse->setGeometry(QRect(230, 19, 81, 23));
        NameBox = new QLineEdit(groupBox_2);
        NameBox->setObjectName(QString::fromUtf8("NameBox"));
        NameBox->setGeometry(QRect(20, 20, 201, 20));
        groupBox_3 = new QGroupBox(AskNewUiPictureDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(9, 310, 151, 91));
        overlayDarken = new QRadioButton(groupBox_3);
        overlayDarken->setObjectName(QString::fromUtf8("overlayDarken"));
        overlayDarken->setGeometry(QRect(20, 30, 131, 17));
        overlayNormal = new QRadioButton(groupBox_3);
        overlayNormal->setObjectName(QString::fromUtf8("overlayNormal"));
        overlayNormal->setGeometry(QRect(20, 50, 141, 17));
        overlaySubstitute = new QRadioButton(groupBox_3);
        overlaySubstitute->setObjectName(QString::fromUtf8("overlaySubstitute"));
        overlaySubstitute->setGeometry(QRect(20, 70, 141, 17));
        replaceOpt = new QCheckBox(AskNewUiPictureDialog);
        replaceOpt->setObjectName(QString::fromUtf8("replaceOpt"));
        replaceOpt->setGeometry(QRect(210, 310, 121, 31));

        retranslateUi(AskNewUiPictureDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskNewUiPictureDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskNewUiPictureDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskNewUiPictureDialog);
    } // setupUi

    void retranslateUi(QDialog *AskNewUiPictureDialog)
    {
        AskNewUiPictureDialog->setWindowTitle(QApplication::translate("AskNewUiPictureDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AskNewUiPictureDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-style:italic;\">This will create: a Mesh, a Material, and a Texture</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-style:italic;\">which can be used ingame as a 2D decoration </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-style:italic;\">in the background of menus.</span></p>"
                        "</body></html>", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AskNewUiPictureDialog", "Size and Pos:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AskNewUiPictureDialog", "X:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AskNewUiPictureDialog", "Y:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AskNewUiPictureDialog", "Size:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AskNewUiPictureDialog", "Lower-left\n"
"corner: ", 0, QApplication::UnicodeUTF8));
        setActualPixels->setText(QApplication::translate("AskNewUiPictureDialog", "Actual pixels", 0, QApplication::UnicodeUTF8));
        setCenter->setText(QApplication::translate("AskNewUiPictureDialog", "Center", 0, QApplication::UnicodeUTF8));
        measurePixel->setText(QApplication::translate("AskNewUiPictureDialog", "in pixels, on a\n"
"1024x768 sceen", 0, QApplication::UnicodeUTF8));
        measurePerc->setText(QApplication::translate("AskNewUiPictureDialog", "in % of screen", 0, QApplication::UnicodeUTF8));
        setFullScreen->setText(QApplication::translate("AskNewUiPictureDialog", "Set fullscreen", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AskNewUiPictureDialog", "Picture name:", 0, QApplication::UnicodeUTF8));
        browse->setText(QApplication::translate("AskNewUiPictureDialog", "Browse", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("AskNewUiPictureDialog", "Overlay mode:", 0, QApplication::UnicodeUTF8));
        overlayDarken->setText(QApplication::translate("AskNewUiPictureDialog", "Darken (multiply)", 0, QApplication::UnicodeUTF8));
        overlayNormal->setText(QApplication::translate("AskNewUiPictureDialog", "Normal (alpha blend)", 0, QApplication::UnicodeUTF8));
        overlaySubstitute->setText(QApplication::translate("AskNewUiPictureDialog", "Substitute (no alpha)", 0, QApplication::UnicodeUTF8));
        replaceOpt->setText(QApplication::translate("AskNewUiPictureDialog", "replace existing Mat.\n"
"Mesh, and Texture", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskNewUiPictureDialog);
    } // retranslateUi

};

namespace Ui {
    class AskNewUiPictureDialog: public Ui_AskNewUiPictureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKNEWUIPICTUREDIALOG_H
