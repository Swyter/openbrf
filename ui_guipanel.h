/********************************************************************************
** Form generated from reading ui file 'guipanel.ui'
**
** Created: Mon Aug 17 15:35:23 2009
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
#include <QtGui/QListView>
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
    QWidget *viewRefSkel;
    QLabel *label_23;
    QComboBox *cbRefSkel;
    QWidget *viewAni;
    QPushButton *buPause;
    QPushButton *buPlay;
    QPushButton *buStop;
    QPushButton *buStepF;
    QPushButton *buStepB;
    QWidget *viewShowAlpha;
    QRadioButton *rbAlphaNo;
    QRadioButton *rbAlphaTransparent;
    QRadioButton *rbAlphaColor;
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
    QLineEdit *leMatDifB;
    QLineEdit *leMatBump;
    QLineEdit *leMatEnv;
    QLineEdit *leMatR;
    QLineEdit *leMatG;
    QLineEdit *leMatB;
    QLabel *label_22;
    QLineEdit *leMatSpec;
    QLabel *label_24;
    QLabel *label_25;
    QLineEdit *leMatCoeff;
    QLineEdit *leMatFlags;
    QLabel *label_26;
    QGroupBox *skeletonData;
    QLabel *label_27;
    QLCDNumber *boxSkelNBones;
    QGroupBox *shaderData;
    QLabel *label_28;
    QLineEdit *leShaderTechnique;
    QLabel *label_29;
    QLineEdit *leShaderFallback;
    QLabel *label_30;
    QLineEdit *leShaderFlags;
    QLineEdit *leShaderRequires;
    QLabel *label_31;
    QGroupBox *groupBox;
    QLabel *label_33;
    QLineEdit *leShaderTaMap;
    QLineEdit *leShaderTaColorOp;
    QLineEdit *leShaderTaAlphaOp;
    QLineEdit *leShaderTaFlags;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QListView *lvTextAcc;
    QGroupBox *bodyData;
    QListView *lvBodyPart;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QWidget *wiBodyAxisA;
    QLineEdit *leBodyAZ;
    QLineEdit *leBodyAY;
    QLineEdit *leBodyAX;
    QLabel *labelAxisRadius;
    QWidget *wiBodyAxisB;
    QLabel *label_40;
    QLineEdit *leBodyBZ;
    QLineEdit *leBodyBY;
    QLineEdit *leBodyBX;
    QWidget *wiBodyRadius;
    QLabel *label_41;
    QLineEdit *leBodyRad;
    QWidget *wiBodyNVerts;
    QLCDNumber *leBodyNVert;
    QLabel *label_43;
    QWidget *wiBodyNFaces;
    QLCDNumber *leBodyNFace;
    QLabel *label_42;
    QWidget *wiBodyFlags;
    QLabel *label_44;
    QLineEdit *leBodyFlags;
    QWidget *wiBodySigns;
    QLabel *label_45;
    QLineEdit *leBodySign;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_37;

    void setupUi(QWidget *GuiPanel)
    {
        if (GuiPanel->objectName().isEmpty())
            GuiPanel->setObjectName(QString::fromUtf8("GuiPanel"));
        GuiPanel->resize(200, 1685);
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
        boxNVerts->setGeometry(QRect(47, 130, 42, 24));
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
        boxNFaces->setGeometry(QRect(3, 130, 42, 24));
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
        generalView->setGeometry(QRect(10, 230, 181, 201));
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
        label->setGeometry(QRect(5, 0, 61, 20));
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
        label_21->setGeometry(QRect(5, 0, 61, 20));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(viewRefAni);

        viewRefSkel = new QWidget(generalView);
        viewRefSkel->setObjectName(QString::fromUtf8("viewRefSkel"));
        viewRefSkel->setMinimumSize(QSize(0, 25));
        label_23 = new QLabel(viewRefSkel);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(5, 0, 61, 20));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cbRefSkel = new QComboBox(viewRefSkel);
        cbRefSkel->setObjectName(QString::fromUtf8("cbRefSkel"));
        cbRefSkel->setGeometry(QRect(70, 0, 101, 21));

        verticalLayout->addWidget(viewRefSkel);

        viewAni = new QWidget(generalView);
        viewAni->setObjectName(QString::fromUtf8("viewAni"));
        viewAni->setMinimumSize(QSize(0, 25));
        buPause = new QPushButton(viewAni);
        buPause->setObjectName(QString::fromUtf8("buPause"));
        buPause->setGeometry(QRect(60, 4, 18, 18));
        QFont font;
        font.setPointSize(7);
        buPause->setFont(font);
        buPlay = new QPushButton(viewAni);
        buPlay->setObjectName(QString::fromUtf8("buPlay"));
        buPlay->setGeometry(QRect(38, 4, 18, 18));
        QFont font1;
        font1.setPointSize(8);
        buPlay->setFont(font1);
        buStop = new QPushButton(viewAni);
        buStop->setObjectName(QString::fromUtf8("buStop"));
        buStop->setGeometry(QRect(80, 4, 18, 18));
        buStop->setFont(font);
        buStepF = new QPushButton(viewAni);
        buStepF->setObjectName(QString::fromUtf8("buStepF"));
        buStepF->setGeometry(QRect(130, 4, 18, 18));
        buStepF->setFont(font1);
        buStepB = new QPushButton(viewAni);
        buStepB->setObjectName(QString::fromUtf8("buStepB"));
        buStepB->setGeometry(QRect(110, 4, 18, 18));
        buStepB->setFont(font1);

        verticalLayout->addWidget(viewAni);

        viewShowAlpha = new QWidget(generalView);
        viewShowAlpha->setObjectName(QString::fromUtf8("viewShowAlpha"));
        viewShowAlpha->setMinimumSize(QSize(0, 60));
        rbAlphaNo = new QRadioButton(viewShowAlpha);
        rbAlphaNo->setObjectName(QString::fromUtf8("rbAlphaNo"));
        rbAlphaNo->setGeometry(QRect(10, 0, 84, 18));
        rbAlphaNo->setChecked(true);
        rbAlphaTransparent = new QRadioButton(viewShowAlpha);
        rbAlphaTransparent->setObjectName(QString::fromUtf8("rbAlphaTransparent"));
        rbAlphaTransparent->setGeometry(QRect(10, 40, 131, 18));
        rbAlphaColor = new QRadioButton(viewShowAlpha);
        rbAlphaColor->setObjectName(QString::fromUtf8("rbAlphaColor"));
        rbAlphaColor->setGeometry(QRect(10, 20, 84, 18));

        verticalLayout->addWidget(viewShowAlpha);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(7, 1);
        textureData = new QGroupBox(GuiPanel);
        textureData->setObjectName(QString::fromUtf8("textureData"));
        textureData->setGeometry(QRect(10, 810, 181, 71));
        label_9 = new QLabel(textureData);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(0, 30, 51, 20));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTextureFlags = new QLineEdit(textureData);
        boxTextureFlags->setObjectName(QString::fromUtf8("boxTextureFlags"));
        boxTextureFlags->setGeometry(QRect(60, 30, 113, 20));
        boxTextureFlags->setReadOnly(true);
        animationData = new QGroupBox(GuiPanel);
        animationData->setObjectName(QString::fromUtf8("animationData"));
        animationData->setGeometry(QRect(10, 1200, 181, 111));
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
        materialData->setGeometry(QRect(10, 900, 181, 251));
        label_12 = new QLabel(materialData);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 35, 46, 14));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_13 = new QLabel(materialData);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 65, 46, 14));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_18 = new QLabel(materialData);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 85, 46, 14));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(materialData);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(10, 105, 46, 14));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_20 = new QLabel(materialData);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 125, 46, 14));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leMatShader = new QLineEdit(materialData);
        leMatShader->setObjectName(QString::fromUtf8("leMatShader"));
        leMatShader->setGeometry(QRect(60, 30, 113, 20));
        leMatDifA = new QLineEdit(materialData);
        leMatDifA->setObjectName(QString::fromUtf8("leMatDifA"));
        leMatDifA->setGeometry(QRect(60, 60, 113, 20));
        leMatDifB = new QLineEdit(materialData);
        leMatDifB->setObjectName(QString::fromUtf8("leMatDifB"));
        leMatDifB->setGeometry(QRect(60, 80, 113, 20));
        leMatBump = new QLineEdit(materialData);
        leMatBump->setObjectName(QString::fromUtf8("leMatBump"));
        leMatBump->setGeometry(QRect(60, 100, 113, 20));
        leMatEnv = new QLineEdit(materialData);
        leMatEnv->setObjectName(QString::fromUtf8("leMatEnv"));
        leMatEnv->setGeometry(QRect(60, 120, 113, 20));
        leMatR = new QLineEdit(materialData);
        leMatR->setObjectName(QString::fromUtf8("leMatR"));
        leMatR->setGeometry(QRect(60, 170, 37, 20));
        leMatG = new QLineEdit(materialData);
        leMatG->setObjectName(QString::fromUtf8("leMatG"));
        leMatG->setGeometry(QRect(97, 170, 37, 20));
        leMatB = new QLineEdit(materialData);
        leMatB->setObjectName(QString::fromUtf8("leMatB"));
        leMatB->setGeometry(QRect(134, 170, 37, 20));
        label_22 = new QLabel(materialData);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(-5, 175, 61, 14));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leMatSpec = new QLineEdit(materialData);
        leMatSpec->setObjectName(QString::fromUtf8("leMatSpec"));
        leMatSpec->setGeometry(QRect(60, 140, 113, 20));
        label_24 = new QLabel(materialData);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(5, 145, 51, 14));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_25 = new QLabel(materialData);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(-5, 195, 61, 14));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leMatCoeff = new QLineEdit(materialData);
        leMatCoeff->setObjectName(QString::fromUtf8("leMatCoeff"));
        leMatCoeff->setGeometry(QRect(60, 190, 37, 20));
        leMatFlags = new QLineEdit(materialData);
        leMatFlags->setObjectName(QString::fromUtf8("leMatFlags"));
        leMatFlags->setGeometry(QRect(60, 220, 111, 20));
        label_26 = new QLabel(materialData);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(15, 225, 41, 14));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        skeletonData = new QGroupBox(GuiPanel);
        skeletonData->setObjectName(QString::fromUtf8("skeletonData"));
        skeletonData->setGeometry(QRect(10, 1150, 181, 51));
        label_27 = new QLabel(skeletonData);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(10, 20, 51, 20));
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxSkelNBones = new QLCDNumber(skeletonData);
        boxSkelNBones->setObjectName(QString::fromUtf8("boxSkelNBones"));
        boxSkelNBones->setGeometry(QRect(70, 20, 41, 21));
        shaderData = new QGroupBox(GuiPanel);
        shaderData->setObjectName(QString::fromUtf8("shaderData"));
        shaderData->setGeometry(QRect(10, 530, 181, 271));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(shaderData->sizePolicy().hasHeightForWidth());
        shaderData->setSizePolicy(sizePolicy2);
        label_28 = new QLabel(shaderData);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(5, 35, 51, 14));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leShaderTechnique = new QLineEdit(shaderData);
        leShaderTechnique->setObjectName(QString::fromUtf8("leShaderTechnique"));
        leShaderTechnique->setGeometry(QRect(60, 30, 113, 20));
        label_29 = new QLabel(shaderData);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(5, 95, 51, 14));
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leShaderFallback = new QLineEdit(shaderData);
        leShaderFallback->setObjectName(QString::fromUtf8("leShaderFallback"));
        leShaderFallback->setGeometry(QRect(60, 90, 113, 20));
        label_30 = new QLabel(shaderData);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(5, 125, 51, 14));
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leShaderFlags = new QLineEdit(shaderData);
        leShaderFlags->setObjectName(QString::fromUtf8("leShaderFlags"));
        leShaderFlags->setGeometry(QRect(60, 120, 113, 20));
        leShaderFlags->setReadOnly(true);
        leShaderRequires = new QLineEdit(shaderData);
        leShaderRequires->setObjectName(QString::fromUtf8("leShaderRequires"));
        leShaderRequires->setGeometry(QRect(60, 60, 113, 20));
        label_31 = new QLabel(shaderData);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(6, 65, 51, 14));
        label_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox = new QGroupBox(shaderData);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(50, 150, 121, 111));
        label_33 = new QLabel(groupBox);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(16, 20, 31, 20));
        label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leShaderTaMap = new QLineEdit(groupBox);
        leShaderTaMap->setObjectName(QString::fromUtf8("leShaderTaMap"));
        leShaderTaMap->setGeometry(QRect(50, 20, 61, 20));
        leShaderTaColorOp = new QLineEdit(groupBox);
        leShaderTaColorOp->setObjectName(QString::fromUtf8("leShaderTaColorOp"));
        leShaderTaColorOp->setGeometry(QRect(50, 40, 61, 20));
        leShaderTaAlphaOp = new QLineEdit(groupBox);
        leShaderTaAlphaOp->setObjectName(QString::fromUtf8("leShaderTaAlphaOp"));
        leShaderTaAlphaOp->setGeometry(QRect(50, 60, 61, 20));
        leShaderTaFlags = new QLineEdit(groupBox);
        leShaderTaFlags->setObjectName(QString::fromUtf8("leShaderTaFlags"));
        leShaderTaFlags->setGeometry(QRect(50, 80, 61, 20));
        label_34 = new QLabel(groupBox);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(-3, 40, 51, 20));
        label_34->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_35 = new QLabel(groupBox);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(-3, 60, 51, 20));
        label_35->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_36 = new QLabel(groupBox);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(-4, 80, 51, 20));
        label_36->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lvTextAcc = new QListView(shaderData);
        lvTextAcc->setObjectName(QString::fromUtf8("lvTextAcc"));
        lvTextAcc->setGeometry(QRect(10, 170, 31, 81));
        lvTextAcc->setContextMenuPolicy(Qt::CustomContextMenu);
        bodyData = new QGroupBox(GuiPanel);
        bodyData->setObjectName(QString::fromUtf8("bodyData"));
        bodyData->setGeometry(QRect(10, 1310, 181, 231));
        lvBodyPart = new QListView(bodyData);
        lvBodyPart->setObjectName(QString::fromUtf8("lvBodyPart"));
        lvBodyPart->setGeometry(QRect(70, 20, 101, 71));
        lvBodyPart->setContextMenuPolicy(Qt::CustomContextMenu);
        lvBodyPart->setSelectionRectVisible(false);
        groupBox_2 = new QGroupBox(bodyData);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 161, 131));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 5, 0, 0);
        wiBodyAxisA = new QWidget(groupBox_2);
        wiBodyAxisA->setObjectName(QString::fromUtf8("wiBodyAxisA"));
        wiBodyAxisA->setMinimumSize(QSize(0, 25));
        leBodyAZ = new QLineEdit(wiBodyAxisA);
        leBodyAZ->setObjectName(QString::fromUtf8("leBodyAZ"));
        leBodyAZ->setGeometry(QRect(119, 5, 37, 20));
        leBodyAZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyAY = new QLineEdit(wiBodyAxisA);
        leBodyAY->setObjectName(QString::fromUtf8("leBodyAY"));
        leBodyAY->setGeometry(QRect(82, 5, 37, 20));
        leBodyAY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyAX = new QLineEdit(wiBodyAxisA);
        leBodyAX->setObjectName(QString::fromUtf8("leBodyAX"));
        leBodyAX->setGeometry(QRect(45, 5, 37, 20));
        leBodyAX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelAxisRadius = new QLabel(wiBodyAxisA);
        labelAxisRadius->setObjectName(QString::fromUtf8("labelAxisRadius"));
        labelAxisRadius->setGeometry(QRect(-20, 10, 61, 14));
        labelAxisRadius->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(wiBodyAxisA);

        wiBodyAxisB = new QWidget(groupBox_2);
        wiBodyAxisB->setObjectName(QString::fromUtf8("wiBodyAxisB"));
        wiBodyAxisB->setMinimumSize(QSize(0, 25));
        label_40 = new QLabel(wiBodyAxisB);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(-20, 5, 61, 14));
        label_40->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyBZ = new QLineEdit(wiBodyAxisB);
        leBodyBZ->setObjectName(QString::fromUtf8("leBodyBZ"));
        leBodyBZ->setGeometry(QRect(119, 0, 37, 20));
        leBodyBZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyBY = new QLineEdit(wiBodyAxisB);
        leBodyBY->setObjectName(QString::fromUtf8("leBodyBY"));
        leBodyBY->setGeometry(QRect(82, 0, 37, 20));
        leBodyBY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyBX = new QLineEdit(wiBodyAxisB);
        leBodyBX->setObjectName(QString::fromUtf8("leBodyBX"));
        leBodyBX->setGeometry(QRect(45, 0, 37, 20));
        leBodyBX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(wiBodyAxisB);

        wiBodyRadius = new QWidget(groupBox_2);
        wiBodyRadius->setObjectName(QString::fromUtf8("wiBodyRadius"));
        wiBodyRadius->setMinimumSize(QSize(0, 30));
        label_41 = new QLabel(wiBodyRadius);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(-20, 10, 61, 14));
        label_41->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyRad = new QLineEdit(wiBodyRadius);
        leBodyRad->setObjectName(QString::fromUtf8("leBodyRad"));
        leBodyRad->setGeometry(QRect(45, 5, 61, 20));

        verticalLayout_2->addWidget(wiBodyRadius);

        wiBodyNVerts = new QWidget(groupBox_2);
        wiBodyNVerts->setObjectName(QString::fromUtf8("wiBodyNVerts"));
        wiBodyNVerts->setMinimumSize(QSize(0, 30));
        leBodyNVert = new QLCDNumber(wiBodyNVerts);
        leBodyNVert->setObjectName(QString::fromUtf8("leBodyNVert"));
        leBodyNVert->setGeometry(QRect(50, 4, 41, 21));
        label_43 = new QLabel(wiBodyNVerts);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(-10, 4, 51, 20));
        label_43->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(wiBodyNVerts);

        wiBodyNFaces = new QWidget(groupBox_2);
        wiBodyNFaces->setObjectName(QString::fromUtf8("wiBodyNFaces"));
        wiBodyNFaces->setMinimumSize(QSize(0, 30));
        leBodyNFace = new QLCDNumber(wiBodyNFaces);
        leBodyNFace->setObjectName(QString::fromUtf8("leBodyNFace"));
        leBodyNFace->setGeometry(QRect(50, 5, 41, 20));
        label_42 = new QLabel(wiBodyNFaces);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(-10, 4, 51, 20));
        label_42->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(wiBodyNFaces);

        wiBodyFlags = new QWidget(groupBox_2);
        wiBodyFlags->setObjectName(QString::fromUtf8("wiBodyFlags"));
        wiBodyFlags->setMinimumSize(QSize(0, 30));
        label_44 = new QLabel(wiBodyFlags);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(-20, 10, 61, 14));
        label_44->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodyFlags = new QLineEdit(wiBodyFlags);
        leBodyFlags->setObjectName(QString::fromUtf8("leBodyFlags"));
        leBodyFlags->setGeometry(QRect(45, 5, 61, 20));

        verticalLayout_2->addWidget(wiBodyFlags);

        wiBodySigns = new QWidget(groupBox_2);
        wiBodySigns->setObjectName(QString::fromUtf8("wiBodySigns"));
        wiBodySigns->setMinimumSize(QSize(0, 30));
        label_45 = new QLabel(wiBodySigns);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(-20, 10, 61, 14));
        label_45->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leBodySign = new QLineEdit(wiBodySigns);
        leBodySign->setObjectName(QString::fromUtf8("leBodySign"));
        leBodySign->setGeometry(QRect(50, 5, 41, 20));

        verticalLayout_2->addWidget(wiBodySigns);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        label_37 = new QLabel(bodyData);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(10, 20, 51, 20));
        label_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

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
        label_7->setText(QApplication::translate("GuiPanel", "faces:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNFaces->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_8->setText(QApplication::translate("GuiPanel", "vert:", 0, QApplication::UnicodeUTF8));
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
        label_23->setText(QApplication::translate("GuiPanel", "Skeleton:", 0, QApplication::UnicodeUTF8));
        buPause->setText(QApplication::translate("GuiPanel", "||", 0, QApplication::UnicodeUTF8));
        buPlay->setText(QApplication::translate("GuiPanel", ">", 0, QApplication::UnicodeUTF8));
        buStop->setText(QApplication::translate("GuiPanel", "[]", 0, QApplication::UnicodeUTF8));
        buStepF->setText(QApplication::translate("GuiPanel", ">|", 0, QApplication::UnicodeUTF8));
        buStepB->setText(QApplication::translate("GuiPanel", "|<", 0, QApplication::UnicodeUTF8));
        rbAlphaNo->setText(QApplication::translate("GuiPanel", "&RGB", 0, QApplication::UnicodeUTF8));
        rbAlphaTransparent->setText(QApplication::translate("GuiPanel", "Alpha &Transparency", 0, QApplication::UnicodeUTF8));
        rbAlphaColor->setText(QApplication::translate("GuiPanel", "&Alpha", 0, QApplication::UnicodeUTF8));
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
        label_22->setText(QApplication::translate("GuiPanel", "Spec RGB:", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("GuiPanel", "Specmap:", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("GuiPanel", "Coeff:", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
        skeletonData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("GuiPanel", "# bones:", 0, QApplication::UnicodeUTF8));
        shaderData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("GuiPanel", "Technique:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        leShaderTechnique->setWhatsThis(QApplication::translate("GuiPanel", "Technique: name of the \"technique\" inside mb.fx file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        leShaderTechnique->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        leShaderTechnique->setAccessibleDescription(QApplication::translate("GuiPanel", "Technique: name of the \"technique\" inside mb.fx file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_29->setText(QApplication::translate("GuiPanel", "Fallback:", 0, QApplication::UnicodeUTF8));
        leShaderFallback->setText(QString());
        label_30->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leShaderFlags->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        leShaderFlags->setInputMask(QApplication::translate("GuiPanel", "000000000000; ", 0, QApplication::UnicodeUTF8));
        leShaderFlags->setText(QString());
        leShaderRequires->setText(QString());
        label_31->setText(QApplication::translate("GuiPanel", "Requires:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GuiPanel", "texture access", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("GuiPanel", "map:", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("GuiPanel", "colorOp:", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("GuiPanel", "alphaOp:", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("GuiPanel", "flags:", 0, QApplication::UnicodeUTF8));
        bodyData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("GuiPanel", "piece", 0, QApplication::UnicodeUTF8));
        labelAxisRadius->setText(QApplication::translate("GuiPanel", "axisA:", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("GuiPanel", "axisB:", 0, QApplication::UnicodeUTF8));
        leBodyBZ->setText(QString());
        leBodyBY->setText(QString());
        leBodyBX->setText(QString());
        label_41->setText(QApplication::translate("GuiPanel", "radius:", 0, QApplication::UnicodeUTF8));
        leBodyRad->setText(QString());
        label_43->setText(QApplication::translate("GuiPanel", "verts:", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("GuiPanel", "faces:", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("GuiPanel", "flags:", 0, QApplication::UnicodeUTF8));
        leBodyFlags->setText(QString());
        label_45->setText(QApplication::translate("GuiPanel", "sign:", 0, QApplication::UnicodeUTF8));
        leBodySign->setText(QString());
        label_37->setText(QApplication::translate("GuiPanel", "piece list:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(GuiPanel);
    } // retranslateUi

};

namespace Ui {
    class GuiPanel: public Ui_GuiPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPANEL_H
