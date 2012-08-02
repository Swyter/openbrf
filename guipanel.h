#ifndef GUIPANEL_H
#define GUIPANEL_H

#include <QtGui/QWidget>
#include <QModelIndexList>
#include <map>
#include <ddsData.h>

class BrfData;
class IniData;
class BrfAnimation;
class QLineEdit;

namespace Ui {
    class GuiPanel;
}

typedef std::map< std::string, std::string > MapSS;

class GuiPanel : public QWidget {
    Q_OBJECT
public:
    GuiPanel(QWidget *parent, IniData &inidata);
    ~GuiPanel();
    BrfData* data;
    BrfData* reference;
    IniData &inidata;
    void setReference(BrfData*);
    void setAnimation(const BrfAnimation* a);
    int getCurrentSubpieceIndex(int brfObjectType) const;

    void setIniData(const IniData &inidata);

    enum{DIFFUSEA, DIFFUSEB, BUMP, ENVIRO, SPECULAR, SHADERNAME } curMaterialFocus;
    QLineEdit* materialLeFocus();

protected:
    //MapSS *mapMT;
    void changeEvent(QEvent *e);
    //enum{MAXSEL=500};
    int _selectedIndex;

public:
    Ui::GuiPanel *ui;
    int displaying;
    int frameTime[10000]; // how elegant is that? ;)

    QAction *textureAccessDup;
    QAction *textureAccessDel;
    QAction *textureAccessAdd;

    QAction *bodyPartDup;
    QAction *bodyPartDel;
    QAction *bodyPartAdd;


signals:
    void followLink();
    //void dataMaterialChanged();

private slots:
    void on_listView_customContextMenuRequested(QPoint pos);
    void on_lvTextAcc_customContextMenuRequested(QPoint pos);
    void updateVisibility();
    void setRulerLenght(int l);
    void setTextureData(DdsData d);

public slots:
    void updateHighlight();
    void updateShaderTextaccSize();
    void updateBodyPartSize();
    void updateShaderTextaccData();
    void updateBodyPartData();
    void setSelection(const QModelIndexList &, int k);
    void updateFrameNumber(int i);
    void updateMaterial(QString st);
    void updateRefAnimation();
    void setRefAnimation(int i);

    void showMaterialDiffuseA();
    void showMaterialDiffuseB();
    void showMaterialBump();
    void showMaterialEnviro();
    void showMaterialSpecular();
    void showMaterialShader();

    void setNavigationStackDepth(int k);

};

#endif // GUIPANEL_H
