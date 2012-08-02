/********************************************************************************
** Form generated from reading ui file 'guipanel.ui'
**
** Created: Sat Aug 1 15:58:37 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GUIPANEL_H
#define UI_GUIPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GuiPanel
{
public:
    QGroupBox *meshData;
    QLineEdit *boxMaterial;
    QLabel *label_2;
    QLCDNumber *boxNVerts;
    QLineEdit *boxFlags;
    QLabel *label_3;
    QLineEdit *boxTexture;
    QLabel *label_6;
    QLabel *label_7;
    QLCDNumber *boxNFaces;
    QLabel *label_8;
    QLCDNumber *boxNPos;
    QLabel *label_10;
    QLCDNumber *boxNFrames;
    QLabel *label_11;
    QWidget *meshDataAni;
    QLabel *label_5;
    QSpinBox *frameNumber;
    QLineEdit *boxFlags_2;
    QLabel *label_4;
    QGroupBox *meshView;
    QCheckBox *cbTexture;
    QCheckBox *cbWireframe;
    QRadioButton *rbVertexcolor;
    QRadioButton *rbRiggingcolor;
    QRadioButton *rbNocolor;
    QCheckBox *cbLighting;
    QWidget *meshViewAni;
    QComboBox *comboBox;
    QPushButton *buPause;
    QPushButton *buPlay;
    QPushButton *buStop;

    void setupUi(QWidget *GuiPanel)
    {
        if (GuiPanel->objectName().isEmpty())
            GuiPanel->setObjectName(QString::fromUtf8("GuiPanel"));
        GuiPanel->resize(200, 504);
        GuiPanel->setMinimumSize(QSize(200, 0));
        GuiPanel->setMaximumSize(QSize(200, 16777215));
        meshData = new QGroupBox(GuiPanel);
        meshData->setObjectName(QString::fromUtf8("meshData"));
        meshData->setGeometry(QRect(10, 20, 181, 201));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(meshData->sizePolicy().hasHeightForWidth());
        meshData->setSizePolicy(sizePolicy);
        meshData->setFlat(false);
        boxMaterial = new QLineEdit(meshData);
        boxMaterial->setObjectName(QString::fromUtf8("boxMaterial"));
        boxMaterial->setGeometry(QRect(60, 20, 111, 20));
        label_2 = new QLabel(meshData);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(5, 20, 51, 20));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNVerts = new QLCDNumber(meshData);
        boxNVerts->setObjectName(QString::fromUtf8("boxNVerts"));
        boxNVerts->setGeometry(QRect(3, 130, 42, 24));
        boxNVerts->setNumDigits(4);
        boxNVerts->setSegmentStyle(QLCDNumber::Filled);
        boxFlags = new QLineEdit(meshData);
        boxFlags->setObjectName(QString::fromUtf8("boxFlags"));
        boxFlags->setGeometry(QRect(60, 80, 71, 21));
        boxFlags->setReadOnly(true);
        label_3 = new QLabel(meshData);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(5, 80, 51, 20));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTexture = new QLineEdit(meshData);
        boxTexture->setObjectName(QString::fromUtf8("boxTexture"));
        boxTexture->setEnabled(false);
        boxTexture->setGeometry(QRect(60, 50, 111, 20));
        boxTexture->setReadOnly(true);
        label_6 = new QLabel(meshData);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(5, 50, 51, 20));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label_7 = new QLabel(meshData);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(16, 110, 31, 20));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNFaces = new QLCDNumber(meshData);
        boxNFaces->setObjectName(QString::fromUtf8("boxNFaces"));
        boxNFaces->setGeometry(QRect(47, 130, 42, 24));
        boxNFaces->setNumDigits(4);
        boxNFaces->setSegmentStyle(QLCDNumber::Filled);
        label_8 = new QLabel(meshData);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 110, 41, 20));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNPos = new QLCDNumber(meshData);
        boxNPos->setObjectName(QString::fromUtf8("boxNPos"));
        boxNPos->setGeometry(QRect(93, 130, 42, 24));
        boxNPos->setNumDigits(4);
        boxNPos->setSegmentStyle(QLCDNumber::Filled);
        label_10 = new QLabel(meshData);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(93, 110, 41, 20));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNFrames = new QLCDNumber(meshData);
        boxNFrames->setObjectName(QString::fromUtf8("boxNFrames"));
        boxNFrames->setGeometry(QRect(136, 130, 42, 24));
        boxNFrames->setNumDigits(4);
        boxNFrames->setSegmentStyle(QLCDNumber::Filled);
        label_11 = new QLabel(meshData);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(136, 110, 41, 20));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        meshDataAni = new QWidget(meshData);
        meshDataAni->setObjectName(QString::fromUtf8("meshDataAni"));
        meshDataAni->setGeometry(QRect(0, 160, 181, 41));
        label_5 = new QLabel(meshDataAni);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 10, 71, 16));
        frameNumber = new QSpinBox(meshDataAni);
        frameNumber->setObjectName(QString::fromUtf8("frameNumber"));
        frameNumber->setGeometry(QRect(80, 7, 40, 22));
        frameNumber->setFrame(false);
        frameNumber->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxFlags_2 = new QLineEdit(meshDataAni);
        boxFlags_2->setObjectName(QString::fromUtf8("boxFlags_2"));
        boxFlags_2->setGeometry(QRect(130, 7, 41, 21));
        boxFlags_2->setReadOnly(true);
        label_4 = new QLabel(meshDataAni);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(121, 10, 20, 20));
        boxMaterial->raise();
        label_2->raise();
        boxNVerts->raise();
        boxFlags->raise();
        label_3->raise();
        boxTexture->raise();
        label_6->raise();
        label_7->raise();
        boxNFaces->raise();
        label_8->raise();
        boxNPos->raise();
        label_10->raise();
        boxNFrames->raise();
        label_11->raise();
        meshDataAni->raise();
        meshView = new QGroupBox(GuiPanel);
        meshView->setObjectName(QString::fromUtf8("meshView"));
        meshView->setGeometry(QRect(10, 280, 180, 141));
        sizePolicy.setHeightForWidth(meshView->sizePolicy().hasHeightForWidth());
        meshView->setSizePolicy(sizePolicy);
        cbTexture = new QCheckBox(meshView);
        cbTexture->setObjectName(QString::fromUtf8("cbTexture"));
        cbTexture->setGeometry(QRect(10, 30, 72, 18));
        cbTexture->setCheckable(false);
        cbWireframe = new QCheckBox(meshView);
        cbWireframe->setObjectName(QString::fromUtf8("cbWireframe"));
        cbWireframe->setGeometry(QRect(10, 70, 72, 18));
        rbVertexcolor = new QRadioButton(meshView);
        rbVertexcolor->setObjectName(QString::fromUtf8("rbVertexcolor"));
        rbVertexcolor->setGeometry(QRect(90, 30, 84, 18));
        rbVertexcolor->setChecked(true);
        rbRiggingcolor = new QRadioButton(meshView);
        rbRiggingcolor->setObjectName(QString::fromUtf8("rbRiggingcolor"));
        rbRiggingcolor->setGeometry(QRect(90, 50, 84, 18));
        rbNocolor = new QRadioButton(meshView);
        rbNocolor->setObjectName(QString::fromUtf8("rbNocolor"));
        rbNocolor->setGeometry(QRect(90, 70, 84, 18));
        cbLighting = new QCheckBox(meshView);
        cbLighting->setObjectName(QString::fromUtf8("cbLighting"));
        cbLighting->setGeometry(QRect(10, 50, 72, 18));
        cbLighting->setChecked(true);
        meshViewAni = new QWidget(meshView);
        meshViewAni->setObjectName(QString::fromUtf8("meshViewAni"));
        meshViewAni->setGeometry(QRect(0, 100, 181, 31));
        comboBox = new QComboBox(meshViewAni);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 10, 71, 21));
        buPause = new QPushButton(meshViewAni);
        buPause->setObjectName(QString::fromUtf8("buPause"));
        buPause->setGeometry(QRect(120, 10, 18, 18));
        QFont font;
        font.setPointSize(7);
        buPause->setFont(font);
        buPlay = new QPushButton(meshViewAni);
        buPlay->setObjectName(QString::fromUtf8("buPlay"));
        buPlay->setGeometry(QRect(100, 10, 18, 18));
        QFont font1;
        font1.setPointSize(8);
        buPlay->setFont(font1);
        buStop = new QPushButton(meshViewAni);
        buStop->setObjectName(QString::fromUtf8("buStop"));
        buStop->setGeometry(QRect(140, 10, 18, 18));
        buStop->setFont(font);
        meshData->raise();
        meshView->raise();
        rbRiggingcolor->raise();

        retranslateUi(GuiPanel);

        QMetaObject::connectSlotsByName(GuiPanel);
    } // setupUi

    void retranslateUi(QWidget *GuiPanel)
    {
        GuiPanel->setWindowTitle(QApplication::translate("GuiPanel", "Form", 0, QApplication::UnicodeUTF8));
        meshData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        boxMaterial->setText(QString());
        label_2->setText(QApplication::translate("GuiPanel", "Material:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNVerts->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        boxFlags->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        boxFlags->setInputMask(QApplication::translate("GuiPanel", "000000000000; ", 0, QApplication::UnicodeUTF8));
        boxFlags->setText(QString());
        label_3->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxTexture->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        boxTexture->setText(QString());
        label_6->setText(QApplication::translate("GuiPanel", "Texture:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("GuiPanel", "verts:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNFaces->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_8->setText(QApplication::translate("GuiPanel", "faces:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNPos->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("GuiPanel", "pos:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNFrames->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_11->setText(QApplication::translate("GuiPanel", "frames:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("GuiPanel", "Time of frame", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxFlags_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        boxFlags_2->setText(QString());
        label_4->setText(QApplication::translate("GuiPanel", ":", 0, QApplication::UnicodeUTF8));
        meshView->setTitle(QApplication::translate("GuiPanel", "View", 0, QApplication::UnicodeUTF8));
        cbTexture->setText(QApplication::translate("GuiPanel", "Texture", 0, QApplication::UnicodeUTF8));
        cbWireframe->setText(QApplication::translate("GuiPanel", "WireFrame", 0, QApplication::UnicodeUTF8));
        rbVertexcolor->setText(QApplication::translate("GuiPanel", "Vertex Color", 0, QApplication::UnicodeUTF8));
        rbRiggingcolor->setText(QApplication::translate("GuiPanel", "Rigging Color", 0, QApplication::UnicodeUTF8));
        rbNocolor->setText(QApplication::translate("GuiPanel", "No Color", 0, QApplication::UnicodeUTF8));
        cbLighting->setText(QApplication::translate("GuiPanel", "Lighting", 0, QApplication::UnicodeUTF8));
        buPause->setText(QApplication::translate("GuiPanel", "||", 0, QApplication::UnicodeUTF8));
        buPlay->setText(QApplication::translate("GuiPanel", ">", 0, QApplication::UnicodeUTF8));
        buStop->setText(QApplication::translate("GuiPanel", "[]", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(GuiPanel);
    } // retranslateUi

};

namespace Ui {
    class GuiPanel: public Ui_GuiPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPANEL_H
