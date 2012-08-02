#ifndef GUIPANEL_H
#define GUIPANEL_H

#include <QtGui/QWidget>
#include <QModelIndexList>
#include <map>

class BrfData;

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


protected:
    MapSS *mapMT;
    void changeEvent(QEvent *e);
    enum{MAXSEL=500};

public:
    Ui::GuiPanel *ui;
    int displaying;
private slots:
    void updateVisibility();
public slots:
    void setSelection(const QModelIndexList &, int k);
    void updateFrameNumber(int i);
    void updateMaterial(QString st);
};

#endif // GUIPANEL_H
