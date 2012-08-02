/********************************************************************************
** Form generated from reading ui file 'guipanel.ui'
**
** Created: Tue Aug 11 16:08:36 2009
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
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
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
    QLineEdit *timeOfFrame;
    QLabel *label_4;
    QGroupBox *generalView;
    QVBoxLayout *verticalLayout;
    QWidget *viewRefSkin;
    QLabel *label;
    QComboBox *cbSkin;
    QWidget *viewMeshRendering;
    QRadioButton *rbNocolor;
    QRadioButton *rbVertexcolor;
    QCheckBox *cbLighting;
    QRadioButton *rbRiggingcolor;
    QCheckBox *cbWireframe;
    QCheckBox *cbTexture;
    QWidget *viewFloor;
    QCheckBox *cbFloor;
    QWidget *viewRefAni;
    QComboBox *cbRefani;
    QLabel *label_21;
    QWidget *viewAni;
    QPushButton *buPause;
    QPushButton *buPlay;
    QPushButton *buStop;
    QSpacerItem *verticalSpacer;
    QGroupBox *textureData;
    QLabel *label_9;
    QLineEdit *boxTextureFlags;
    QGroupBox *animationData;
    QLCDNumber *boxAniMinFrame;
    QLCDNumber *boxAniNBones;
    QLCDNumber *boxAniNFrames;
    QLCDNumber *boxAniMaxFrame;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QGroupBox *materialData;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLineEdit *leMatShader;
    QLineEdit *leMatDifA;
    QLineEdit *leMatDifA_2;
    QLineEdit *leMatBump;
    QLineEdit *leMatEnv;

    void setupUi(QWidget *GuiPanel)
    {
        if (GuiPanel->objectName().isEmpty())
            GuiPanel->setObjectName(QString::fromUtf8("GuiPanel"));
        GuiPanel->resize(200, 1120);
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
        label_5->setGeometry(QRect(10, 12, 71, 16));
        frameNumber = new QSpinBox(meshDataAni);
        frameNumber->setObjectName(QString::fromUtf8("frameNumber"));
        frameNumber->setGeometry(QRect(78, 7, 41, 22));
        frameNumber->setFrame(false);
        frameNumber->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timeOfFrame = new QLineEdit(meshDataAni);
        timeOfFrame->setObjectName(QString::fromUtf8("timeOfFrame"));
        timeOfFrame->setGeometry(QRect(130, 7, 41, 21));
        timeOfFrame->setReadOnly(false);
        label_4 = new QLabel(meshDataAni);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(121, 10, 20, 20));
        generalView = new QGroupBox(GuiPanel);
        generalView->setObjectName(QString::fromUtf8("generalView"));
        generalView->setGeometry(QRect(10, 230, 181, 231));
        sizePolicy.setHeightForWidth(generalView->sizePolicy().hasHeightForWidth());
        generalView->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(generalView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        verticalLayout->setContentsMargins(0, 0, 0, -1);
        viewRefSkin = new QWidget(generalView);
        viewRefSkin->setObjectName(QString::fromUtf8("viewRefSkin"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(25);
        sizePolicy1.setHeightForWidth(viewRefSkin->sizePolicy().hasHeightForWidth());
        viewRefSkin->setSizePolicy(sizePolicy1);
        viewRefSkin->setMinimumSize(QSize(0, 25));
        label = new QLabel(viewRefSkin);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 0, 46, 14));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cbSkin = new QComboBox(viewRefSkin);
        cbSkin->setObjectName(QString::fromUtf8("cbSkin"));
        cbSkin->setGeometry(QRect(70, 0, 101, 21));

        verticalLayout->addWidget(viewRefSkin);

        viewMeshRendering = new QWidget(generalView);
        viewMeshRendering->setObjectName(QString::fromUtf8("viewMeshRendering"));
        viewMeshRendering->setMinimumSize(QSize(0, 60));
        viewMeshRendering->setBaseSize(QSize(0, 80));
        rbNocolor = new QRadioButton(viewMeshRendering);
        rbNocolor->setObjectName(QString::fromUtf8("rbNocolor"));
        rbNocolor->setGeometry(QRect(90, 40, 84, 18));
        rbVertexcolor = new QRadioButton(viewMeshRendering);
        rbVertexcolor->setObjectName(QString::fromUtf8("rbVertexcolor"));
        rbVertexcolor->setGeometry(QRect(90, 0, 84, 18));
        rbVertexcolor->setChecked(true);
        cbLighting = new QCheckBox(viewMeshRendering);
        cbLighting->setObjectName(QString::fromUtf8("cbLighting"));
        cbLighting->setGeometry(QRect(10, 20, 72, 18));
        cbLighting->setChecked(true);
        rbRiggingcolor = new QRadioButton(viewMeshRendering);
        rbRiggingcolor->setObjectName(QString::fromUtf8("rbRiggingcolor"));
        rbRiggingcolor->setGeometry(QRect(90, 20, 84, 18));
        cbWireframe = new QCheckBox(viewMeshRendering);
        cbWireframe->setObjectName(QString::fromUtf8("cbWireframe"));
        cbWireframe->setGeometry(QRect(10, 40, 72, 18));
        cbTexture = new QCheckBox(viewMeshRendering);
        cbTexture->setObjectName(QString::fromUtf8("cbTexture"));
        cbTexture->setGeometry(QRect(10, -2, 81, 20));

        verticalLayout->addWidget(viewMeshRendering);

        viewFloor = new QWidget(generalView);
        viewFloor->setObjectName(QString::fromUtf8("viewFloor"));
        viewFloor->setMinimumSize(QSize(0, 20));
        cbFloor = new QCheckBox(viewFloor);
        cbFloor->setObjectName(QString::fromUtf8("cbFloor"));
        cbFloor->setGeometry(QRect(10, 0, 72, 18));
        cbFloor->setChecked(true);

        verticalLayout->addWidget(viewFloor);

        viewRefAni = new QWidget(generalView);
        viewRefAni->setObjectName(QString::fromUtf8("viewRefAni"));
        viewRefAni->setMinimumSize(QSize(0, 25));
        cbRefani = new QComboBox(viewRefAni);
        cbRefani->setObjectName(QString::fromUtf8("cbRefani"));
        cbRefani->setGeometry(QRect(70, 0, 101, 21));
        label_21 = new QLabel(viewRefAni);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(0, 0, 61, 20));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(viewRefAni);

        viewAni = new QWidget(generalView);
        viewAni->setObjectName(QString::fromUtf8("viewAni"));
        viewAni->setMinimumSize(QSize(0, 25));
        buPause = new QPushButton(viewAni);
        buPause->setObjectName(QString::fromUtf8("buPause"));
        buPause->setGeometry(QRect(80, 4, 18, 18));
        QFont font;
        font.setPointSize(7);
        buPause->setFont(font);
        buPlay = new QPushButton(viewAni);
        buPlay->setObjectName(QString::fromUtf8("buPlay"));
        buPlay->setGeometry(QRect(60, 4, 18, 18));
        QFont font1;
        font1.setPointSize(8);
        buPlay->setFont(font1);
        buStop = new QPushButton(viewAni);
        buStop->setObjectName(QString::fromUtf8("buStop"));
        buStop->setGeometry(QRect(100, 4, 18, 18));
        buStop->setFont(font);

        verticalLayout->addWidget(viewAni);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(5, 1);
        textureData = new QGroupBox(GuiPanel);
        textureData->setObjectName(QString::fromUtf8("textureData"));
        textureData->setGeometry(QRect(10, 810, 181, 71));
        label_9 = new QLabel(textureData);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(0, 30, 51, 20));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTextureFlags = new QLineEdit(textureData);
        boxTextureFlags->setObjectName(QString::fromUtf8("boxTextureFlags"));
        boxTextureFlags->setGeometry(QRect(60, 30, 71, 21));
        boxTextureFlags->setReadOnly(true);
        animationData = new QGroupBox(GuiPanel);
        animationData->setObjectName(QString::fromUtf8("animationData"));
        animationData->setGeometry(QRect(10, 680, 181, 111));
        boxAniMinFrame = new QLCDNumber(animationData);
        boxAniMinFrame->setObjectName(QString::fromUtf8("boxAniMinFrame"));
        boxAniMinFrame->setGeometry(QRect(70, 80, 41, 21));
        boxAniNBones = new QLCDNumber(animationData);
        boxAniNBones->setObjectName(QString::fromUtf8("boxAniNBones"));
        boxAniNBones->setGeometry(QRect(70, 20, 41, 21));
        boxAniNFrames = new QLCDNumber(animationData);
        boxAniNFrames->setObjectName(QString::fromUtf8("boxAniNFrames"));
        boxAniNFrames->setGeometry(QRect(70, 50, 41, 21));
        boxAniMaxFrame = new QLCDNumber(animationData);
        boxAniMaxFrame->setObjectName(QString::fromUtf8("boxAniMaxFrame"));
        boxAniMaxFrame->setGeometry(QRect(130, 80, 41, 21));
        label_14 = new QLabel(animationData);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 20, 51, 20));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(animationData);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 50, 51, 20));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(animationData);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 80, 51, 20));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_17 = new QLabel(animationData);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(111, 80, 20, 20));
        label_17->setAlignment(Qt::AlignCenter);
        materialData = new QGroupBox(GuiPanel);
        materialData->setObjectName(QString::fromUtf8("materialData"));
        materialData->setGeometry(QRect(10, 930, 181, 181));
        label_12 = new QLabel(materialData);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 30, 46, 14));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_13 = new QLabel(materialData);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 60, 46, 14));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_18 = new QLabel(materialData);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 90, 46, 14));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(materialData);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(10, 120, 46, 14));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_20 = new QLabel(materialData);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 150, 46, 14));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leMatShader = new QLineEdit(materialData);
        leMatShader->setObjectName(QString::fromUtf8("leMatShader"));
        leMatShader->setGeometry(QRect(60, 30, 113, 20));
        leMatDifA = new QLineEdit(materialData);
        leMatDifA->setObjectName(QString::fromUtf8("leMatDifA"));
        leMatDifA->setGeometry(QRect(60, 60, 113, 20));
        leMatDifA_2 = new QLineEdit(materialData);
        leMatDifA_2->setObjectName(QString::fromUtf8("leMatDifA_2"));
        leMatDifA_2->setGeometry(QRect(60, 90, 113, 20));
        leMatBump = new QLineEdit(materialData);
        leMatBump->setObjectName(QString::fromUtf8("leMatBump"));
        leMatBump->setGeometry(QRect(60, 120, 113, 20));
        leMatEnv = new QLineEdit(materialData);
        leMatEnv->setObjectName(QString::fromUtf8("leMatEnv"));
        leMatEnv->setGeometry(QRect(60, 150, 113, 20));

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
        frameNumber->setSuffix(QString());
        frameNumber->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        timeOfFrame->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        timeOfFrame->setText(QString());
        label_4->setText(QApplication::translate("GuiPanel", ":", 0, QApplication::UnicodeUTF8));
        generalView->setTitle(QApplication::translate("GuiPanel", "View", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GuiPanel", "Skin:", 0, QApplication::UnicodeUTF8));
        rbNocolor->setText(QApplication::translate("GuiPanel", "No Color", 0, QApplication::UnicodeUTF8));
        rbVertexcolor->setText(QApplication::translate("GuiPanel", "Vertex Color", 0, QApplication::UnicodeUTF8));
        cbLighting->setText(QApplication::translate("GuiPanel", "Lighting", 0, QApplication::UnicodeUTF8));
        rbRiggingcolor->setText(QApplication::translate("GuiPanel", "Rigging Color", 0, QApplication::UnicodeUTF8));
        cbWireframe->setText(QApplication::translate("GuiPanel", "WireFrame", 0, QApplication::UnicodeUTF8));
        cbTexture->setText(QApplication::translate("GuiPanel", "Texture", 0, QApplication::UnicodeUTF8));
        cbFloor->setText(QApplication::translate("GuiPanel", "Floor", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("GuiPanel", "Animation:", 0, QApplication::UnicodeUTF8));
        buPause->setText(QApplication::translate("GuiPanel", "||", 0, QApplication::UnicodeUTF8));
        buPlay->setText(QApplication::translate("GuiPanel", ">", 0, QApplication::UnicodeUTF8));
        buStop->setText(QApplication::translate("GuiPanel", "[]", 0, QApplication::UnicodeUTF8));
        textureData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxTextureFlags->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        boxTextureFlags->setInputMask(QApplication::translate("GuiPanel", "000000000000; ", 0, QApplication::UnicodeUTF8));
        boxTextureFlags->setText(QString());
        animationData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("GuiPanel", "# bones:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("GuiPanel", "# frames:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("GuiPanel", "interval:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("GuiPanel", "-", 0, QApplication::UnicodeUTF8));
        materialData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("GuiPanel", "Shader:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("GuiPanel", "DiffuseA:", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("GuiPanel", "DiffuseB:", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("GuiPanel", "Bump:", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("GuiPanel", "Enviro:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(GuiPanel);
    } // retranslateUi

};

namespace Ui {
    class GuiPanel: public Ui_GuiPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPANEL_H
