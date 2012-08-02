/********************************************************************************
** Form generated from reading ui file 'guipanel.ui'
**
** Created: Fri Sep 17 19:50:42 2010
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
#include <QtGui/QSlider>
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
    QLabel *labMatName;
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
    QWidget *viewRuler;
    QSpinBox *rulerSpin;
    QSlider *rulerSlid;
    QCheckBox *cbRuler;
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
    QLabel *label_39;
    QLabel *label_20;
    QLCDNumber *boxTextureMipmap;
    QLCDNumber *boxTextureFileSize;
    QLCDNumber *boxTextureResY;
    QLabel *label_24;
    QLCDNumber *boxTextureResX;
    QLabel *label_38;
    QLineEdit *boxTextureFormat;
    QLabel *label_46;
    QLabel *labBackT;
    QGroupBox *animationData;
    QLCDNumber *boxAniMinFrame;
    QLCDNumber *boxAniNBones;
    QLCDNumber *boxAniNFrames;
    QLCDNumber *boxAniMaxFrame;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QWidget *meshDataAni_2;
    QLabel *label_12;
    QSpinBox *frameNumberAni;
    QLineEdit *timeOfFrameAni;
    QLabel *label_13;
    QGroupBox *materialData;
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
    QLabel *label_25;
    QLineEdit *leMatCoeff;
    QLineEdit *leMatFlags;
    QLabel *label_26;
    QPushButton *buFlagMat;
    QLabel *label_32;
    QSpinBox *leMatRendOrd;
    QLabel *labelShader;
    QLabel *labelDiffuseA;
    QLabel *labelDiffuseB;
    QLabel *labelBump;
    QLabel *labelEnviro;
    QLabel *labelSpecular;
    QLabel *labBackM;
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
    QLabel *labBackS;
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
        GuiPanel->resize(200, 1913);
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
        labMatName = new QLabel(meshData);
        labMatName->setObjectName(QString::fromUtf8("labMatName"));
        labMatName->setGeometry(QRect(5, 20, 51, 20));
        labMatName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNVerts = new QLCDNumber(meshData);
        boxNVerts->setObjectName(QString::fromUtf8("boxNVerts"));
        boxNVerts->setGeometry(QRect(47, 130, 42, 24));
        boxNVerts->setNumDigits(6);
        boxNVerts->setSegmentStyle(QLCDNumber::Flat);
        boxFlags = new QLineEdit(meshData);
        boxFlags->setObjectName(QString::fromUtf8("boxFlags"));
        boxFlags->setGeometry(QRect(60, 80, 71, 21));
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
        boxNFaces->setNumDigits(6);
        boxNFaces->setSegmentStyle(QLCDNumber::Flat);
        label_8 = new QLabel(meshData);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 110, 41, 20));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNPos = new QLCDNumber(meshData);
        boxNPos->setObjectName(QString::fromUtf8("boxNPos"));
        boxNPos->setGeometry(QRect(93, 130, 42, 24));
        boxNPos->setNumDigits(6);
        boxNPos->setSegmentStyle(QLCDNumber::Flat);
        boxNPos->setProperty("intValue", QVariant(12));
        label_10 = new QLabel(meshData);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(93, 110, 41, 20));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxNFrames = new QLCDNumber(meshData);
        boxNFrames->setObjectName(QString::fromUtf8("boxNFrames"));
        boxNFrames->setGeometry(QRect(136, 130, 42, 24));
        boxNFrames->setSmallDecimalPoint(false);
        boxNFrames->setNumDigits(6);
        boxNFrames->setSegmentStyle(QLCDNumber::Flat);
        boxNFrames->setProperty("intValue", QVariant(12));
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
        generalView->setGeometry(QRect(10, 230, 181, 191));
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
        viewMeshRendering->setMinimumSize(QSize(0, 55));
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
        cbTexture->setChecked(true);

        verticalLayout->addWidget(viewMeshRendering);

        viewFloor = new QWidget(generalView);
        viewFloor->setObjectName(QString::fromUtf8("viewFloor"));
        viewFloor->setMinimumSize(QSize(0, 15));
        cbFloor = new QCheckBox(viewFloor);
        cbFloor->setObjectName(QString::fromUtf8("cbFloor"));
        cbFloor->setGeometry(QRect(10, 0, 72, 18));
        cbFloor->setChecked(true);

        verticalLayout->addWidget(viewFloor);

        viewRuler = new QWidget(generalView);
        viewRuler->setObjectName(QString::fromUtf8("viewRuler"));
        viewRuler->setMinimumSize(QSize(0, 22));
        rulerSpin = new QSpinBox(viewRuler);
        rulerSpin->setObjectName(QString::fromUtf8("rulerSpin"));
        rulerSpin->setGeometry(QRect(140, 0, 41, 22));
        rulerSpin->setMaximum(511);
        rulerSpin->setValue(100);
        rulerSlid = new QSlider(viewRuler);
        rulerSlid->setObjectName(QString::fromUtf8("rulerSlid"));
        rulerSlid->setGeometry(QRect(59, 1, 80, 20));
        rulerSlid->setMaximum(300);
        rulerSlid->setValue(100);
        rulerSlid->setOrientation(Qt::Horizontal);
        cbRuler = new QCheckBox(viewRuler);
        cbRuler->setObjectName(QString::fromUtf8("cbRuler"));
        cbRuler->setGeometry(QRect(10, 0, 51, 17));
        cbRuler->setChecked(false);

        verticalLayout->addWidget(viewRuler);

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

        verticalLayout->setStretch(8, 1);
        textureData = new QGroupBox(GuiPanel);
        textureData->setObjectName(QString::fromUtf8("textureData"));
        textureData->setGeometry(QRect(10, 920, 181, 181));
        label_9 = new QLabel(textureData);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(4, 30, 51, 20));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTextureFlags = new QLineEdit(textureData);
        boxTextureFlags->setObjectName(QString::fromUtf8("boxTextureFlags"));
        boxTextureFlags->setGeometry(QRect(62, 30, 111, 20));
        boxTextureFlags->setReadOnly(false);
        label_39 = new QLabel(textureData);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(108, 90, 20, 20));
        label_39->setAlignment(Qt::AlignCenter);
        label_20 = new QLabel(textureData);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(4, 90, 51, 20));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTextureMipmap = new QLCDNumber(textureData);
        boxTextureMipmap->setObjectName(QString::fromUtf8("boxTextureMipmap"));
        boxTextureMipmap->setGeometry(QRect(61, 120, 46, 21));
        boxTextureMipmap->setNumDigits(7);
        boxTextureMipmap->setSegmentStyle(QLCDNumber::Flat);
        boxTextureFileSize = new QLCDNumber(textureData);
        boxTextureFileSize->setObjectName(QString::fromUtf8("boxTextureFileSize"));
        boxTextureFileSize->setGeometry(QRect(61, 150, 46, 21));
        boxTextureFileSize->setNumDigits(7);
        boxTextureFileSize->setSegmentStyle(QLCDNumber::Flat);
        boxTextureResY = new QLCDNumber(textureData);
        boxTextureResY->setObjectName(QString::fromUtf8("boxTextureResY"));
        boxTextureResY->setGeometry(QRect(126, 90, 46, 21));
        boxTextureResY->setNumDigits(7);
        boxTextureResY->setSegmentStyle(QLCDNumber::Flat);
        label_24 = new QLabel(textureData);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(4, 150, 51, 20));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTextureResX = new QLCDNumber(textureData);
        boxTextureResX->setObjectName(QString::fromUtf8("boxTextureResX"));
        boxTextureResX->setGeometry(QRect(61, 90, 46, 21));
        boxTextureResX->setNumDigits(7);
        boxTextureResX->setSegmentStyle(QLCDNumber::Flat);
        label_38 = new QLabel(textureData);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(4, 120, 51, 20));
        label_38->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxTextureFormat = new QLineEdit(textureData);
        boxTextureFormat->setObjectName(QString::fromUtf8("boxTextureFormat"));
        boxTextureFormat->setEnabled(false);
        boxTextureFormat->setGeometry(QRect(62, 60, 111, 20));
        boxTextureFormat->setReadOnly(true);
        label_46 = new QLabel(textureData);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(4, 60, 51, 20));
        label_46->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labBackT = new QLabel(textureData);
        labBackT->setObjectName(QString::fromUtf8("labBackT"));
        labBackT->setGeometry(QRect(145, -2, 31, 20));
        labBackT->setAutoFillBackground(true);
        labBackT->setAlignment(Qt::AlignCenter);
        animationData = new QGroupBox(GuiPanel);
        animationData->setObjectName(QString::fromUtf8("animationData"));
        animationData->setGeometry(QRect(10, 1460, 181, 141));
        boxAniMinFrame = new QLCDNumber(animationData);
        boxAniMinFrame->setObjectName(QString::fromUtf8("boxAniMinFrame"));
        boxAniMinFrame->setGeometry(QRect(70, 80, 46, 21));
        boxAniMinFrame->setNumDigits(7);
        boxAniMinFrame->setSegmentStyle(QLCDNumber::Flat);
        boxAniNBones = new QLCDNumber(animationData);
        boxAniNBones->setObjectName(QString::fromUtf8("boxAniNBones"));
        boxAniNBones->setGeometry(QRect(70, 20, 46, 21));
        boxAniNBones->setNumDigits(7);
        boxAniNBones->setSegmentStyle(QLCDNumber::Flat);
        boxAniNFrames = new QLCDNumber(animationData);
        boxAniNFrames->setObjectName(QString::fromUtf8("boxAniNFrames"));
        boxAniNFrames->setGeometry(QRect(70, 50, 46, 21));
        boxAniNFrames->setNumDigits(7);
        boxAniNFrames->setSegmentStyle(QLCDNumber::Flat);
        boxAniMaxFrame = new QLCDNumber(animationData);
        boxAniMaxFrame->setObjectName(QString::fromUtf8("boxAniMaxFrame"));
        boxAniMaxFrame->setGeometry(QRect(129, 80, 46, 21));
        boxAniMaxFrame->setNumDigits(7);
        boxAniMaxFrame->setSegmentStyle(QLCDNumber::Flat);
        label_14 = new QLabel(animationData);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(14, 20, 51, 20));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(animationData);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(14, 50, 51, 20));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(animationData);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(14, 80, 51, 20));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_17 = new QLabel(animationData);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(113, 80, 20, 20));
        label_17->setAlignment(Qt::AlignCenter);
        meshDataAni_2 = new QWidget(animationData);
        meshDataAni_2->setObjectName(QString::fromUtf8("meshDataAni_2"));
        meshDataAni_2->setGeometry(QRect(0, 103, 181, 41));
        label_12 = new QLabel(meshDataAni_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(2, 11, 71, 16));
        frameNumberAni = new QSpinBox(meshDataAni_2);
        frameNumberAni->setObjectName(QString::fromUtf8("frameNumberAni"));
        frameNumberAni->setGeometry(QRect(70, 7, 46, 21));
        frameNumberAni->setWrapping(true);
        frameNumberAni->setFrame(false);
        frameNumberAni->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timeOfFrameAni = new QLineEdit(meshDataAni_2);
        timeOfFrameAni->setObjectName(QString::fromUtf8("timeOfFrameAni"));
        timeOfFrameAni->setGeometry(QRect(129, 7, 46, 21));
        timeOfFrameAni->setReadOnly(false);
        label_13 = new QLabel(meshDataAni_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(122, 10, 20, 20));
        materialData = new QGroupBox(GuiPanel);
        materialData->setObjectName(QString::fromUtf8("materialData"));
        materialData->setGeometry(QRect(10, 1130, 181, 281));
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
        label_25 = new QLabel(materialData);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(-5, 195, 61, 14));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leMatCoeff = new QLineEdit(materialData);
        leMatCoeff->setObjectName(QString::fromUtf8("leMatCoeff"));
        leMatCoeff->setGeometry(QRect(60, 190, 37, 20));
        leMatFlags = new QLineEdit(materialData);
        leMatFlags->setObjectName(QString::fromUtf8("leMatFlags"));
        leMatFlags->setGeometry(QRect(60, 249, 81, 20));
        label_26 = new QLabel(materialData);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(15, 254, 41, 14));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        buFlagMat = new QPushButton(materialData);
        buFlagMat->setObjectName(QString::fromUtf8("buFlagMat"));
        buFlagMat->setGeometry(QRect(140, 250, 31, 20));
        label_32 = new QLabel(materialData);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(-5, 225, 61, 14));
        label_32->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leMatRendOrd = new QSpinBox(materialData);
        leMatRendOrd->setObjectName(QString::fromUtf8("leMatRendOrd"));
        leMatRendOrd->setGeometry(QRect(60, 220, 37, 22));
        leMatRendOrd->setMinimum(-8);
        leMatRendOrd->setMaximum(7);
        labelShader = new QLabel(materialData);
        labelShader->setObjectName(QString::fromUtf8("labelShader"));
        labelShader->setGeometry(QRect(3, 34, 52, 14));
        labelShader->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelDiffuseA = new QLabel(materialData);
        labelDiffuseA->setObjectName(QString::fromUtf8("labelDiffuseA"));
        labelDiffuseA->setGeometry(QRect(3, 64, 52, 14));
        labelDiffuseA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelDiffuseB = new QLabel(materialData);
        labelDiffuseB->setObjectName(QString::fromUtf8("labelDiffuseB"));
        labelDiffuseB->setGeometry(QRect(3, 84, 52, 14));
        labelDiffuseB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelBump = new QLabel(materialData);
        labelBump->setObjectName(QString::fromUtf8("labelBump"));
        labelBump->setGeometry(QRect(3, 104, 52, 14));
        labelBump->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelEnviro = new QLabel(materialData);
        labelEnviro->setObjectName(QString::fromUtf8("labelEnviro"));
        labelEnviro->setGeometry(QRect(3, 124, 52, 14));
        labelEnviro->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelSpecular = new QLabel(materialData);
        labelSpecular->setObjectName(QString::fromUtf8("labelSpecular"));
        labelSpecular->setGeometry(QRect(3, 144, 52, 14));
        labelSpecular->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labBackM = new QLabel(materialData);
        labBackM->setObjectName(QString::fromUtf8("labBackM"));
        labBackM->setGeometry(QRect(145, -2, 31, 20));
        labBackM->setAutoFillBackground(true);
        labBackM->setAlignment(Qt::AlignCenter);
        skeletonData = new QGroupBox(GuiPanel);
        skeletonData->setObjectName(QString::fromUtf8("skeletonData"));
        skeletonData->setGeometry(QRect(10, 1410, 181, 51));
        label_27 = new QLabel(skeletonData);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(13, 20, 51, 20));
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        boxSkelNBones = new QLCDNumber(skeletonData);
        boxSkelNBones->setObjectName(QString::fromUtf8("boxSkelNBones"));
        boxSkelNBones->setGeometry(QRect(70, 20, 46, 21));
        boxSkelNBones->setNumDigits(7);
        boxSkelNBones->setSegmentStyle(QLCDNumber::Flat);
        shaderData = new QGroupBox(GuiPanel);
        shaderData->setObjectName(QString::fromUtf8("shaderData"));
        shaderData->setGeometry(QRect(10, 630, 181, 271));
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
        leShaderFlags->setGeometry(QRect(60, 120, 111, 20));
        leShaderFlags->setReadOnly(false);
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
        labBackS = new QLabel(shaderData);
        labBackS->setObjectName(QString::fromUtf8("labBackS"));
        labBackS->setGeometry(QRect(145, -2, 31, 20));
        labBackS->setAutoFillBackground(true);
        labBackS->setAlignment(Qt::AlignCenter);
        bodyData = new QGroupBox(GuiPanel);
        bodyData->setObjectName(QString::fromUtf8("bodyData"));
        bodyData->setGeometry(QRect(10, 1600, 181, 231));
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
        leBodyNVert->setNumDigits(8);
        leBodyNVert->setSegmentStyle(QLCDNumber::Flat);
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
        leBodyNFace->setNumDigits(8);
        leBodyNFace->setSegmentStyle(QLCDNumber::Flat);
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
#ifndef QT_NO_STATUSTIP
        boxMaterial->setStatusTip(QApplication::translate("GuiPanel", "Set material used by this mesh.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        boxMaterial->setText(QString());
#ifndef QT_NO_STATUSTIP
        labMatName->setStatusTip(QApplication::translate("GuiPanel", "Click to follow material (if known in module.ini).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labMatName->setText(QApplication::translate("GuiPanel", "<a href=\"link\">Material</a>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNVerts->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxNVerts->setStatusTip(QApplication::translate("GuiPanel", "Number of vertices of this mesh, including seams.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        boxFlags->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxFlags->setStatusTip(QApplication::translate("GuiPanel", "Mesh flags (unknown meaning)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        boxFlags->setInputMask(QApplication::translate("GuiPanel", "000000000000; ", 0, QApplication::UnicodeUTF8));
        boxFlags->setText(QString());
        label_3->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxTexture->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxTexture->setStatusTip(QApplication::translate("GuiPanel", "Diffuse texture name (depends on material)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        boxTexture->setText(QString());
        label_6->setText(QApplication::translate("GuiPanel", "Texture:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("GuiPanel", "faces:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNFaces->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxNFaces->setStatusTip(QApplication::translate("GuiPanel", "Number of triangles of this mesh.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_8->setText(QApplication::translate("GuiPanel", "vert:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNPos->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxNPos->setStatusTip(QApplication::translate("GuiPanel", "Number of different X,Y, Z position of this mesh.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_10->setText(QApplication::translate("GuiPanel", "pos:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxNFrames->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxNFrames->setStatusTip(QApplication::translate("GuiPanel", "Number of frames (>1 for vertex animated meshes).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_11->setText(QApplication::translate("GuiPanel", "frames:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("GuiPanel", "Time of frame", 0, QApplication::UnicodeUTF8));
        frameNumber->setSuffix(QString());
        frameNumber->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        timeOfFrame->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        timeOfFrame->setStatusTip(QApplication::translate("GuiPanel", "Time of this frame", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        timeOfFrame->setText(QString());
        label_4->setText(QApplication::translate("GuiPanel", ":", 0, QApplication::UnicodeUTF8));
        generalView->setTitle(QApplication::translate("GuiPanel", "View", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GuiPanel", "Skin:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbSkin->setStatusTip(QApplication::translate("GuiPanel", "Select a reference skin to show this animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        rbNocolor->setStatusTip(QApplication::translate("GuiPanel", "Show no color (all white).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rbNocolor->setText(QApplication::translate("GuiPanel", "No Color", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        rbVertexcolor->setStatusTip(QApplication::translate("GuiPanel", "Use vertex color as defined in the BRF file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rbVertexcolor->setText(QApplication::translate("GuiPanel", "Vertex Color", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbLighting->setStatusTip(QApplication::translate("GuiPanel", "Enable disable lighting,", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbLighting->setText(QApplication::translate("GuiPanel", "Lighting", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        rbRiggingcolor->setStatusTip(QApplication::translate("GuiPanel", "Show rigging by coloring mesh according to attached bones", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rbRiggingcolor->setText(QApplication::translate("GuiPanel", "Rigging Color", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbWireframe->setStatusTip(QApplication::translate("GuiPanel", "Show/hide wireframe.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbWireframe->setText(QApplication::translate("GuiPanel", "WireFrame", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbTexture->setStatusTip(QApplication::translate("GuiPanel", "Enable disable texture mapping.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbTexture->setText(QApplication::translate("GuiPanel", "Texture", 0, QApplication::UnicodeUTF8));
        cbTexture->setShortcut(QApplication::translate("GuiPanel", "Ctrl+R", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbFloor->setStatusTip(QApplication::translate("GuiPanel", "Show/hide a floor at Y = 0.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbFloor->setText(QApplication::translate("GuiPanel", "Floor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        rulerSpin->setStatusTip(QApplication::translate("GuiPanel", "Current weapon reach.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        rulerSlid->setStatusTip(QApplication::translate("GuiPanel", "Current weapon reach.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        cbRuler->setStatusTip(QApplication::translate("GuiPanel", "Show ruler to measure weapon reach.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cbRuler->setText(QApplication::translate("GuiPanel", "Ruler", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbRefani->setStatusTip(QApplication::translate("GuiPanel", "Select a reference animation to view rigged meshes.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_21->setText(QApplication::translate("GuiPanel", "Animation:", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("GuiPanel", "Skeleton:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        cbRefSkel->setStatusTip(QApplication::translate("GuiPanel", "Select a reference skeleton (e.g. human or horse)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        buPause->setStatusTip(QApplication::translate("GuiPanel", "Pause the animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buPause->setText(QApplication::translate("GuiPanel", "||", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        buPlay->setStatusTip(QApplication::translate("GuiPanel", "Play the animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buPlay->setText(QApplication::translate("GuiPanel", ">", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        buStop->setStatusTip(QApplication::translate("GuiPanel", "Stop the animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buStop->setText(QApplication::translate("GuiPanel", "[]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        buStepF->setStatusTip(QApplication::translate("GuiPanel", "Next frame.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buStepF->setText(QApplication::translate("GuiPanel", ">|", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        buStepB->setStatusTip(QApplication::translate("GuiPanel", "Prev frame.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        buStepB->setText(QApplication::translate("GuiPanel", "|<", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        rbAlphaNo->setStatusTip(QApplication::translate("GuiPanel", "Show RGB channel of texture.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rbAlphaNo->setText(QApplication::translate("GuiPanel", "&RGB", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        rbAlphaTransparent->setStatusTip(QApplication::translate("GuiPanel", "Show alpha transparency (alpha = 0 means transparent).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rbAlphaTransparent->setText(QApplication::translate("GuiPanel", "Alpha &Transparency", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        rbAlphaColor->setStatusTip(QApplication::translate("GuiPanel", "Show alpha channel of textures.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rbAlphaColor->setText(QApplication::translate("GuiPanel", "&Alpha", 0, QApplication::UnicodeUTF8));
        textureData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxTextureFlags->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxTextureFlags->setStatusTip(QApplication::translate("GuiPanel", "Texture flags (unknown meaning)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        boxTextureFlags->setInputMask(QApplication::translate("GuiPanel", "000000000000; ", 0, QApplication::UnicodeUTF8));
        boxTextureFlags->setText(QString());
        label_39->setText(QApplication::translate("GuiPanel", "x", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("GuiPanel", "res:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        boxTextureFileSize->setStatusTip(QApplication::translate("GuiPanel", "Space taken on disk (compressed).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_24->setText(QApplication::translate("GuiPanel", "KBytes:", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("GuiPanel", "# mipmap:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        boxTextureFormat->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        boxTextureFormat->setStatusTip(QApplication::translate("GuiPanel", "Format of dds file on disk.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        boxTextureFormat->setText(QString());
        label_46->setText(QApplication::translate("GuiPanel", "Format:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labBackT->setStatusTip(QApplication::translate("GuiPanel", "Click to go back to the material.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labBackT->setText(QApplication::translate("GuiPanel", "(<a href=\"link\">back</a>)", 0, QApplication::UnicodeUTF8));
        animationData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        boxAniMinFrame->setStatusTip(QApplication::translate("GuiPanel", "First frame of the animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        boxAniNBones->setStatusTip(QApplication::translate("GuiPanel", "Number of bones this animation is made for.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        boxAniNFrames->setStatusTip(QApplication::translate("GuiPanel", "Number of frames in this animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        boxAniMaxFrame->setStatusTip(QApplication::translate("GuiPanel", "Last frame of the animation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_14->setText(QApplication::translate("GuiPanel", "# bones:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("GuiPanel", "# frames:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("GuiPanel", "interval:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("GuiPanel", "-", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("GuiPanel", "time of frame", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        frameNumberAni->setStatusTip(QApplication::translate("GuiPanel", "Frame number (1 = first)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        frameNumberAni->setSuffix(QString());
        frameNumberAni->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        timeOfFrameAni->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        timeOfFrameAni->setStatusTip(QApplication::translate("GuiPanel", "Time of this frame (must always be increasing)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        timeOfFrameAni->setText(QString());
        label_13->setText(QApplication::translate("GuiPanel", ":", 0, QApplication::UnicodeUTF8));
        materialData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        leMatShader->setStatusTip(QApplication::translate("GuiPanel", "Shader used by this material.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatDifA->setStatusTip(QApplication::translate("GuiPanel", "Main diffuse (RGB color) texture", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatDifB->setStatusTip(QApplication::translate("GuiPanel", "Second diffuse (RGB color) texture", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatBump->setStatusTip(QApplication::translate("GuiPanel", "Bumpmap texture", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatEnv->setStatusTip(QApplication::translate("GuiPanel", "Environment map texture", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatR->setStatusTip(QApplication::translate("GuiPanel", "Red component of specular color.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatG->setStatusTip(QApplication::translate("GuiPanel", "Green component of specular color.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatB->setStatusTip(QApplication::translate("GuiPanel", "Blue component of specular color.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_22->setText(QApplication::translate("GuiPanel", "Spec RGB:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        leMatSpec->setStatusTip(QApplication::translate("GuiPanel", "Specular map texture.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_25->setText(QApplication::translate("GuiPanel", "Coeff:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        leMatCoeff->setStatusTip(QApplication::translate("GuiPanel", "Specular coefficient (glossiness). Higher = smaller brighter reflections.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        leMatFlags->setStatusTip(QApplication::translate("GuiPanel", "Flags (click on button to edit bits)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_26->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
        buFlagMat->setText(QApplication::translate("GuiPanel", "...", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("GuiPanel", "Rend.Ord:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        leMatRendOrd->setStatusTip(QApplication::translate("GuiPanel", "Rendering order (negative = soonest)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        labelShader->setStatusTip(QApplication::translate("GuiPanel", "Click to open file containing shader.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelShader->setText(QApplication::translate("GuiPanel", "<a href=\"link\">Shader</a>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labelDiffuseA->setStatusTip(QApplication::translate("GuiPanel", "Click to open file containing this texture.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelDiffuseA->setText(QApplication::translate("GuiPanel", "<a href=\"link\">DiffuseA</a>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labelDiffuseB->setStatusTip(QApplication::translate("GuiPanel", "Click to open file containing this texture.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelDiffuseB->setText(QApplication::translate("GuiPanel", "<a href=\"link\">DiffuseB</a>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labelBump->setStatusTip(QApplication::translate("GuiPanel", "Click to open file containing this bumpmap,", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelBump->setText(QApplication::translate("GuiPanel", "<a href=\"link\">Bump</a>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labelEnviro->setStatusTip(QApplication::translate("GuiPanel", "Click to open file containing this environment map.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelEnviro->setText(QApplication::translate("GuiPanel", "<a href=\"link\">Enviro</a>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labelSpecular->setStatusTip(QApplication::translate("GuiPanel", "Click to open file containing this specular map.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labelSpecular->setText(QApplication::translate("GuiPanel", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"link\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">Specular</span></a><span style=\" font-size:8pt;\">:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        labBackM->setStatusTip(QApplication::translate("GuiPanel", "Click to go back to the mesh.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labBackM->setText(QApplication::translate("GuiPanel", "(<a href=\"link\">back</a>)", 0, QApplication::UnicodeUTF8));
        skeletonData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("GuiPanel", "# bones:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        boxSkelNBones->setStatusTip(QApplication::translate("GuiPanel", "Number of bones composing this skeleton.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        shaderData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("GuiPanel", "Technique:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        leShaderTechnique->setStatusTip(QApplication::translate("GuiPanel", "Technique: name of the \"technique\" inside mb.fx file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        leShaderTechnique->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        leShaderTechnique->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        leShaderTechnique->setAccessibleDescription(QApplication::translate("GuiPanel", "Technique: name of the \"technique\" inside mb.fx file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_29->setText(QApplication::translate("GuiPanel", "Fallback:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        leShaderFallback->setStatusTip(QApplication::translate("GuiPanel", "Which other shader to use if requirements are not met.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        leShaderFallback->setText(QString());
        label_30->setText(QApplication::translate("GuiPanel", "Flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leShaderFlags->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        leShaderFlags->setStatusTip(QApplication::translate("GuiPanel", "Shader flags (no known meaning).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        leShaderFlags->setInputMask(QApplication::translate("GuiPanel", "000000000000; ", 0, QApplication::UnicodeUTF8));
        leShaderFlags->setText(QString());
#ifndef QT_NO_STATUSTIP
        leShaderRequires->setStatusTip(QApplication::translate("GuiPanel", "Specify if a DDX version is required here", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        leShaderRequires->setText(QString());
        label_31->setText(QApplication::translate("GuiPanel", "Requires:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GuiPanel", "texture access", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("GuiPanel", "map:", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("GuiPanel", "colorOp:", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("GuiPanel", "alphaOp:", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("GuiPanel", "flags:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        lvTextAcc->setStatusTip(QApplication::translate("GuiPanel", "Texture access index", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        labBackS->setText(QApplication::translate("GuiPanel", "(<a href=\"link\">back</a>)", 0, QApplication::UnicodeUTF8));
        bodyData->setTitle(QApplication::translate("GuiPanel", "Data", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        lvBodyPart->setStatusTip(QApplication::translate("GuiPanel", "Select a subpiece composing this collision object.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
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
