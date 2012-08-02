#ifndef GUIPANEL_H
#define GUIPANEL_H

#include <QtGui/QWidget>
#include <QModelIndexList>
#include <map>

class BrfData;
class BrfAnimation;

namespace Ui {
    class GuiPanel;
}

typedef std::map< std::string, std::string > MapSS;

class GuiPanel : public QWidget {
    Q_OBJECT
public:
    GuiPanel(QWidget *parent, MapSS *m);
    ~GuiPanel();
    BrfData* data;
    BrfData* reference;
    void setReference(BrfData*);
    void setAnimation(const BrfAnimation* a);
    int getCurrentSubpieceIndex(int brfObjectType) const;


protected:
    MapSS *mapMT;
    void changeEvent(QEvent *e);
    enum{MAXSEL=500};
    int _selectedIndex;

public:
    Ui::GuiPanel *ui;
    int displaying;

    QAction *textureAccessDup;
    QAction *textureAccessDel;
    QAction *textureAccessAdd;

    QAction *bodyPartDup;
    QAction *bodyPartDel;
    QAction *bodyPartAdd;

private slots:
    void on_listView_customContextMenuRequested(QPoint pos);
    void on_lvTextAcc_customContextMenuRequested(QPoint pos);
    void updateVisibility();
public slots:
    void updateShaderTextaccSize();
    void updateBodyPartSize();
    void updateShaderTextaccData();
    void updateBodyPartData();
    void setSelection(const QModelIndexList &, int k);
    void updateFrameNumber(int i);
    void updateMaterial(QString st);
    void updateRefAnimation();
    void setRefAnimation(int i);
};

#endif // GUIPANEL_H
